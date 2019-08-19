import sys
#sys.path.append("/usr/local/lib/python3.6/site-packages/skyfield")
#print(sys.path)


#from skyfield.api import Topos, load


import time
import random

#RPi Pinouts

#I2C Pins
#GPIO2 -> SDA
#GPIO3 -> SCL

#Import the Library Required
import smbus
import time
#import starpointer


# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
#Slave Address 4
address = 0x04

mystr1 = "\0" * 100
mystr2 = "\0" * 100

data_list1=list()
data_list2=list()

def writeNumber(value):
    bus.write_byte(address, value)
    #bus.write_block_data(address, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    return number





while (1==1):


    mystr1 = str(90.0)
    mystr2 = str(180.0)


    #mystr1 = str(random.randint(0,3600)/10)
    #mystr2 = str(random.randint(0,3600)/10)

    for i in range(len(mystr1)):
        data_list1.append(ord(mystr1[i]))

    for i in range(len(mystr2)):
        data_list2.append(ord(mystr2[i]))


    bus.write_block_data(address,ord("X"),data_list1)
    print('I sent E data')
    print(ord("E"),data_list1)
    time.sleep(0.1)    #Wait for the data_list
    bus.write_block_data(address,ord("X"),data_list2)
    print('I sent A data')
    print(ord("A"),data_list2)
    time.sleep(0.1)
    data_list1=[]
    data_list2=[]
    time.sleep(1)


#print("RA/DEC:")
#print(ra)
#print(dec)
#ra, dec, distance = topocentric.radec(epoch='date')

#print(ra)
#print(dec)
