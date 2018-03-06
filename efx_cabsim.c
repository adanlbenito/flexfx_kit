#include "efx_cabsim.h"

const char* company_name_string   = "FlexFX"; // Your company name
const char* product_name_string   = "Cabsim"; // Your product name
const char* usb_audio_output_name = "FlexFX Audio Output"; // USB audio output endpoint name
const char* usb_audio_input_name  = "FlexFX Audio In put"; // USB audio input endpoint name
const char* usb_midi_output_name  = "FlexFX MIDI Output";  // USB MIDI output endpoint name
const char* usb_midi_input_name   = "FlexFX MIDI In put";  // USB MIDI input endpoint name

const int audio_sample_rate     = 48000; // Audio sampling frequency
const int usb_output_chan_count = 2;     // 2 USB audio class 2.0 output channels
const int usb_input_chan_count  = 2;     // 2 USB audio class 2.0 input channels
const int i2s_channel_count     = 2;     // ADC/DAC channels per SDIN/SDOUT wire

//const char interface_string[]  = "No interface is specified";
//const char controller_string[] = "No controller is available";

const int i2s_sync_word[8] = { 0xFFFFFFFF,0x00000000,0,0,0,0,0,0 }; // I2S WCLK values per slot

static void adc_read( double values[4] ) // I2C control for the Analog Devices AD7999 ADC.
{
    byte ii, hi, lo, value;
    i2c_start( 100000 ); // Set bit clock to 400 kHz and assert start condition.
    i2c_write( 0x52+1 ); // Select I2C peripheral for Read.
    for( ii = 0; ii < 4; ++ii ) {
        hi = i2c_read(); i2c_ack(0); // Read low byte, assert ACK.
        lo = i2c_read(); i2c_ack(ii==3); // Read high byte, assert ACK (NACK on last read).
        value = (hi<<4) + (lo>>4); // Select correct value and store ADC sample.
        values[hi>>4] = ((double)value)/256.0; // Convert from byte to double (0<=val<1.0).
    }
    i2c_stop();
}

void control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] )
{
    // Pass incoming properties on to CABSIM control ...
    efx_cabsim__control( rcv_prop, usb_prop, dsp_prop );
    // If outgoing USB or DSP properties are still use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;

    // If outgoing USB or DSP properties are now use then come back later ...
    if( usb_prop[0] != 0 || dsp_prop[0] != 0 ) return;

    double volume = 0, tone = 0, preset = 0, pot_values[4];
    adc_read( pot_values ); // Read pot values for volume, tone and preset

    // Check to see if any of the pots have changed in a meaningful way ...
    int updated[3] = {0,0,0};
    updated[0] = (pot_values[3] < volume-FQ(0.01)) || (pot_values[3] > volume+FQ(0.01));
    updated[1] = (pot_values[2] < tone-FQ(0.01)) || (pot_values[2] > tone+FQ(0.01));
    static int focus = 0; double step = FQ(0.1111111), gap = QF(0.03);
    for( int click = 0, ii = 0; ii < 9; ++ii ) {
        click = preset >= (ii*step+gap) && preset <= (ii*(step+1)-gap);
        if( !click && focus ) focus = 0;
        if( !focus && click ) updated[2] = 1;
    }

    // Check to see of any pot changes result in a need to update control settings ...
    if( updated[0] || updated[1] || updated[2] ) {
        if( updated[0] ) volume = pot_values[3];
        if( updated[1] ) tone = pot_values[2];
        if( updated[2] ) preset = pot_values[1];
        // Send property with updated controll settings to the DSP threads via cabsim control ...
        dsp_prop[0] = 0x00008001;
        dsp_prop[1] = volume; dsp_prop[2] = tone; dsp_prop[3] = preset;
        efx_cabsim__control( rcv_prop, usb_prop, dsp_prop );
    }
}

void mixer( const int* usb_output, int* usb_input,
            const int* i2s_output, int* i2s_input,
            const int* dsp_output, int* dsp_input, const int* property )
{
    efx_cabsim__mixer( usb_output, usb_input, i2s_output, i2s_input,
                       dsp_output, dsp_input, property );
}

void dsp_thread1( int* samples, const int* property ) {efx_cabsim__dsp_thread1(samples,property);}
void dsp_thread2( int* samples, const int* property ) {efx_cabsim__dsp_thread2(samples,property);}
void dsp_thread3( int* samples, const int* property ) {efx_cabsim__dsp_thread3(samples,property);}
void dsp_thread4( int* samples, const int* property ) {efx_cabsim__dsp_thread4(samples,property);}
void dsp_thread5( int* samples, const int* property ) {efx_cabsim__dsp_thread5(samples,property);}
