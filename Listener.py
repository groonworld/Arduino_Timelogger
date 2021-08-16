#------------------------
# TODO:
# 1. Basic logic: write Serial listener, print states to cosole
# 2. Calculations of date and time
# 3. Logging results to excel file
# 4. Daily targets and arduino-LED-signalling
# 5. implement other excel stuff (monthly report, target meeting quota, etc)
#------------------------

import serial, time, sys
from datetime import datetime

arduino = serial.Serial(port = 'COM5', baudrate = 9600, timeout = 0.1)
lastLine = ''


#------------------------
# Functions
#------------------------

def read():
    data = arduino.readline().decode('utf-8').rstrip()
    return data

def login(lane):
    print('logging into state ' + str(lane))

def logout(lane):
    print('logging out of state ' + str(lane))
 
def action(num):
    if (1 <= num <= 4):
        login(num)
    elif (5 <= num <= 8):
        logout(num - 4)
    else:
        print('WTF, this can\'t be happening!')
        time.sleep(.7)
        sys.exit()

#------------------------
# MAIN
#------------------------

while True:
    currentLine = read()
    if (currentLine != lastLine):
        action(int(currentLine))
    else:
        time.sleep(.1)  
    
