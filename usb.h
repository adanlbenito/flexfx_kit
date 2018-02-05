#ifndef UAC2_INCLUDED
#define UAC2_INCLUDED

void UAC2
(
    chanend ? c_control_messages[],
    chanend ? c_audio_messages[],
    chanend ? c_midi_messages[]
);

// USB state callback/event functions to be implemented by the user's application

void UAC2_system__usb_connected( chanend c_control_messages[], int connected_flag );
void UAC2_system__audio_start  ( chanend c_control_messages[] );
void UAC2_system__audio_stop   ( chanend c_control_messages[] );

// Audio state callback/Event functions to be implemented by the user's application

void UAC2_control__get_output_mute  ( chanend c_control_messages[], int channel, int& flag );
void UAC2_control__set_output_mute  ( chanend c_control_messages[], int channel, int  flag );
void UAC2_control__get_output_volume( chanend c_control_messages[], int channel, int& level );
void UAC2_control__set_output_volume( chanend c_control_messages[], int channel, int  level );

void UAC2_control__get_input_mute  ( chanend c_control_messages[], int channel, int& flag );
void UAC2_control__set_input_mute  ( chanend c_control_messages[], int channel, int  flag );
void UAC2_control__get_input_volume( chanend c_control_messages[], int channel, int& level );
void UAC2_control__set_input_volume( chanend c_control_messages[], int channel, int  level );

// Audio flow callback/event functions to be implemented by the user's application

void UAC2_audio__initialize ( chanend c_audio_messages[] );
void UAC2_audio__configure  ( chanend c_audio_messages[], int sample_rate );
void UAC2_audio__synchronize( chanend c_audio_messages[] );
void UAC2_audio__transfer   ( chanend c_audio_messages[], unsigned *unsafe output_samples, unsigned *unsafe input_samples );

// MIDI control and callback functions to be implemented by the user's application

void UAC2_midi__initialize( chanend c_midi_messages[] );
void UAC2_midi__configure ( chanend c_midi_messages[] );
void UAC2_midi__transfer  ( chanend c_midi_messages[], int *unsafe send_property, int *unsafe recv_property );

#endif

