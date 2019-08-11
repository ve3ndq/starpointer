#RPi Pinouts

#I2C Pins 
#GPIO2 -> SDA
#GPIO3 -> SCL

#Import the Library Required 
import smbus
import time

# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
#Slave Address 4
address = 0x04

data_list=list()

def writeNumber(value):
    bus.write_byte(address, value)
    #bus.write_block_data(address, 0, value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    return number

while True:
	#Receives the data from the User
	data = raw_input("Enter the data to be sent : ")

	for i in range(65,67):
		data_list.append(i)
	
	data_list = []
	mystr = data
	
	for i in range(len(mystr)):
		data_list.append(ord(mystr[i]))
		
	bus.write_block_data(address,ord("E"),data_list)
	time.sleep(0.1)
	data_list=[]


	#    for i in data_list:
    #Sends to the Slaves
#   	writeNumber(int(ord(i)))
#		writeNumber(49)

#		time.sleep(.01)

#Write a new line
#	writeNumber(int(0x0A))
#End of the Script

