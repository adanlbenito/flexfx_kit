// =================================================================================================
// Three-stage tube preamp. Each stage implements a triode preamp model. The three stages are
// followed by a grapic EQ. Preamp and tone parameters range from 1 (lowest frequency, minimum
// level, etc) to 9 (highest frequency, maximum level, etc) with 5 being the middle or neutral
// value.
//
// The FlexFX audio sample rate must be set to 192 kHz. The guitar samples are up-sampled to 960
// kHz for non-linear preamp/tube model processing and then down-sampled back to 192 kHz. Therefore
// the ADC, DAC and USB audio sample rates are all 192 khz.
//
// Each preamp stage incorporates adjustable pre-filtering (bass attenuation), a 12AX7 amplifier
// model with slew-rate limiting and adjustable bias, and post-filtering, all creating a simple
// 12AX7-based guitar preamp stage. The graphic EQ is at the end of the three-stage-preamp chain and
// is used as the final tone-shaping component (a.k.a the preamp's the tone stack).
//
// Mapping a preset to a FlexFX property ...
//
// + Effect identifier (3301 for Digital Preamp v1)
// |
// |      +- Preset (1 through 9)
// |      |
// |      | Preamp Stage 1
// |      | +-------- Lowcut frequency for amp input (1=lowest,9=highest)
// |      | |+------- Midrange boost frequency for amp (1=lowest,9=highest)
// |      | ||+------ Drive level for amp input (1=minimum,9=maximum)
// |      | |||+----- Tube bias for amp tube model (1=cold,5=neutral,9=hot)
// |      | ||||+---- Tube bias shift amount for amp tube model (1=min,9=max)
// |      | |||||+--- Slew-rate limit for amp tube model (1=max,9=min)
// |      | ||||||+-- Highcut frequency for amp output (1=lowest,9=highest)
// |      | |||||||
// |      | |||||||  Preamp Stage 2 (same parameters as stage 1)
// |      | |||||||  |||||||
// |      | |||||||  |||||||  Preamp Stage 3 (same parameters as stage 1)
// |\     | |||||||  |||||||  |||||||
// ||\    | |||||||  |||||||  |||||||  Output EQ (1=-12dB, 5=0dB, 9=+12dB)
// |||\   | |||||||  |||||||  |||||||  +++--------- EQ bands 60Hz,120Hz,240Hz
// ||||   | |||||||  |||||||  |||||||  |||+++------ EQ bands 480Hz,960Hz,1k92
// ||||   | |||||||  |||||||  |||||||  ||||||++-+-- EQ bands 3k84,7k68,15k36
// ||||   | |||||||  |||||||  |||||||  |||||||| |
// 33010001 55555550 55555550 55555550 55555555 50000000   (ASCII/HEX values)
// =================================================================================================

void efx_preamp__control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] );

void efx_preamp__mixer( const int* usb_output, int* usb_input,
                        const int* i2s_output, int* i2s_input,
                        const int* dsp_output, int* dsp_input, const int* property );

void efx_preamp__dsp_thread1( int* samples, const int* property );
void efx_preamp__dsp_thread2( int* samples, const int* property );
void efx_preamp__dsp_thread3( int* samples, const int* property );
void efx_preamp__dsp_thread4( int* samples, const int* property );
void efx_preamp__dsp_thread5( int* samples, const int* property );
