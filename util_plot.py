import sys, struct, random

if len(sys.argv) < 3:

    print ""
    print "Usage: python plot.py <datafile> time"
    print "       python plot.py <datafile> time [beg end]"
    print "       python plot.py <datafile> freq lin"
    print "       python plot.py <datafile> freq log"
    print ""
    print "Where: <datafile> Contains one sample value per line.  Each sample is an"
    print "                  ASCII/HEX value (e.g. FFFF0001) representing a fixed-"
    print "                  point sample value."
    print "       time       Indicates that a time-domain plot should be shown"
    print "       freq       Indicates that a frequency-domain plot should be shown"
    print "       [beg end]  Optional; specifies the first and last sample in order to"
    print "                  create a sub-set of data to be plotted"
    print ""
    print "Examle: Create time-domain plot data in \'out.txt\' showing samples 100"
    print "        through 300 ... bash$ python plot.py out.txt 100 300"
    print ""
    print "Examle: Create frequency-domain plot data in \'out.txt\' showing the Y-axis"
    print "        with a logarithmic scale ... bash$ python plot.py out.txt freq log"
    print ""

    exit(0)

import numpy
import matplotlib.pyplot

numpy.seterr( all='ignore' )

K = 1
yy = [[0] * 65536, [0] * 65536, [0] * 65536]
mm = [[0] * 65536, [0] * 65536, [0] * 65536]
N = 0

file = open( sys.argv[1], "rt" )
while True:
    ll = file.readline();
    ll = ll.replace( "\n", "" );
    if len(ll) < 1: break
    ll = ll.split()
    K = len(ll)
    for ii in range(0,K):
        #xx = int(ll[ii],16) * 2
        #tt = xx
        #if xx & 0x80000000 == 0x80000000:
        #    xx = -(1 - float((xx & 0x7FFFFFFF)) / (2**31))
        #else:
        #    xx = float((xx & 0x7FFFFFFF)) / ((2**31)-1)
        #yy[ii][N] = xx
        yy[ii][N] = float(ll[ii])
    N += 1
    if N == 65536: break
file.close()

xx = [0] * N
ff = [0] * N
mm[0] = [0] * N
mm[1] = [0] * N
mm[2] = [0] * N
yy[0] = yy[0][0:N]
yy[1] = yy[1][0:N]
yy[2] = yy[2][0:N]

for ii in range( 0, N ):
    ff[ii] = ii
    xx[ii] = float(ii) / N

matplotlib.pyplot.grid( b=True, which='both', color='0.65',linestyle='-' )

if sys.argv[2] == "time":

    beg = 0; end = N
    if len(sys.argv) > 3: beg = int( sys.argv[3] )
    if len(sys.argv) > 4: end = int( sys.argv[4] )
    ff = ff[beg:end]

    matplotlib.pyplot.title( "Time Domain" )
    matplotlib.pyplot.xlabel( "Sample Number" )
    matplotlib.pyplot.ylabel( "Sample Amplitude" )
    matplotlib.pyplot.xticks( numpy.arange(min(ff), max(ff), (max(ff)-min(ff))/4 ))

    ymin = 0; ymax = 0

    for ii in range(0,K):
        yy[ii] = yy[ii][beg:end]
        ymin = min(ymin,min(yy[ii]))
        ymax = max(ymax,max(yy[ii]))
        matplotlib.pyplot.plot( ff, yy[ii] )

    matplotlib.pyplot.ylim( ymin, ymax )
    matplotlib.pyplot.yticks( numpy.arange(ymin, ymax, (ymax-ymin)/20 ))
    matplotlib.pyplot.show()

if sys.argv[2] == "freq":

    fs = 1.0
    if len(sys.argv) > 4: fs = float( sys.argv[4] )

    ff = numpy.array(ff[0:N/2+1]) / float(N/2)
    ff *= fs / 2.0

    for ii in range(0,K):
        mm[ii] = numpy.fft.rfft(yy[ii])
        mm[ii] = mm[ii][0:len(mm[ii])-0]
        #mm[ii] = numpy.abs(mm[ii]) * 2.0 / N
        mm[ii] = numpy.abs(mm[ii]) * 1.0 / N
        mm[ii] /= numpy.max(mm[ii])
        mm[ii] = mm[ii][0:N/2+1]

    matplotlib.pyplot.title( "Frequency Domain" )
    matplotlib.pyplot.xlabel( "Frequency (Normalized to Fs)" )
    matplotlib.pyplot.ylabel( "FFT Amplitude (dBfs)" )
    matplotlib.pyplot.ylim( -120, 0 )

    for ii in range(0,K):
        mm[ii] = 20.0 * numpy.log( mm[ii] )
        matplotlib.pyplot.plot( ff, mm[ii] )

    matplotlib.pyplot.xticks( numpy.arange(min(ff), max(ff), (max(ff)-min(ff))/10 ))
    matplotlib.pyplot.yticks( numpy.arange(-120, 0, (20+130)/15 ))
    matplotlib.pyplot.show()

from numpy import argmax, sqrt, mean, absolute, arange, log10
from scipy.signal import blackmanharris
from numpy.fft import rfft, irfft

def find_range(f, x):
    lowermin = 0; uppermin = 0
    for i in arange(x+1, len(f)):
        if f[i+1] >= f[i]:
            uppermin = i
            break
    for i in arange(x-1, 0, -1):
        if f[i] <= f[i-1]:
            lowermin = i + 1
            break
    return (lowermin, uppermin)

for signal in yy:

    if K == 0: break
    K -= 1

    # Get rid of DC and window the signal
    signal -= mean(signal) # TODO: Do this in the frequency domain, and take any skirts with it?
    windowed = signal * blackmanharris(len(signal))  # TODO Kaiser?

    # Measure the total signal before filtering but after windowing
    total_rms = sqrt(mean(absolute(windowed)**2))

    # Find the peak of the frequency spectrum (fundamental frequency), and
    # filter the signal by throwing away values between the nearest local minima
    f = rfft(windowed); i = argmax(abs(f))
    lowermin, uppermin = find_range(abs(f), i)
    f[lowermin: uppermin] = 0

    # Transform noise back into the signal domain and measure it
    # TODO: Could probably calculate the RMS directly in the frequency domain instead
    noise = irfft(f)
    rms_flat = sqrt(mean(absolute(noise)**2))
    THDN = rms_flat / total_rms
    print "DynRange = %06.1fdB, SNR = %05.1fdB, THD+N = %04.1f%%" % \
           (20 * log10(total_rms), \
           20 * log10(total_rms) - 20 * log10(rms_flat), \
           THDN * 100 )
