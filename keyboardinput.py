import sys
#sys.path.append("/usr/local/lib/python3.6/site-packages/skyfield")
#print(sys.path)
from skyfield.api import Topos, load
import time
import shlex, subprocess
import os

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

def debugprint(tempval):
    os.system('clear')
    print('SATELLITE:')
    print(satellite)
    print('{:.3f} days away from epoch'.format(days))
    print("EL(alt):")
    print(alt)
    print(alt.degrees)
    print('Az:')
    print(az)
    print(az.degrees)
    print('Geocentric:')
    print('Latitude:', subpoint.latitude)
    print('Longitude:', subpoint.longitude)
    print('Elevation (m):', int(subpoint.elevation.m))


stations_url = 'http://celestrak.com/NORAD/elements/stations.txt'
stations_url = 'http://celestrak.com/NORAD/elements/visual.txt'
satellites = load.tle(stations_url)
satellite = satellites['ISS (ZARYA)']
#satellite = satellites['NOAA 18']

print('-------------------SATELLITE:')
print(satellite)

#print('EPOCH')
#print(satellite.epoch.utc_jpl())

quit=0
while (quit==0):




    print("E-Elevation Target")
    print("A-Azimuth Target")
    print("Z-Calibrate")
    print("X-Servos ON/OFF")
    print("J-Jog El Steps")
    print("K-Jog Az Steps")
    print("I-GOTO ISS")
    print("Q-Quit")
    inputcommand=input("Command:")

    if (inputcommand == 'Q'):
        print("Quit")
        quit=1

    if (inputcommand == 'E'):
        print("E was input")
        value=(input("Elevation in Degrees:"))
        for i in range(len(value)):
            data_list1.append(ord(mystr1[i]))
        try:
            bus.write_block_data(address,ord("E"),data_list1)
        except IOError:
            subprocess.call(['i2cdetect', '-y', '1'])
            flag = 1     #optional flag to signal your code to resend or something















    if (inputcommand == 'I'):
        ts = load.timescale()
        #t = ts.utc(2014, 1, 23, 11, 18, 7)
        t=ts.now()

        days = t - satellite.epoch
        if abs(days) > 7:
            satellites = load.tle(stations_url, reload=True)
            satellite = satellites['ISS (ZARYA)']

        geocentric = satellite.at(t)


        burlington = Topos('43.328674 N', '79.817734 W')

        difference = satellite - burlington
        topocentric = difference.at(t)


        alt, az, distance = topocentric.altaz()

        ra, dec, distance = topocentric.radec()  # ICRF ("J2000")
        subpoint = geocentric.subpoint()

        mystr1 = str(alt.degrees)
        mystr2 = str(az.degrees)



        for i in range(len(mystr1)):
            data_list1.append(ord(mystr1[i]))

        for i in range(len(mystr2)):
            data_list2.append(ord(mystr2[i]))


        try:
            bus.write_block_data(address,ord("E"),data_list1)
        except IOError:
            subprocess.call(['i2cdetect', '-y', '1'])
            flag = 1     #optional flag to signal your code to resend or something


        time.sleep(0.3)    #Wait for the data_list

        try:
            bus.write_block_data(address,ord("A"),data_list2)
        except IOError:
            subprocess.call(['i2cdetect', '-y', '1'])
            flag = 1     #optional flag to signal your code to resend or something
        time.sleep(0.3)
        data_list1=[]
        data_list2=[]

        debugprint(1)
        time.sleep(0.1)

#End of program
