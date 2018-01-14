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
// Preamp parameter names and values for preamp stages 1, 2 and 3 are below. The values of these
// parameters for each stage are controlled via FlexFx properties with the property's five 32-bit
// values set to parameters for each preamp stage.
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
// PROP_PREAMP_PARAMS_1[5]  Preamp Lowcut  Bass attenuation cutoff freq for stage 1 preamp input.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
// PROP_PREAMP_PARAMS_1[2]  Input Drive    Preamp drive level for each stage 1 preamp.
//                                         Ranges from 1 (minimum drive) to 9 (maximum drive).
// PROP_PREAMP_PARAMS_1[1]  Tube Bias      Preamp tube bias point for stage 1 preamp.
//                                         Ranges from 1 (cold) to 5 (neutral) to 9 (hot).
// PROP_PREAMP_PARAMS_1[3]  Tube Slewlim   Slew-rate limiting for each stage 1 preamp.
//                                         Ranges from 1 (most/slow) to 9 (least/fast).
// PROP_PREAMP_PARAMS_1[4]  Preamp Tone    Treble attenuation cutoff freq for stage 1 preamp output.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
//
// PROP_PREAMP_PARAMS_2[5]  Preamp Lowcut  Bass attenuation cutoff freq for stage 2 preamp input.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
// PROP_PREAMP_PARAMS_2[2]  Input Drive    Preamp drive level for each stage 2 preamp.
//                                         Ranges from 1 (minimum drive) to 9 (maximum drive).
// PROP_PREAMP_PARAMS_2[1]  Tube Bias      Preamp tube bias point for stage 2 preamp.
//                                         Ranges from 1 (cold) to 5 (neutral) to 9 (hot).
// PROP_PREAMP_PARAMS_2[3]  Tube Slewlim   Slew-rate limiting for each stage 2 preamp.
//                                         Ranges from 1 (most/slow) to 9 (least/fast).
// PROP_PREAMP_PARAMS_2[4]  Preamp Tone    Treble attenuation cutoff freq for stage 2 preamp output.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
//
// PROP_PREAMP_PARAMS_3[5]  Preamp Lowcut  Bass attenuation cutoff freq for stage 3 preamp input.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
// PROP_PREAMP_PARAMS_3[2]  Input Drive    Preamp drive level for each stage 3 preamp.
//                                         Ranges from 1 (minimum drive) to 9 (maximum drive).
// PROP_PREAMP_PARAMS_3[1]  Tube Bias      Preamp tube bias point for stage 3 preamp.
//                                         Ranges from 1 (cold) to 5 (neutral) to 9 (hot).
// PROP_PREAMP_PARAMS_3[3]  Tube Slewlim   Slew-rate limiting for each stage 3 preamp.
//                                         Ranges from 1 (most/slow) to 9 (least/fast).
// PROP_PREAMP_PARAMS_3[4]  Preamp Tone    Treble attenuation cutoff freq for stage 3 preamp output.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
//
// PROP_PREAMP_EMPHASIS[1]  Mid Emphasis   Mid-range boost center frequency for stage 1 preamp.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
// PROP_PREAMP_EMPHASIS[2]  Mid Emphasis   Mid-range boost center frequency for stage 2 preamp.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
// PROP_PREAMP_EMPHASIS[3]  Mid Emphasis   Mid-range boost center frequency for stage 3 preamp.
//                                         Ranges from 1 (min frequency) to 9 (max frequency).
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
//
// PROP_PREAMP_EQ_123[1]    EQ Band 1      Output level for 60 Hz.  Range is 1 (-12dB) to 9 (+12dB).
// PROP_PREAMP_EQ_123[2]    EQ Band 2      Output level for 120 Hz. Range is 1 (-12dB) to 9 (+12dB).
// PROP_PREAMP_EQ_123[3]    EQ Band 3      Output level for 240 Hz. Range is 1 (-12dB) to 9 (+12dB).
// PROP_PREAMP_EQ_456[1]    EQ Band 4      Output level for 480 Hz. Range is 1 (-12dB) to 9 (+12dB).
// PROP_PREAMP_EQ_456[2]    EQ Band 5      Output level for 960 Hz. Range is 1 (-12dB) to 9 (+12dB).
// PROP_PREAMP_EQ_456[3]    EQ Band 6      Output level for 1.92 kHz. Range is 1(-12dB) to 9(+12dB).
// PROP_PREAMP_EQ_789[1]    EQ Band 6      Output level for 3.84 kHz. Range is 1(-12dB) to 9(+12dB).
// PROP_PREAMP_EQ_789[2]    EQ Band 6      Output level for 7.68 kHz. Range is 1(-12dB) to 9(+12dB).
// PROP_PREAMP_EQ_789[3]    EQ Band 6      Output level for 15.4 kHz. Range is 1(-12dB) to 9(+12dB).
//
// Property                 Parameter      Description
// -------------------------------------------------------------------------------------------------
//
// PROP_PREAMP_OUTPUT[1]    Volume Left    Left channel output volume 0.0 (min) to 0.999 (max).
// PROP_PREAMP_OUTPUT[2]    Volume Right   Right channel output volume 0.0 (min) to 0.999 (max).
// =================================================================================================

#define PROP_PREAMP_OUTPUT   0x33010001 // Out Volume L, Out Volume R, 0, 0, 0
#define PROP_PREAMP_EMPHASIS 0x33010002 // Mid-emphasis, index into filter table for each amp stage
#define PROP_PREAMP_PARAMS_1 0x33010003 // Bias,Drive,Slew,Tone,Lowcut
#define PROP_PREAMP_PARAMS_2 0x33010004 // Bias,Drive,Slew,Tone,Lowcut
#define PROP_PREAMP_PARAMS_3 0x33010005 // Bias,Drive,Slew,Tone,Lowcut
#define PROP_PREAMP_EQ_123   0x33010006 // EQ levels for 60 Hz, 120 Hz, 240 Hz
#define PROP_PREAMP_EQ_456   0x33010007 // EQ levels for 480 Hz, 960 kHz, 1.92 kHz
#define PROP_PREAMP_EQ_789   0x33010008 // EQ levels for 3.84 kHz, 7.68 kHz, 15.36 kHz

void efx_preamp__control( int rcv_prop[6], int usb_prop[6], int dsp_prop[6] );

void efx_preamp__mixer( const int* usb_output, int* usb_input,
                        const int* i2s_output, int* i2s_input,
                        const int* dsp_output, int* dsp_input, const int* property );

void efx_preamp__dsp_thread1( int* samples, const int* property );
void efx_preamp__dsp_thread2( int* samples, const int* property );
void efx_preamp__dsp_thread3( int* samples, const int* property );
void efx_preamp__dsp_thread4( int* samples, const int* property );
void efx_preamp__dsp_thread5( int* samples, const int* property );
