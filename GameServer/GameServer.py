import socket,select
import time
import random

actionDic={ 'INITMAINROLE':'1',
            'INITSPRITE':'2',
           'SPRITEDESTORYPLAYER':'3',
           'SPRITEREVIVE':'4',
            'STAND':'0',	
            'WALK':'8',
            'ATTACK':'16',
           'DIE':'24',}

OBJ_TYPE = {
            'player':'1',
            'wolf':'2',
            }

OBJ_STATU = {
            'dormant':'0',
            'normal':'1',
            }

LastanimalGenTimeInterval = 0
    
class SpriteDB:
    
    def __init__(self):
        self.spriteDic={}
        self.maxID = 0

    def genID(self):
        self.maxID+=1
        return self.maxID

    def getSprite(self, ID):
        if ID in self.spriteDic:
            return self.spriteDic[ID]
        else:
            return None
    
    def addSprite(self, sprite):
        self.spriteDic[sprite.ID] = sprite
        
    def removeSprite(self,ID):
        if ID in g_roleDB.spriteDic:
            del g_roleDB.spriteDic[ID]

    def findUserByName(self, name):
        for v in self.spriteDic.values():
            if name == v.name:
                return v
        return None 
    
g_roleDB = SpriteDB()
g_animalDB = SpriteDB()
g_deadRoleList = []
g_deadAnimalList = []


def GameLoop(*connList):    
    currentTime = time.time()    
    global LastanimalGenTimeInterval
    '''
        typeid, id, statu, action, targetTypeID, targetid, x, y
    '''
    global g_deadRoleList
    for role in g_deadRoleList:
        if currentTime - role.dieTime>=1:
            role.revive()
            g_deadRoleList.remove(role)

    if currentTime - LastanimalGenTimeInterval>=5:
        for animal in g_deadAnimalList:
            if currentTime - animal.dieTime>=2:
                animal.revive()
                g_deadAnimalList.remove(animal)
        LastanimalGenTimeInterval = currentTime
            
class ListeningManager:
    def __init__(self,connectionManager):
        self.cm=connectionManager
        self.listensockList=[]

    def addPort(self, port):
        listensock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        listensock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        listensock.bind(('',port))
        listensock.listen(8)
        self.listensockList.append(listensock)

    def listen(self):
        comeList,wlist,elist=select.select(self.listensockList,[],[],0.01)
        for sock in comeList:
            clientsock,clientaddr = sock.accept()
            clientsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)            
            self.cm.newConnection(clientsock)

class ConnectionManager:
    def __init__(self):
        self.connectionList=[]
        self.clientsockList=[]
    
    def newConnection(self, clientsock):
        print 'a new connection come'
        self.clientsockList.append(clientsock)
        conn = Connection(clientsock)
        self.connectionList.append(conn)
        conn.addHandler(HandlerLogon())
        
    def manage(self):        
        self.listen()
        GameLoop(*self.connectionList)
        self.send()
        self.closeConnections()
        

    def close(self, connection):
        self.clientsockList.remove(connection.sock)
        self.connectionList.remove(connection)

        connection.closeSocket()

    def listen(self):
        if len(self.connectionList)==0:
            return
        comeList,wlist,elist=select.select(self.clientsockList,[],[],0.01)
        for conn in self.connectionList:
            if conn.sock in comeList:
                if conn.receive()<0:
                    conn.close()
                    conn.handler().hungup(conn)
                    self.close(conn)
                        
    def send(self):
        for conn in self.connectionList:
            conn.sendBuffer()

    def closeConnections(self):
        for conn in self.connectionList:
            if conn.closed == True:
                self.close(conn)

class Connection:
    def __init__(self, sock):
        self.handlerList=[]
        self.translator = Translator()
        self.databuffer=''
        self.sock = sock
        self.closed = False
        self.roleID = None

    def handler(self):
        if len(self.handlerList)==0:
            return None
        else:
            return self.handlerList[-1]

    def addHandler(self, handler):
        self.handlerList.append(handler)
        handler.enter(self)
        
    def bufferData(self, data):
        self.databuffer+=data
        
    def sendBuffer(self):
        if self.closed == False:
            lensend=self.sock.send(self.databuffer)
            if self.roleID is not None and lensend>0:
                print self.roleID, 'send',self.databuffer[0:lensend]
            self.databuffer=self.databuffer[lensend:]
        
    def receive(self):
        try:
            data = self.sock.recv(512)
            if len(data)>0:
                self.translator.translate(self, data)                
                return 0
            else:
                return -1
        except Exception,x:
            print x,            
            if type(x) is socket.error:
                print socket.errorTab[x[0]]
            return -1

    def removeHandler(self):
        self.handler().leave(self)
        del self.handlerList[-1]        

        if self.handler() is not None:
            self.handler().enter(self)            

    def clearHandlers(self):
        if self.handler() is not None:
            self.handler().leave(self)

        del self.handlerList[:]   

    def close(self):        
        self.closed = True
        
    def closeSocket(self):
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()
        self.clearHandlers()
 
