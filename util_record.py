import sys
import pyaudio
from array import array

pa = pyaudio.PyAudio()

info = pa.get_host_api_info_by_index(0)
count = info.get('deviceCount')
#print count
device = pa.get_device_info_by_index(0)
#print device["name"]

stream = pa.open( format=pyaudio.paInt32, channels=1, rate=48000,
                  input=True, output=False, frames_per_buffer=1024,
                  input_device_index = 0, output_device_index = 0 )

stream.read( 65536 )
data = array( 'i', stream.read(65536))

stream.stop_stream()
stream.close()
pa.terminate()

for xx in data:
    xx = float(xx) / (2**31)
    print "%+01.9f" % xx

