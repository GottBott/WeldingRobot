import socket
import serial
import time
from Tkinter import *

# conect to remote control sudo
TCP_IP = 'localhost'
TCP_PORT = 5001
BUFFER_SIZE = 1024
MESSAGE =""
s =''

root = Tk()
root.title("Positioning Data")
root.geometry("1260x375+0+425")
master = root

x=0;
z=0;
o=0;
d=0;


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
    log.insert('0.0',"Atempting To Connect \n\n")
    try:
        global s
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP,TCP_PORT))
        log.insert('0.0',"Connection Successful! \n\n")
        readSocket()

    except:
        s.close()
        log.insert('0.0', "Connection Failed! \n\n")
        root.after(1000,connectSocket)


def readSocket():
    try:
        MESSAGE = input('Choose a number: ')
        s.send(MESSAGE)
        updateValues(MESSAGE)
        log.insert('0.0', str(MESSAGE) + "\n")
        root.after(10,readSocket)
        
    except:
        log.insert('0.0',"No Connection Established \n\n")
        connectSocket()
        
def updateValues(m):

    global x
    global z
    
    if("W" in m):
        z += 1;

    if( m == "S"):
        z -= 1;
        
    if( m == "A"):
        x -= 1;
        
    if( m == "D"):
        x += 1;

    XPOS.set(x)
    ZPOS.set(z)


root.after(10,readSocket)
root.mainloop()