class HandlerLogon:
    def handle(self, connection, data):
        global g_roleDB
        global g_animalDB
        try:            
            username, password = data.split(',')
            if g_roleDB.findUserByName(username) is not None:
                print 'user is online'
                connection.translator.sendString(connection, 'user is online')
            else:
                role = Role(connection, username)
                connection.roleID = role.ID
                g_roleDB.addSprite(role)
                connection.translator.sendString(connection, role.posMsg('INITMAINROLE'))
                connection.removeHandler()

                self.sendOthers(connection, role.posMsg('INITSPRITE'))

                for role in g_roleDB.spriteDic.values():
                    if role.ID != connection.roleID:
                        connection.translator.sendString(connection, role.posMsg('INITSPRITE'))

                for sprite in g_animalDB.spriteDic.values():
                    print 'send g_animalDB'
                    connection.translator.sendString(connection, sprite.posMsg('INITSPRITE'))
                    
                connection.addHandler(HandlerLogic())
        except Exception,x:
            print x

    def hungup(self,connection):
        pass
    def enter(self, connection):
        print 'HandlerLogon enter'
        connection.translator.sendString(connection, 'username,password')
    def leave(self,connection):
        print 'HandlerLogon leave'
        pass

    def sendOthers(self, conn, data):
        global g_roleDB
        for role in g_roleDB.spriteDic.values():
            if role.ID != conn.roleID:
                role.conn.translator.sendString(role.conn, data)
    
class HandlerLogic:
    '''
        typeid, id, statu, action, targetTypeID, targetid, x, y

    '''
    def handle(self, connection,data):
        global g_roleDB
        global g_animalDB
        msgList=data.split(',')

        print 'HandlerLogic',msgList
        for v in g_roleDB.spriteDic.values():
            v.conn.translator.sendString(v.conn, data)

        role = None
        global OBJ_TYPE
        if msgList[3] == actionDic['ATTACK']:
            if msgList[4] == OBJ_TYPE['player']:
                role = g_roleDB.getSprite(int(msgList[5]))
                role.HP-=1
                if role.HP<=0:
                    role.conn.addHandler(HandlerDie())
                print connection.roleID,'at',role.x, role.y
            elif msgList[4] == OBJ_TYPE['wolf']:                
                animal = g_animalDB.getSprite(int(msgList[5]))
                animal.HP-=1
                if animal.HP<=0:
                    #for v in g_roleDB.spriteDic.values():                        
                        #v.conn.translator.sendString(v.conn, animal.dieMsg())
                    animal.die()
                    
                     
        else:
            role = g_roleDB.getSprite(connection.roleID)
            role.x, role.y = msgList[6:8]
            print connection.roleID,'at',role.x, role.y
        

    def enter(self, connection):
        print 'HandlerLogic enter'
        pass
        
    def hungup(self,connection):
        global g_roleDB
        global g_deadRoleList

        role = g_roleDB.getSprite(connection.roleID)
        if role in g_deadRoleList:
            g_deadRoleList.remove(g_roleDB.getSprite(connection.roleID))
        
        self.sendOthers(connection, g_roleDB.getSprite(connection.roleID).hungupMsg())
        g_roleDB.removeSprite(connection.roleID)
        
        
    def sendOthers(self, conn, data):
        global g_roleDB
        for role in g_roleDB.spriteDic.values():
            if role.ID != conn.roleID:
                role.conn.translator.sendString(role.conn, data)
                
    def leave(self,connection):
        print 'HandlerLogic leave'
        pass
class HandlerDie:
    '''
        typeid, id, statu, action, targetTypeID, targetid, x, y
    '''
    def handle(self, connection, data):
        '''
        global g_roleDB
        global actionDic
        msgList=data.split(',')

        if msgList[3] == actionDic['SPRITEREVIVE']:        
            for v in g_roleDB.spriteDic.values():
                v.conn.translator.sendString(v.conn, data)
            role = g_roleDB.getSprite(connection.roleID)
            role.HP=3
            role.x=msgList[6]
            role.y=msgList[7]
            
            connection.removeHandler()
        
        pass
        '''
    def enter(self, connection):
        global g_roleDB
        print 'HandlerDie enter'
        for v in g_roleDB.spriteDic.values():
                    v.conn.translator.sendString(v.conn, g_roleDB.getSprite(connection.roleID).dieMsg())
        g_roleDB.getSprite(connection.roleID).dieTime = time.time()
        g_deadRoleList.append(g_roleDB.getSprite(connection.roleID))

    def hungup(self,connection):
        global g_roleDB

        self.sendOthers(connection, g_roleDB.getSprite(connection.roleID).hungupMsg())
        g_roleDB.removeSprite(connection.roleID)
          
    def leave(self,connection):
        print 'HandlerDie leave'
        pass

    def sendOthers(self, conn, data):
        global g_roleDB
        for role in g_roleDB.spriteDic.values():
            if role.ID != conn.roleID:
                role.conn.translator.sendString(role.conn, data)
    
