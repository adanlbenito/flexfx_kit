import sys, struct, numpy

import numpy as np
import scipy.signal as dsp
import matplotlib.pyplot as plot

print "Usage 1: python design.py <type> <freq> <Q> <gain>"
print "         <type> filter type (notch, lowpass, highpass, allpass, bandpass,"
print "                             peaking, highshelf, or lowshelf"
print "         <freq> is cutoff frequency relative to Fs (0 <= freq < 0.5)"
print "         <Q> is the filter Q-factor"
print "         <gain> is the filter positive or negative gain in dB"

if len(sys.argv) < 4: exit(0)

np.seterr( all='ignore' )

type = sys.argv[1]
freq = float( sys.argv[2] )
Q    = float( sys.argv[3] )
gain = float( sys.argv[4] )

def plot_response( bb, aa, xmin=None, xmax=None, ymin=-60.0, ymax=6.0 ):

    global type

    fs = 0.5
    w,h = dsp.freqz(bb,aa)
    h_dB = 20 * np.log10 (abs(h))
    if xmin == None: xmin = min(fs*w/np.pi)
    if xmax == None: xmax = max(fs*w/np.pi)
    if ymin == None: ymin = min(h_dB)
    if ymax == None: ymax = max(h_dB)

    plot.subplot(211)
    xx = fs * w/max(w)
    plot.xlim( xmin, xmax )
    plot.ylim( ymin, ymax )
    plot.ylabel( 'Magnitude (db)' )
    plot.xlabel( 'Frequency (Fs = 1.0)' )
    plot.title( 'Frequency response' )
    plot.plot( fs*w/max(w), h_dB )
    plot.xticks( numpy.arange( min(xx), max(xx), (max(xx)-min(xx))/10 ))
    if type == "peaking" or type == "highshelf" or type == "lowshelf":
        plot.yticks( numpy.arange( ymin, ymax, 3 ))
    else:
        plot.yticks( numpy.arange( ymin, ymax, 6 ))
    plot.grid( b=True, which='both', color='0.65',linestyle='-' )

    plot.subplot(212)
    h_Phase = np.unwrap(np.arctan2(np.imag(h),np.real(h))) / np.pi * 180.0
    plot.xlim( xmin, xmax )
    plot.ylabel( 'Phase (degrees)' )
    plot.xlabel( 'Frequency (Fs = 1.0)' )
    plot.title( 'Phase response' )
    plot.subplots_adjust( hspace=0.5 )
    plot.plot( w/max(w), h_Phase)
    #plot.xticks( numpy.arange( min(xx), max(xx), (max(xx)-min(xx))/10 ))
    #plot.yticks( numpy.arange(-120, 0, 120/10 ))
    plot.grid( b=True, which='both', color='0.65',linestyle='-' )

    plot.show()

def _make_biquad_notch( filter_freq, q_factor ):

	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/(2.0 * q_factor)

	b0 = +1.0
	b1 = -2.0 * np.cos(w0)
	b2 = +1.0
	a0 = +1.0 + alpha
	a1 = -2.0 * np.cos(w0)
	a2 = +1.0 - alpha

	return (b0,b1,b2,a0,a1,a2)

def _make_biquad_lowpass( filter_freq, q_factor ):

	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/(2 * q_factor)

	b0 = (+1.0 - np.cos(w0)) / 2.0
	b1 =  +1.0 - np.cos(w0)
	b2 = (+1.0 - np.cos(w0)) / 2.0
	a0 = +1.0 + alpha
	a1 = -2.0 * np.cos(w0)
	a2 = +1.0 - alpha

	return (b0,b1,b2,a0,a1,a2)

def _make_biquad_highpass( filter_freq, q_factor ):

	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/(2 * q_factor)

	b0 =  (1.0 + np.cos(w0)) / 2.0
	b1 = -(1.0 + np.cos(w0))
	b2 =  (1.0 + np.cos(w0)) / 2.0
	a0 =  +1.0 + alpha
	a1 =  -2.0 * np.cos(w0)
	a2 =  +1.0 - alpha

	return (b0,b1,b2,a0,a1,a2)

def _make_biquad_allpass( filter_freq, q_factor ):

	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/(2.0 * q_factor)

	b0 =  +1.0 - alpha
	b1 =  -2.0 * np.cos(w0)
	b2 =  +1.0 + alpha
	a0 =  +1.0 + alpha
	a1 =  -2.0 * np.cos(w0)
	a2 =  +1.0 - alpha

	return (b0,b1,b2,a0,a1,a2)

