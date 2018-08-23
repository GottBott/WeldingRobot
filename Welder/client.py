import socket
import serial
import time

# conect to remote control sudo
TCP_IP = 'localhost'
TCP_PORT = 5005
BUFFER_SIZE = 1024
MESSAGE =""
while 1:
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP,TCP_PORT))
        while 1:
            MESSAGE = input('Choose a number: ')
            s.send(MESSAGE)
            time.sleep(0.001)
    except:
        s.close()
        print "Could Not Create Conection"
        time.sleep(1)
    
