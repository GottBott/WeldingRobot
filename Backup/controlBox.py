import socket
import serial
import time
from Tkinter import *


# conect to robot at the givin adress 
TCP_IP = '192.168.0.180'
TCP_PORT = 5001
BUFFER_SIZE = 1024
MESSAGE =""
try:
    ser = serial.Serial('/dev/ttyACM0',115200)
except:
    pass
s =''

root = Tk()
root.title("Positioning Data")
root.geometry("1260x375+0+425")
master = root

x=0;
z=0;
o=0;
d=0;
q=0;

XPOS = StringVar()
ZPOS = StringVar()
HeightRead = StringVar()
OSCFreq = StringVar()
OSCDist = StringVar()

# lable for box output
w = Label(master, text="Controller Output",font=("Helvetica", 24))
w.place(x = 900, y =25)

# make a text box to put the serial output
log = Text ( root, width=200, height=30, font=("Helvetica", 18), takefocus=0)
log.pack()
log.place(x = 900, y =75)


# XPOS
w = Label(master, text="X-Position",font=("Helvetica", 24))
w.place(x = 50, y =50)
w = Label(root, textvariable = XPOS, relief=RAISED,font=("Helvetica", 24))
w.place(x = 50, y =100)

# ZPOS
w = Label(master, text="Z-Position",font=("Helvetica", 24))
w.place(x = 50, y =200)
w = Label(root, textvariable = ZPOS, relief=RAISED,font=("Helvetica", 24))
w.place(x = 50, y =250)

# HeightRead
w = Label(master, text="Height Reading",font=("Helvetica", 24))
w.place(x = 250, y =100)
w = Label(root, textvariable = HeightRead, relief=RAISED,font=("Helvetica", 24))
w.place(x = 250, y =150)

# OSCFreq
w = Label(master, text="Osilation Frequency",font=("Helvetica", 24))
w.place(x = 500, y =50)
w = Label(root, textvariable = OSCFreq, relief=RAISED,font=("Helvetica", 24))
w.place(x = 500, y =100)

# OSCDist
w = Label(master, text="Oscilation Distance",font=("Helvetica", 24))
w.place(x = 500, y =200)
w = Label(root, textvariable = OSCDist, relief=RAISED,font=("Helvetica", 24))
w.place(x = 500, y =250)

def connectSocket():
    global q
    log.insert('0.0',"Atempting To Connect \n\n")
    try:
        global s
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP,TCP_PORT))
        log.insert('0.0',"Connection Successful! \n\n")
        readSocket()

    except:
        s.close()
        log.insert('0.0', "Connection Failed! "+str(q)+" \n\n")
        time.sleep(1)
        q = q +1
        root.after(1000,connectSocket)

def readSocket():
    try:
        MESSAGE = ser.readline()
        if("G" in MESSAGE or "H" in MESSAGE):
            VALUE = ser.readline()
            s.send(MESSAGE)
            s.send(VALUE)
            updateValues(MESSAGE,VALUE)
        else:
            s.send(MESSAGE)
            updateValues(MESSAGE,0)
       # log.insert('0.0', str(MESSAGE))
        root.after(10,readSocket)

    except:
        log.insert('0.0',"No Connection Established \n\n")
        connectSocket()

def updateValues(m,n):

    global x
    global z
    global o
    global d

    if("W" in m):
        z += 1;
        log.insert('0.0',"UP \n\n")

    if("S" in m):
        z -= 1;
        log.insert('0.0',"DOWN \n\n")

    if("A" in m):
        x -= 1;
        log.insert('0.0',"LEFT \n\n")

    if("D" in m):
        x += 1;
        log.insert('0.0',"RIGHT \n\n")

    if("G" in m):
        #s = str.split("G");
       # o = int(s[1]%100);
        o =10-((int(n))/100);
        log.insert('0.0',"OSCFREQ \n\n")

    if("H" in m):
   #     s = str.split('G');
    #    d = int(s[1]%100);
        d = 10-((int(n))/100);
        log.insert('0.0',"OSCDIST \n\n")

    XPOS.set(x)
    ZPOS.set(z)
    OSCFreq.set(o)
    OSCDist.set(d)

root.after(10,readSocket)
root.mainloop()