# Constant 0 dB peak gain
# FIXME: Results in a peaking filter at freq1 rather than BP filter that's flat within the pass-band
def _make_biquad_bandpass( filter_freq, filter_freq2 ):

	w0 = 2.0 * np.pi * filter_freq
	BW = filter_freq2 - filter_freq
	alpha = np.sin(w0) * np.sinh( np.log(2)/2 * BW * w0/np.sin(w0) )

	b0 =  np.sin(w0) / 2.0
	b1 =  +0.0
	b2 = -np.sin(w0) / 2.0
	a0 =  +1.0 + alpha
	a1 =  -2.0 * np.cos(w0)
	a2 =  +1.0 - alpha

	return (b0,b1,b2,a0,a1,a2)

# gain can be + or -
def _make_biquad_peaking( filter_freq, q_factor, gain_db ):

	A  = np.sqrt( 10 ** (gain_db/20) )
	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/(2.0 * q_factor)

	b0 = +1.0 + alpha * A
	b1 = -2.0 * np.cos(w0)
	b2 = +1.0 - alpha * A
	a0 = +1.0 + alpha / A
	a1 = -2.0 * np.cos(w0)
	a2 = +1.0 - alpha / A

	return (b0,b1,b2,a0,a1,a2)

def _make_biquad_lowshelf( filter_freq, q_factor, gain_db ):

	S = q_factor
	A  = 10.0 ** (gain_db / 40.0)
	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/2 * np.sqrt( (A + 1/A)*(1/S - 1) + 2 )

	b0 =    A*( (A+1) - (A-1)*np.cos(w0) + 2*np.sqrt(A)*alpha )
	b1 =  2*A*( (A-1) - (A+1)*np.cos(w0)                   )
	b2 =    A*( (A+1) - (A-1)*np.cos(w0) - 2*np.sqrt(A)*alpha )
	a0 =        (A+1) + (A-1)*np.cos(w0) + 2*np.sqrt(A)*alpha
	a1 =   -2*( (A-1) + (A+1)*np.cos(w0)                   )
	a2 =        (A+1) + (A-1)*np.cos(w0) - 2*np.sqrt(A)*alpha

	return (b0,b1,b2,a0,a1,a2)

def _make_biquad_highshelf( filter_freq, q_factor, gain_db ):

	S = q_factor
	A  = 10.0 ** (gain_db / 40.0)
	w0 = 2.0 * np.pi * filter_freq
	alpha = np.sin(w0)/2 * np.sqrt( (A + 1/A)*(1/S - 1) + 2 )

	b0 =    A*( (A+1) + (A-1)*np.cos(w0) + 2*np.sqrt(A)*alpha )
	b1 = -2*A*( (A-1) + (A+1)*np.cos(w0)                   )
	b2 =    A*( (A+1) + (A-1)*np.cos(w0) - 2*np.sqrt(A)*alpha )
	a0 =        (A+1) - (A-1)*np.cos(w0) + 2*np.sqrt(A)*alpha
	a1 =    2*( (A-1) - (A+1)*np.cos(w0)                   )
	a2 =        (A+1) - (A-1)*np.cos(w0) - 2*np.sqrt(A)*alpha

	return (b0,b1,b2,a0,a1,a2)

if type == "notch":      (b0,b1,b2,a0,a1,a2) = _make_biquad_notch    ( freq, Q )
if type == "lowpass":    (b0,b1,b2,a0,a1,a2) = _make_biquad_lowpass  ( freq, Q )
if type == "highpass":   (b0,b1,b2,a0,a1,a2) = _make_biquad_highpass ( freq, Q )
if type == "allpass":    (b0,b1,b2,a0,a1,a2) = _make_biquad_allpass  ( freq, Q )
if type == "bandpass":   (b0,b1,b2,a0,a1,a2) = _make_biquad_bandpass ( freq, Q )
if type == "peaking":    (b0,b1,b2,a0,a1,a2) = _make_biquad_peaking  ( freq, Q, gain )
if type == "highshelf":  (b0,b1,b2,a0,a1,a2) = _make_biquad_highshelf( freq, Q, gain )
if type == "lowshelf":   (b0,b1,b2,a0,a1,a2) = _make_biquad_lowshelf ( freq, Q, gain )

if type == "peaking" or type == "highshelf" or type == "lowshelf":
    if gain > 0:
        plot_response( [b0,b1,b2], [a0,a1,a2], ymin=-3, ymax=3+gain )
    if gain < 0:
        plot_response( [b0,b1,b2], [a0,a1,a2], ymin=-3+gain, ymax=3 )
else:
    plot_response( [b0,b1,b2], [a0,a1,a2], ymin=-60, ymax=6 )

sys.stdout.write( "FQ(%+1.9f),"  % (b0/a0) );
sys.stdout.write( "FQ(%+1.9f),"  % (b1/a0) );
sys.stdout.write( "FQ(%+1.9f),"  % (b2/a0) );
sys.stdout.write( "FQ(%+1.9f),"  % (a1/a0) );
sys.stdout.write( "FQ(%+1.9f)\n" % (a2/a0) );

