import socket,select
import time

actionDic={'SPRITEINITROLE':'1',
            'SPRITEINITPLAYER':'2',
           'SPRITEDESTORYPLAYER':'3',
           'SPRITEREVIVE':'4',
            'STAND':'0',	
            'WALK':'8',
            'ATTACK':'16',
           'DIE':'24',}

class RoleDB:
    
    def __init__(self):
        self.roleDic={}
        self.maxID = 0

    def genID(self):
        self.maxID+=1
        return self.maxID

    def removeRole(self,ID):
        global g_roleDB
        if ID in g_roleDB.roleDic:
            del g_roleDB.roleDic[ID]

    def findUserByName(self, name):
        for v in self.roleDic.values():
            if name == v.name:
                return v
        return None
    
g_roleDB = RoleDB()

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
        try:            
            username, password = data.split(',')
            if g_roleDB.findUserByName(username) is not None:
                print 'user is online'
                connection.translator.sendString(connection, 'user is online')
            else:
                role = Role(connection, username)
                connection.roleID = role.roleID
                g_roleDB.roleDic[role.roleID] = role
                connection.translator.sendString(connection, role.pos('SPRITEINITROLE'))
                connection.removeHandler()

                self.sendOthers(connection, role.pos('SPRITEINITPLAYER'))

                for role in g_roleDB.roleDic.values():
                    if role.roleID != connection.roleID:
                        connection.translator.sendString(connection, role.pos('SPRITEINITPLAYER'))
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
        for role in g_roleDB.roleDic.values():
            if role.roleID != conn.roleID:
                role.conn.translator.sendString(role.conn, data)
    
class HandlerLogic:
    def handle(self, connection,data):
        global g_roleDB
        msgList=data.split(',')

        print 'HandlerLogic',msgList
        for v in g_roleDB.roleDic.values():
            v.conn.translator.sendString(v.conn, data)

        if msgList[1] == actionDic['ATTACK']:
            g_roleDB.roleDic[int(msgList[2])].HP-=1
            if g_roleDB.roleDic[int(msgList[2])].HP<=0:
                g_roleDB.roleDic[int(msgList[2])].conn.addHandler(HandlerDie())           
        else:
            g_roleDB.roleDic[connection.roleID].x, g_roleDB.roleDic[connection.roleID].y = msgList[3:5]
            
        print connection.roleID,'at',g_roleDB.roleDic[connection.roleID].x, g_roleDB.roleDic[connection.roleID].y

    def enter(self, connection):
        print 'HandlerLogic enter'
        pass
        
    def hungup(self,connection):
        global g_roleDB

        self.sendOthers(connection, str(connection.roleID)+','+actionDic['SPRITEDESTORYPLAYER']+',0,0,0')
        g_roleDB.removeRole(connection.roleID)
        
    def sendOthers(self, conn, data):
        global g_roleDB
        for role in g_roleDB.roleDic.values():
            if role.roleID != conn.roleID:
                role.conn.translator.sendString(role.conn, data)
                
    def leave(self,connection):
        print 'HandlerLogic leave'
        pass
class HandlerDie:
    def handle(self, connection,data):
        global g_roleDB
        global actionDic
        msgList=data.split(',')

        if msgList[1] == actionDic['SPRITEREVIVE']:        
            for v in g_roleDB.roleDic.values():
                v.conn.translator.sendString(v.conn, data)

            g_roleDB.roleDic[connection.roleID].HP=3
            g_roleDB.roleDic[connection.roleID].x=msgList[3]
            g_roleDB.roleDic[connection.roleID].y=msgList[4]
            
            connection.removeHandler()
        
        pass
    def enter(self, connection):
        global g_roleDB
        print 'HandlerDie enter'
        for v in g_roleDB.roleDic.values():
                    v.conn.translator.sendString(v.conn, g_roleDB.roleDic[connection.roleID].die())

    def hungup(self,connection):
        global g_roleDB

        self.sendOthers(connection, str(connection.roleID)+','+actionDic['SPRITEDESTORYPLAYER']+',0,0,0')
        g_roleDB.removeRole(connection.roleID)
          
    def leave(self,connection):
        print 'HandlerDie leave'
        pass

    def sendOthers(self, conn, data):
        global g_roleDB
        for role in g_roleDB.roleDic.values():
            if role.roleID != conn.roleID:
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
    def __init__(self, connection, name):
        global g_roleDB
        self.roleID  = g_roleDB.genID()
        self.name=name
        self.HP=3
        self.x = 320
        self.y = 240
        self.conn = connection
 
    def pos(self, typeName):
        global actionDic
        return str(self.roleID)+','+actionDic[typeName]+',0,'+str(self.x)+','+str(self.y)

    def die(self):
        global actionDic
        return str(self.roleID)+','+actionDic['DIE']+',0,'+str(self.x)+','+str(self.y)
    
    def rebirth(self):
        pass
        
def main():

    connectM = ConnectionManager()
    listenM = ListeningManager(connectM)

    listenM.addPort(5099)
    while(1):
        listenM.listen()
        connectM.manage()
        time.sleep(0.005)

if __name__ == "__main__":
    main()
