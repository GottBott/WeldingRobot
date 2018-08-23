
import socket
import serial
import time

TCP_IP = ''
TCP_PORT = 5001
BUFFER_SIZE = 512  # Normally 1024, but we want fast response

try:
    ser = serial.Serial('/dev/ttyACM0',115200)
except:
    pass

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print 'Connection address:', addr
while 1:
    data = conn.recv(BUFFER_SIZE)
    if not data: break
   # print "received data:", data
    try:
        ser.write(data)
    except:
        pass
   # time.sleep(.01)
conn.close()


