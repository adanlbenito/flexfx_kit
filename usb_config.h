#ifndef _UAC2_CONFIG_H_
#define _UAC2_CONFIG_H_

#define _UAC2_USB_INTERFACE_INDEX__AUDIO_CONTROL  0
#define _UAC2_USB_INTERFACE_INDEX__AUDIO_INPUT    1
#define _UAC2_USB_INTERFACE_INDEX__AUDIO_OUTPUT   2
#define _UAC2_USB_INTERFACE_INDEX__MIDI_STANDARD  3
#define _UAC2_USB_INTERFACE_INDEX__MIDI_SPECIFIC  4

#define _UAC2_USB_AUDIO_INTERFACE_COUNT 3 // Input, output, and control interfaces
#define _UAC2_USB_TOTAL_INTERFACE_COUNT 5 // Audio(3), MIDI(2), Bulk(1)
//#define _UAC2_USB_TOTAL_INTERFACE_COUNT 6 // Audio(3), MIDI(2), Bulk(1)

#define _UAC2_INPUT_ENDPOINT_INDEX__CONTROL    0  // Default control interface
#define _UAC2_INPUT_ENDPOINT_INDEX__FEEDBACK   1  // Audio rate feedback
#define _UAC2_INPUT_ENDPOINT_INDEX__AUDIO      2  // Audio data input
#define _UAC2_INPUT_ENDPOINT_INDEX__MIDI       3

#define _UAC2_OUTPUT_ENDPOINT_INDEX__CONTROL  0  // Default control interface
#define _UAC2_OUTPUT_ENDPOINT_INDEX__AUDIO    1  // Audio data output
#define _UAC2_OUTPUT_ENDPOINT_INDEX__MIDI     2

#define _UAC2_INPUT_ENDPOINT_ADDR__CONTROL    (0x80 | _UAC2_INPUT_ENDPOINT_INDEX__CONTROL)
#define _UAC2_INPUT_ENDPOINT_ADDR__FEEDBACK   (0x80 | _UAC2_INPUT_ENDPOINT_INDEX__FEEDBACK)
#define _UAC2_INPUT_ENDPOINT_ADDR__AUDIO      (0x80 | _UAC2_INPUT_ENDPOINT_INDEX__AUDIO)
#define _UAC2_INPUT_ENDPOINT_ADDR__MIDI       (0x80 | _UAC2_INPUT_ENDPOINT_INDEX__MIDI)

#define _UAC2_OUTPUT_ENDPOINT_ADDR__CONTROL  _UAC2_OUTPUT_ENDPOINT_INDEX__CONTROL
#define _UAC2_OUTPUT_ENDPOINT_ADDR__AUDIO    _UAC2_OUTPUT_ENDPOINT_INDEX__AUDIO
#define _UAC2_OUTPUT_ENDPOINT_ADDR__MIDI     _UAC2_OUTPUT_ENDPOINT_INDEX__MIDI

#define INPUT_INTERFACES        (1)
#define OUTPUT_INTERFACES       (1)

#define USB_BUS_MAX_POWER 250

#define NUM_CLOCKS              (1)

/* Audio Unit ID defines */
#define FU_USBIN                 11              /* Feature Unit: USB Audio device -> host */
#define FU_USBOUT                10              /* Feature Unit: USB Audio host -> device*/
#define ID_IT_USB                2               /* Input terminal: USB streaming */
#define ID_IT_AUD                1               /* Input terminal: Analogue input */
#define ID_OT_USB                22              /* Output terminal: USB streaming */
#define ID_OT_AUD                20              /* Output terminal: Analogue output */
#define ID_CLKSEL                40              /* Clock selector ID */
#define ID_CLKSRC_INT            41              /* Clock source ID (internal) */

#endif