class Translator:
    def __init__(self):
        self.databuffer = ''

    def translate(self, conn, data):
        self.databuffer+=data
        msg = self.getOneMsg()
        if len(msg)>0:
            conn.handler().handle(conn, msg)
    def sendString(self, conn, data):
        conn.bufferData('['+data+']')

    def getOneMsg(self):
        posBegin = self.databuffer.find('[')
        if posBegin>=0:
            self.databuffer = self.databuffer[posBegin:]
            posEnd = self.databuffer.find(']')
            if posEnd>0:
                msg = self.databuffer[1:posEnd]
                self.databuffer = self.databuffer[posEnd+1:]
                return msg
            
            

class Role:
    '''
        typeid, id, statu, action, targetTypeID, targetid, x, y
    '''
    TypeID = OBJ_TYPE['player']
    def __init__(self, connection, name):
        global g_roleDB
        global OBJ_STATU
        self.ID  = g_roleDB.genID()
        self.name=name
        self.HP=3
        self.x = 320
        self.y = 240
        self.conn = connection
        self.statu = OBJ_STATU['normal']
        self.dieTime = 0
 
    def posMsg(self, typeName):
        global actionDic
        return Role.TypeID+','+str(self.ID)+','+self.statu+','+actionDic[typeName]+',0,0,'+str(self.x)+','+str(self.y)

    def dieMsg(self):
        global actionDic
        return Role.TypeID+','+str(self.ID)+','+self.statu+','+actionDic['DIE']+',0,0,'+str(self.x)+','+str(self.y)

    def reviveMsg(self):
        global actionDic
        return self.TypeID+','+str(self.ID)+','+self.statu+','+actionDic['SPRITEREVIVE']+',0,0,'+str(self.x)+','+str(self.y)
    
    def hungupMsg(self):
        global actionDic
        return Role.TypeID+','+str(self.ID)+','+self.statu+','+actionDic['SPRITEDESTORYPLAYER']+',0,0,0,0'

    def revive(self):
        global g_roleDB
        self.HP=3
        self.x = 320
        self.y = 240
        
        for v in g_roleDB.spriteDic.values():
            v.conn.translator.sendString(v.conn, self.reviveMsg())
        
        self.conn.removeHandler()
        
    def printInfo(self):
        print self.ID
        print self.name
        print self.HP
        print self.x
        print self.y
        print self.conn
    def rebirth(self):
        pass

class Animal:
    '''
        typeid, id, statu, action, targetTypeID, targetid, x, y
    '''
    def __init__(self, TypeID):        
        global g_animalDB
        self.TypeID = TypeID
        self.HP = 5
        self.x = 0
        self.y = 0
        self.ID = g_animalDB.genID()
        self.statu = OBJ_STATU['normal']
        self.dieTime = 0

    def posMsg(self, typeName):
        global actionDic
        return self.TypeID+','+str(self.ID)+','+self.statu+','+actionDic[typeName]+',0,0,'+str(self.x)+','+str(self.y)

    def reviveMsg(self):
        global actionDic
        return self.TypeID+','+str(self.ID)+','+self.statu+','+actionDic['SPRITEREVIVE']+',0,0,'+str(self.x)+','+str(self.y)

    def dieMsg(self):
        global actionDic
        return self.TypeID+','+str(self.ID)+','+self.statu+','+actionDic['DIE']+',0,0,'+str(self.x)+','+str(self.y)

    def die(self):
        global g_roleDB
        for v in g_roleDB.spriteDic.values():                        
            v.conn.translator.sendString(v.conn, self.dieMsg())
        self.dieTime = time.time()
        g_deadAnimalList.append(self)

    def revive(self):
        global g_roleDB
        self.HP =5
        self.x = random.randint(0,640/32)*32
        self.y = random.randint(0,480/24)*24 
        
        for v in g_roleDB.spriteDic.values():
            v.conn.translator.sendString(v.conn, self.reviveMsg())

    def printInfo(self):
        print self.TypeID, self.ID, self.statu
        

def LoadGame():
    global g_animalDB
    global OBJ_TYPE
    for x in xrange(6):
        animal = Animal(OBJ_TYPE['wolf'])
        animal.inUse = True
        animal.x = random.randint(0,640/32)*32
        animal.y = random.randint(0,480/24)*24 
        g_animalDB.addSprite(animal)

         
def main():

    connectM = ConnectionManager()
    listenM = ListeningManager(connectM)
    LoadGame()     

    listenM.addPort(5099)
    while(1):
        listenM.listen()
        connectM.manage()
        time.sleep(0.005)

if __name__ == "__main__":
    main()
