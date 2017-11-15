from numpy import log10, cos, sin, pi, absolute, arange
from scipy.signal import kaiserord, firwin, freqz
from matplotlib.pyplot import *

print "Usage: python util_fir <pass_freq> <stop_freq> <ripple> <attenuation>"
print "       <pass_freq> is the passband frequency relative to Fs (0 <= freq < 0.5)"
print "       <stop_freq> is the stopband frequency relative to Fs (0 <= freq < 0.5)"
print "       <ripple> is the maximum passband ripple"
print "       <attenuation> is the stopband attenuation"

if len(sys.argv) < 4: exit(0)

passband_freq   = float( sys.argv[1] )
stopband_freq   = float( sys.argv[2] )
passband_ripple = float( sys.argv[3] )
stopband_atten  = float( sys.argv[4] )

width = abs(passband_freq - stopband_freq) / 0.5
(tap_count,beta) = kaiserord( ripple=stopband_atten, width=width )

taps = firwin( numtaps=tap_count, cutoff  = ((passband_freq+stopband_freq)/2)/0.5, \
               window  = ('kaiser', beta) )

figure(1)

plot(taps, 'bo-', linewidth=1)
title('Filter Coefficients (%d taps)' % tap_count)
grid(True)

figure(2)

clf()
ww, hh = freqz( taps, worN=8000 )
hh = 20 * log10(abs(hh))

plot( (ww/pi)*0.5, hh, linewidth=1 )
xlabel( 'Frequency (Hz)' )
ylabel( 'Gain' )
title( 'Frequency Response' )
ylim( -120.0, +3.0 )
grid(True)

show()

for cc in taps[0:len(taps)-1]: sys.stdout.write( "FQ(%+1.9f)," % cc )
sys.stdout.write( "FQ(%+1.9f)\n" % taps[len(taps)-1] )
