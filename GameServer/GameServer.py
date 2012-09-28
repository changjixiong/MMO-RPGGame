import socket,select
import time

actionDic={'SPRITEINITROLE':'1',
            'SPRITEINITPLAYER':'2',
           'SPRITEDESTORYPLAYER':'3',
            'STAND':'0',	
            'WALK':'8',}

class ListeningManager:
    def __init__(self,connectionManager):
        self.cm=connectionManager
        self.listenList=[]

    def addPort(self, port):
        listensock=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        listensock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        listensock.bind(('',port))
        listensock.listen(8)
        self.listenList.append(listensock)

    def listen(self):
        comeList,wlist,elist=select.select(self.listenList,[],[],0.01)
        for sock in comeList:
            clientsock,clientaddr = sock.accept()
            clientsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)            
            self.cm.newConnection(clientsock)

class ConnectionManager:
    def __init__(self):
        self.connectionList=[]
        self.datasockList=[]
        pass
    
    def newConnection(self, clientsock):
        print 'a new connection come'
        self.datasockList.append(clientsock)
        conn = Connection(clientsock)
        self.connectionList.append(conn)
        conn.addHandler(HandlerLogon())
        
    def manage(self):        
        self.listen()
        self.send()
        self.closeConnections()

    def close(self, connection):
        self.datasockList.remove(connection.sock)
        self.connectionList.remove(connection)

    def listen(self):
        if len(self.connectionList)==0:
            return
        comeList,wlist,elist=select.select(self.datasockList,[],[],0.01)
        for conn in self.connectionList:
            if conn.sock in comeList:
                conn.receive()                    
                        
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
        self.role = None

    def initRole(self, roleName):
        self.role = Role(self, roleName)

    def handler(self):
        if len(self.handlerList)==0:
            return 0
        else:
            return self.handlerList[-1]

    def addHandler(self, handler):
        self.handlerList.append(handler)
        handler.enter(self)
        
    def bufferData(self, data):
        self.databuffer+=data
        
    def sendBuffer(self):
        if self.closed == False:            
            len=self.sock.send(self.databuffer)
            if self.role is not None and len>0:
                print self.role.id, 'send',self.databuffer[0:len]
            self.databuffer=self.databuffer[len:]
        
    def receive(self):
        try:
            data = self.sock.recv(512)
            if len(data)>0:
                self.translator.translate(self, data)
            else:
                self.close()
        except Exception,x:
            print x,
            self.close()
            if type(x) is socket.error:
                print socket.errorTab[x[0]]

    def removeHandler(self):
        self.handler().leave(self)
        self.handlerList.pop()
        if len(self.handlerList)>0:
            self.handler().enter(self)

    def sendOthers(self, data):
        for item in self.role.roleDB.roleDic.values():
            if item[0] is not None and item[0] is not self:
                item[0].translator.sendString(item[0], data)

    def close(self):
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()
        self.closed = True

        if self.role is not None:
            self.sendOthers(str(self.role.id)+','+actionDic['SPRITEDESTORYPLAYER']+',0,0') 
            self.role.roleDB.removeRole(self.role)
            self.role=None
 
class HandlerLogon:
    def handle(self, connection, data):
        try:
            datalist = data.split(',')
            if datalist[0] not in Role.roleDB.roleDic:
                Role.roleDB.roleDic[datalist[0]]=[None,32*5,24*5]

            if Role.roleDB.roleDic[datalist[0]][0] is not None:
                print 'user is online'
                connection.translator.sendString(connection, 'user is online')
            else:                
                connection.initRole(data.split(',')[0])
                connection.translator.sendString(connection, connection.role.pos('SPRITEINITROLE'))
                connection.removeHandler()
                connection.addHandler(HandlerLogic())
        except Exception,x:
            print x

    def enter(self, connection):
        connection.translator.sendString(connection, 'username,password')
    def leave(self,connection):
        pass
    
class HandlerLogic:
    def handle(self, connection,data):
        #deal
        msg = data
        msgList=msg.split(',')
        for item in connection.role.roleDB.roleDic.values():
            if item[0] is not None:
                item[0].translator.sendString(item[0], msg)
               
        connection.role.x, connection.role.y = msgList[2:4]
        print connection.role.id,'at',connection.role.x, connection.role.y

    
    def enter(self, connection):
        for item in connection.role.roleDB.roleDic.values():
            #print item[0].role.pos('SPRITEINITPLAYER')
            if item[0] is not None and item[0] is not connection:
                item[0].translator.sendString(item[0], connection.role.pos('SPRITEINITPLAYER'))
                connection.translator.sendString(connection, item[0].role.pos('SPRITEINITPLAYER'))
        
                
    def leave(self,connection):
        pass
    
class Translator:
    databuffer = ''
    def __init__(self):
        pass
    def translate(self, conn, data):
        #get one message
        msg = data[1:-1]
        #databuffer+=remain
        conn.handler().handle(conn, msg)
    def sendString(self, conn, data):
        conn.bufferData('['+data+']')

class RoleDB:
    roleDic={}
    def __init__(self):
        # these three users are for test
        # roleDic will be inited by user data file
        #RoleDB.roleDic['role1']=[None,32*2,24*2]
        pass
    def removeRole(self,role):
        RoleDB.roleDic[role.name][0]=None
        
class Role:
    roleDB = RoleDB()
    roleID = 0
    def __init__(self, connection, name):
        Role.roleID+=1
        Role.roleDB.roleDic[name][0]=connection

        self.id  = Role.roleID
        self.name=name        
        self.x = self.roleDB.roleDic[self.name][1]
        self.y = self.roleDB.roleDic[self.name][2]
 
    def pos(self, typeName):
        global actionDic
        return str(self.id)+','+actionDic[typeName]+','+str(self.x)+','+str(self.y)
        
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
