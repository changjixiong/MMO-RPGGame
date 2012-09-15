import socket,select
import time
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
        #conn.SetBlocking( false )
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

    def close(self):
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()
        self.closed = True
 
class HandlerLogon:#public ConnectionHandler
    def handle(self, connection, data):
        #add user
        print data.split(',')[0],'come'
        connection.removeHandler()
        connection.addHandler(HandlerLogic())
    def enter(self, connection):
        connection.translator.sendString(connection, '')
    def leave(self,connection):
        pass
    
class HandlerLogic:
    def handle(self, connection,data):
        #deal
        msg = '['+data+']'
        print msg
        connection.translator.sendString(connection, msg)
    
    def enter(self, connection):
        pass
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
        conn.bufferData(data)

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
