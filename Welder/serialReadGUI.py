from serial import *
from Tkinter import *

serialPort = "/dev/tty.usbmodem1421"
baudRate = 9600
ser = Serial(serialPort , baudRate, timeout=0, writeTimeout=0) #ensure non-blocking

#make a TkInter Window
root = Tk()
root.title("Positioning Display")
root.geometry("1260x400+0+400")
master = root

XPOS = StringVar()


w = Label(master, text="X-Position",font=("Helvetica", 24))
w.place(x = 50, y =50)

w = Label(root, textvariable = XPOS, relief=RAISED,font=("Helvetica", 24))
w.place(x = 50, y =100)


w = Label(master, text="Z-Position",font=("Helvetica", 24))
w.place(x = 50, y =200)

w = Label(master, text="Height Reading",font=("Helvetica", 24))
w.place(x = 500, y =50)


w = Label(master, text="Osilation Frequency",font=("Helvetica", 24))
w.place(x = 900, y =50)

w = Label(master, text="Oscilation Distance",font=("Helvetica", 24))
w.place(x = 900, y =200)



#make our own buffer
#useful for parsing commands
#Serial.readline seems unreliable at times too
serBuffer = ""

def readSerial():
    while True:
        c = ser.read() # attempt to read a character from Serial
        
        #was anything read?
        if len(c) == 0:
            break
        
        # get the buffer from outside of this function
        global serBuffer
        
        # check if character is a delimeter
        if c == '\r':
            c = '' # don't want returns. chuck it
            
        if c == '\n':
            serBuffer += "\n" # add the newline to the buffer
            XPOS.set(serBuffer)
            serBuffer = "" # empty the buffer

        else:
            serBuffer += c # add to the buffer
           
    root.after(10, readSerial) # check serial again soon


# after initializing serial, an arduino may need a bit of time to reset
root.after(100, readSerial)

root.mainloop()




