import socket
import time

ser=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
ser.bind(('',5099))
ser.listen(5)

connection,address=ser.accept()
print 'get connected from',address

State="Connected"

while True:    
    try:
        time.sleep(0.01)
        buf=connection.recv(512)
        print buf 
        connection.send(buf)
    except:
        connection,address=ser.accept()
        print 'get connected from',address
