import sys
#sys.path.append("/usr/local/lib/python3.6/site-packages/skyfield")
print(sys.path)
from skyfield.api import Topos, load
stations_url = 'http://celestrak.com/NORAD/elements/stations.txt'
satellites = load.tle(stations_url)
satellite = satellites['ISS (ZARYA)']

print('-------------------SATELLITE:')
print(satellite)

print('EPOCH')
print(satellite.epoch.utc_jpl())


ts = load.timescale()
#t = ts.utc(2014, 1, 23, 11, 18, 7)
t=ts.now()

days = t - satellite.epoch
print('{:.3f} days away from epoch'.format(days))
#TESTING 123
if abs(days) > 7:
    satellites = load.tle(stations_url, reload=True)
    satellite = satellites['ISS (ZARYA)']

geocentric = satellite.at(t)
print('GEOCENTRIC POS:')
print(geocentric.position.km)

bluffton = Topos('40.8939 N', '83.8917 W')
burlington = Topos('43.328674 N', '79.817734 W')

difference = satellite - burlington
print('DIFFERENCE:')
print(difference)

topocentric = difference.at(t)
print('TOPOCENTRIC POSITION:')
print(topocentric.position.km)

alt, az, distance = topocentric.altaz()

if alt.degrees > 0:
    print('The ISS is above the horizon')
print("ALT/AZ/DIST:")
print(alt)
print(az)
print(distance.km)
ra, dec, distance = topocentric.radec()  # ICRF ("J2000")

subpoint = geocentric.subpoint()
print('Latitude:', subpoint.latitude)
print('Longitude:', subpoint.longitude)
print('Elevation (m):', int(subpoint.elevation.m))


#print("RA/DEC:")
#print(ra)
#print(dec)
#ra, dec, distance = topocentric.radec(epoch='date')

#print(ra)
#print(dec)
