#ifndef _UAC2_DESCRIPTORS_H_
#define _UAC2_DESCRIPTORS_H_

#include "xud_usb.h"
#include "usb_audio.h"
#include "usb_config.h"

#define UAC2_AUDIO_VENDOR_ID  0x20B1
#define UAC2_AUDIO_PRODUCT_ID 0x3301
#define UAC2_OUTPUT_CHANNEL_COUNT  2
#define UAC2_INPUT_CHANNEL_COUNT   2
#define UAC2_OUTPUT_SAMPLE_SIZE   32
#define UAC2_INPUT_SAMPLE_SIZE    32

int UAC2_strDescs_length;

char* UAC2_strDescs[] =
{
    "Langids",                        // LangIDs place holder
    "BITSTREAM                     ", // iManufacturer
    "FLEXFX                        ", // iProduct and iInterface for control interface
    "Serial                        ", // iSerialNumber
    "Clock Select",
    "Default Clock",
    "Audio Out",
    "Audio In"
    "MIDI Out",
    "MIDI In",
};

#define MANUFACTURER_STRING_INDEX     1
#define PRODUCT_STRING_INDEX          2
#define SERIAL_NUM_STRING_INDEX       3
#define CLOCK_SELECT_STRING_INDEX     4
#define INTERNAL_CLOCK_STRING_INDEX   5
#define OUTPUT_INTERFACE_STRING_INDEX 6
#define INPUT_INTERFACE_STRING_INDEX  7
#define MIDI_OUT_STRING_INDEX         8
#define MIDI_IN_STRING_INDEX          9

// Device Descriptor for Audio Class 2.0 (Assumes High-Speed )

unsigned char UAC2_devDesc_Audio2[] =
{
    18,                              /* 0  bLength : Size of descriptor in Bytes (18 Bytes) */
    USB_DESCTYPE_DEVICE,           /* 1  bdescriptorType */
    0,                               /* 2  bcdUSB */
    2,                               /* 3  bcdUSB */
    0xEF,                            /* 4  bDeviceClass (See Audio Class Spec page 45) */
    0x02,                           /* 5  bDeviceSubClass */
    0x01,                           /* 6  bDeviceProtocol */
    64,                              /* 7  bMaxPacketSize */
    (UAC2_AUDIO_VENDOR_ID & 0xFF), /* 8  idVendor */
    (UAC2_AUDIO_VENDOR_ID >> 8),   /* 9  idVendor */
    (UAC2_AUDIO_PRODUCT_ID & 0xFF),/* 10 idProduct */
    (UAC2_AUDIO_PRODUCT_ID >> 8),  /* 11 idProduct */
    (0x12 & 0xFF),   /* 12 bcdDevice : Device release number */
    (0x13 >> 8),     /* 13 bcdDevice : Device release number */
    1,         /* 14 iManufacturer : Index of manufacturer string */
    2,           /* 15 iProduct : Index of product string descriptor */
    1,//    SERIAL_STR_INDEX,       /* 16 iSerialNumber : Index of serial number decriptor */
    0x02                             /* 17 bNumConfigurations : Number of possible configs. Set to 2 so that Windows
                                          does not load Composite driver. */
};

// Device Descriptor for Null Device

unsigned char UAC2_devDesc_Null[] =
{
    18,                              /* 0  bLength : Size of descriptor in Bytes (18 Bytes) */
    USB_DESCTYPE_DEVICE,           /* 1  bdescriptorType */
    0,                               /* 2  bcdUSB */
    2,                               /* 3  bcdUSB */
    0x0,                            /* 4  bDeviceClass */
    0x0  ,                           /* 5  bDeviceSubClass */
    0x00,                           /* 6  bDeviceProtocol */
    64,                              /* 7  bMaxPacketSize */
    (UAC2_AUDIO_VENDOR_ID & 0xFF), /* 8  idVendor */
    (UAC2_AUDIO_VENDOR_ID >> 8),   /* 9  idVendor */
    (UAC2_AUDIO_PRODUCT_ID & 0xFF),/* 10 idProduct */
    (UAC2_AUDIO_PRODUCT_ID >> 8),  /* 11 idProduct */
    (0x12 & 0xFF),   /* 12 bcdDevice : Device release number */
    (0x34 >> 8),     /* 13 bcdDevice : Device release number */
    MANUFACTURER_STRING_INDEX,         /* 14 iManufacturer : Index of manufacturer string */
    PRODUCT_STRING_INDEX,           /* 15 iProduct : Index of product string descriptor */
    SERIAL_NUM_STRING_INDEX,           /* 16 iSerialNumber : Index of serial number decriptor */
    0x01                             /* 17 bNumConfigurations : Number of possible configs */
};

/* Lenths of input/output term descriptors - from spec */
#define LEN_OUTPUT_TERMINAL         (0x0C)
#define LEN_INPUT_TERMINAL          (0x11)

/* Lengh of out terminal descriptors in total */
#define LEN_TERMS_OUT               ((LEN_OUTPUT_TERMINAL + LEN_INPUT_TERMINAL) * OUTPUT_INTERFACES)
#define LEN_TERMS_IN                ((LEN_OUTPUT_TERMINAL + LEN_INPUT_TERMINAL) * INPUT_INTERFACES)

/* Calc total length of configuration desc based on defines */
#define LEN_FU_OUT                  (6 + (UAC2_OUTPUT_CHANNEL_COUNT + 1) * 4)
//#define LEN_FU_OUT                  0

#define LEN_FU_IN                   (6 + (UAC2_INPUT_CHANNEL_COUNT + 1) * 4)
//#define LEN_FU_IN                   0

#define LEN_CLK                     (8)
#define LEN_CLK_SEL                 (7 + NUM_CLOCKS)
#define LEN_CLOCKING                (LEN_CLK_SEL + (NUM_CLOCKS * LEN_CLK))

/* Total length of the Class-Specific AC Interface Descriptor - Clock Entities, Units and Terminals */
#define LEN_AC                      (9)
#define TLEN_AC                     (LEN_AC + LEN_FU_OUT + LEN_FU_IN + LEN_CLOCKING + LEN_TERMS_OUT + LEN_TERMS_IN)


#define USB_CDC_ACM_SUBCLASS        0x02
#define USB_CDC_AT_COMMAND_PROTOCOL 0x01
#define USB_DESCTYPE_CS_INTERFACE   0x24
//#define CDC_NOTIFICATION_EP_NUM     1
//#define CDC_DATA_RX_EP_NUM          1
//#define CDC_DATA_TX_EP_NUM          2
#define MAX_EP_SIZE                 512
#define CDC_SET_LINE_CODING         0x20
#define CDC_GET_LINE_CODING         0x21
#define CDC_SET_CONTROL_LINE_STATE  0x22
#define CDC_SEND_BREAK              0x23

/* Configuration Descriptor for Audio 2.0 (HS) operation */
unsigned char UAC2_cfgDesc_Audio2[] =
{
    0x09,                            /* 0  bLength */
    USB_DESCTYPE_CONFIGURATION,    /* 1  bDescriptorType */

    0,0, //2,3  set at enumeration time

    _UAC2_USB_TOTAL_INTERFACE_COUNT,                   /* 4  bNumInterface: Number of interfaces*/
    0x01,                            /* 5  bConfigurationValue */
    0x00,                            /* 6  iConfiguration */
    #ifdef SELF_POWERED
    192,                            /* 7  bmAttributes */
    #else
    128,                            /* 7  bmAttributes */
    #endif
    USB_BUS_MAX_POWER,              /* 8  bMaxPower */

    /* Interface Association Descriptor */

    0x08,                            /* 0  bLength */
    0x0b,                            /* 1  bDescriptorType */
    0x00,                            /* 2  bFirstInterface */
    _UAC2_USB_AUDIO_INTERFACE_COUNT,                /* 3  bInterfaceCount */
    USB_CLASS_AUDIO,                /* 4  bFunctionClass: AUDIO_FUNCTION */

    //FUNCTION_SUBCLASS_UNDEFINED,  /* 5  bFunctionSubClass: FUNCTION_SUBCLASS_UNDEFINED */
    0,    /* 5  bFunctionSubClass: FUNCTION_SUBCLASS_UNDEFINED */

    AF_VERSION_02_00,                /* 6  bFunctionProtocol: AF_VERSION_02_00 */
    0x00,                            /* 7  iFunction (String Index) */

    /* Standard Audio Control Interface Descriptor (Note: Must be first with lowest interface number)r */
    0x09,                            /* 0  bLength: 9 */
    USB_DESCTYPE_INTERFACE,        /* 1  bDescriptorType: INTERFACE */
    0x00,                            /* 2  bInterfaceNumber */
    0x00,                            /* 3  bAlternateSetting: Must be 0 */
    0x00,                            /* 4  bNumEndpoints (0 or 1 if optional interrupt endpoint is present */
    AUDIO,                           /* 5  bInterfaceClass: AUDIO */
    AUDIOCONTROL,                   /* 6  bInterfaceSubClass: AUDIOCONTROL*/
    IP_VERSION_02_00,                /* 7  bInterfaceProtocol: IP_VERSION_02_00 */
    PRODUCT_STRING_INDEX,           /* 8  iInterface (re-use iProduct) */




    // -----------------------------------------------------------------------------
    /* Class Specific Audio Control Interface Header Descriptor: */

    LEN_AC,                            /* 0   bLength */
    CS_INTERFACE,                   /* 1   bDescriptorType: 0x24 */
    HEADER,                          /* 2   bDescriptorSubtype: HEADER */
    0x00, 0x02,                      /* 3:4 bcdUSB */
    IO_BOX,                         /* 5   bCatagory (Primary use of audio function) */
    (TLEN_AC & 0xFF),                 /* 6   wTotalLength */
    (TLEN_AC >> 8),                   /* 7   wTotalLength */
    0x00,                            /* 8   bmControls (0:1 Latency Control, 2:7 must be 0 */

    /* Clock Source Descriptor (4.7.2.1) */
    LEN_CLK,                           /* 0   bLength: 8 */
    CS_INTERFACE,                   /* 1   bDescriptorType */
    CLOCK_SOURCE,                   /* 2   bDescriptorSubtype */
    ID_CLKSRC_INT,                     /* 3   bClockID */
    0x03,                              /* 4   bmAttributes:
                                               D[1:0] :
                                                00: External Clock
                                                01: Internal Fixed Clock
                                                10: Internal Variable Clock
                                                11: Internal Progamable Clock
                                               D[2]   : Clock synced to SOF
                                               D[7:3] : Reserved (0) */
    0x07,                            /* 5   bmControls
                                             D[1:0] : Clock Freq Control
                                            D[3:2] : Clock Validity Control
                                            D[7:4] : Reserved (0) */
    0x00,                            /* 6   bAssocTerminal */
    INTERNAL_CLOCK_STRING_INDEX,                            /* 7   iClockSource (String Index) */

    /* Clock Selector Descriptor (4.7.2.2) */

    LEN_CLK_SEL,                    /* 0    bLength */
    CS_INTERFACE,                   /* 1    bDescriptorType */
    CLOCK_SELECTOR,                 /* 2    bDescriptorSubtype */
    ID_CLKSEL,                      /* 3    bClockID */
    NUM_CLOCKS,                     /* 4    Number of input pins*/
    ID_CLKSRC_INT,                  // 5 ID of default clock
    0x03,                           /* 6   bmControls
                                            D[1:0] : Clock Selector Control
                                            D[7:4] : Reserved (0) */
    CLOCK_SELECT_STRING_INDEX,                             /* 7   iClockSel (String Index) */

    // -----------------------------------------------------------------------------

    #if UAC2_OUTPUT_CHANNEL_COUNT > 0

    /* OUTPUT PATH FROM HOST TO DEVICE */
    /* Input Terminal Descriptor (USB Input Terminal) */

    0x11,                              /* 0  bLength in bytes: 17 */
    CS_INTERFACE,                      /* 1  bDescriptorType: 0x24 */
    INPUT_TERMINAL,                  /* 2  bDescriptorSubType: INPUT_TERMINAL */
    ID_IT_USB,                          /* 3  bTerminalID */
    (USB_TERMTYPE_USB_STREAMING&0xff),USB_TERMTYPE_USB_STREAMING>>8, /* 4  wTerminalType: USB Streaming */
    0x00,                              /* 6  bAssocTerminal */
    ID_CLKSEL,                          /* 7  bCSourceID: ID of Clock Entity */
    UAC2_OUTPUT_CHANNEL_COUNT,                 /* 8  bNrChannels */
    0,0,0,0,                         /* 9  bmChannelConfig */
    OUTPUT_INTERFACE_STRING_INDEX,  /* 13 iChannelNames */
    0x00, 0x00,                     /* 14 bmControls */
    6,                               /* 16 iTerminal */

    /* Output Terminal Descriptor (Audio) */

    0x0C,                                /* 0  bLength */
    CS_INTERFACE,                    /* 1  bDescriptorType: 0x24 */
    OUTPUT_TERMINAL,                    /* 2  bDescriptorSubType: OUTPUT_TERMINAL */
    ID_OT_AUD,                            /* 3  bTerminalID */
    (SPEAKER&0xff),SPEAKER>>8,      /* 4  wTerminalType */
    0x00,                            /* 6  bAssocTerminal */
    FU_USBOUT,                        /* 7  bSourceID Connect to analog input feature unit*/
    ID_CLKSEL,                         /* 8  bCSourceUD */
    0x00, 0x00,                      /* 9  bmControls */
    0,                                /* 11 iTerminal */
    #endif

    #if UAC2_INPUT_CHANNEL_COUNT > 0

    /* INPUT FROM DEVICE TO HOST PATH */
    /* Input Terminal Descriptor (Analogue Input Terminal) */

    0x11,                              /* 0  bLength in bytes: 17 */
    CS_INTERFACE,                      /* 1  bDescriptorType: 0x24 */
    INPUT_TERMINAL,                  /* 2  bDescriptorSubType: INPUT_TERMINAL */
    ID_IT_AUD,                          /* 3  bTerminalID */
    (MICROPHONE_&0xff),MICROPHONE_>>8,     /* 4  wTerminalType: USB Streaming */
    0x00,                             /* 6  bAssocTerminal */
    ID_CLKSEL,                          /* 7  bCSourceID: ID of Clock Entity */
    UAC2_INPUT_CHANNEL_COUNT,        /* 8  bNrChannels */
    0,0,0,0,                         /* 9  bmChannelConfig */
    INPUT_INTERFACE_STRING_INDEX,   /* 13 iChannelNames */
    0x00, 0x00,                     /* 14 bmControls */
    0,                               /* 16 iTerminal */

    /* Output Terminal Descriptor (USB Streaming) */
    0x0C,                                /* 0  bLength */
    CS_INTERFACE,                    /* 1  bDescriptorType: 0x24 */
    OUTPUT_TERMINAL,                    /* 2  bDescriptorSubType: OUTPUT_TERMINAL */
    ID_OT_USB,                            /* 3  bTerminalID */
    (USB_TERMTYPE_USB_STREAMING&0xff),USB_TERMTYPE_USB_STREAMING>>8, /* 4  wTerminalType */
    0x00,                            /* 6  bAssocTerminal */
    FU_USBIN,                        /* 7  bSourceID Connect to analog input feature unit*/
    ID_CLKSEL,                            /* 8  bCSourceUD */
    0x00, 0x00,                      /* 9  bmControls */
    7,                                /* 11 iTerminal */
    #endif

    #if UAC2_OUTPUT_CHANNEL_COUNT > 0

    /* Zero bandwith alternative 0 */
    /* Standard AS Interface Descriptor (4.9.1) */

    0x09,                           /* 0  bLength: (in bytes, 9) */
    USB_DESCTYPE_INTERFACE,        /* 1  bDescriptorType: INTERFACE */
    1,                              /* 2  bInterfaceNumber: Number of interface */
    0,                              /* 3  bAlternateSetting */
    0,                              /* 4  bNumEndpoints */
    AUDIO,                           /* 5  bInterfaceClass: AUDIO */
    AUDIOSTREAMING,                   /* 6  bInterfaceSubClass: AUDIO_STREAMING */
    IP_VERSION_02_00,               /* 7  bInterfaceProtocol: IP_VERSION_02_00 */
    4,                              /* 8  iInterface: (Sting index) */

    /* Alternative 1 */
    /* Standard AS Interface Descriptor (4.9.1) (Alt) */

    0x09,                           /* 0  bLength: (in bytes, 9) */
    USB_DESCTYPE_INTERFACE,        /* 1  bDescriptorType: INTERFACE */
    1,                              /* 2  bInterfaceNumber: Number of interface */
    1,                              /* 3  bAlternateSetting */
    2,                              /* 4  bNumEndpoints */
    AUDIO,                           /* 5  bInterfaceClass: AUDIO */
    AUDIOSTREAMING,                   /* 6  bInterfaceSubClass: AUDIO_STREAMING */
    IP_VERSION_02_00,               /* 7  bInterfaceProtocol: IP_VERSION_02_00 */
    4,                              /* 8  iInterface: (Sting index) */

    /* Class Specific AS Interface Descriptor */

    0x10,                           /* 0  bLength: 16 */
    CS_INTERFACE,                   /* 1  bDescriptorType: 0x24 */
    AS_GENERAL,                     /* 2  bDescriptorSubType */
    ID_IT_USB,                      /* 3  bTerminalLink (Linked to USB input terminal) */
    0x00,                           /* 4  bmControls */
    0x01,                           /* 5  bFormatType */
    PCM, 0x00, 0x00, 0x00,          /* 6:10  bmFormats (note this is a bitmap) */
    UAC2_OUTPUT_CHANNEL_COUNT,     /* 11 bNrChannels */
    0,0,0,0,                        /* 12:14: bmChannelConfig */
    OUTPUT_INTERFACE_STRING_INDEX,  /* 15 iChannelNames */

    /* Type 1 Format Type Descriptor */

    0x06,                             /* 0  bLength (in bytes): 6 */
    CS_INTERFACE,                     /* 1  bDescriptorType: 0x24 */
    FORMAT_TYPE,                     /* 2  bDescriptorSubtype: FORMAT_TYPE */
    FORMAT_TYPE_I,                     /* 3  bFormatType: FORMAT_TYPE_1 */
    (UAC2_OUTPUT_SAMPLE_SIZE/8),   /* 4  bSubslotSize (Number of bytes per subslot) */
    24,                             /* 5  bBitResolution (Number of bits used per subslot) */

    /* Standard AS Isochronous Audio Data Endpoint Descriptor (4.10.1.1) */

    0x07,                           /* 0  bLength: 7 */
    USB_DESCTYPE_ENDPOINT,                   /* 1  bDescriptorType: ENDPOINT */
    0x01,                            /* 2  bEndpointAddress (D7: 0:out, 1:in) */
    0x05,                              /* 3  bmAttributes (bitmap)  */
//    MAX_PACKET_SIZE_OUT_HS&0xff,        /* 4  wMaxPacketSize */
//    (MAX_PACKET_SIZE_OUT_HS&0xff00)>>8, /* 5  wMaxPacketSize */
    0, 4,                         // 1024
    1,                              /* 6  bInterval */

    /* Class-Specific AS Isochronous Audio Data Endpoint Descriptor (4.10.1.2) */

    0x08,                           /* 0   bLength */
    CS_ENDPOINT,                       /* 1   bDescriptorType */
    0x01,                           /* 2   bDescriptorSubtype */
    0x00,                           /* 3   bmAttributes */
    0x00,                           /* 4   bmControls (Bitmap: Pitch control, over/underun etc) */
    0x02,                           /* 5   bLockDelayUnits: Decoded PCM samples */
    8,0,                            /* 6:7 bLockDelay */

    /* Feedback EP */

    0x07,                           /* 0  bLength: 7 */
    USB_DESCTYPE_ENDPOINT,         /* 1  bDescriptorType: ENDPOINT */
    0x81,                            /* 2  bEndpointAddress (D7: 0:out, 1:in) */
    17,                              /* 3  bmAttributes (bitmap)  */
    4,0,                            /* 4  wMaxPacketSize */
    4,                              /* 6  bInterval. Only values <= 1 frame (4) supported by MS */
    #endif /* OUTPUT */

    /* Standard AS Interface Descriptor (4.9.1) */

    #if UAC2_INPUT_CHANNEL_COUNT > 0
    0x09,                               /* 0  bLength: (in bytes, 9) */
    USB_DESCTYPE_INTERFACE,           /* 1  bDescriptorType: INTERFACE */
    (OUTPUT_INTERFACES + 1),        /* 2  bInterfaceNumber: Number of interface */
    0,                              /* 3  bAlternateSetting */
    0,                              /* 4  bNumEndpoints */
    AUDIO,                           /* 5  bInterfaceClass: AUDIO */
    AUDIOSTREAMING,                   /* 6  bInterfaceSubClass: AUDIO_STREAMING */
    0x20,                           /* 7  bInterfaceProtocol: IP_VERSION_02_00 */
    5,                              /* 8  iInterface: (Sting index) */

    /* Standard AS Interface Descriptor (4.9.1) (Alt) */

    0x09,                           /* 0  bLength: (in bytes, 9) */
    USB_DESCTYPE_INTERFACE,        /* 1  bDescriptorType: INTERFACE */
    (OUTPUT_INTERFACES + 1),        /* 2  bInterfaceNumber: Number of interface */
    1,                              /* 3  bAlternateSetting */
    1,                              /* 4  bNumEndpoints */
    AUDIO,                           /* 5  bInterfaceClass: AUDIO */
    AUDIOSTREAMING,                   /* 6  bInterfaceSubClass: AUDIO_STREAMING */
    IP_VERSION_02_00,               /* 7  bInterfaceProtocol: IP_VERSION_02_00 */
    5,                              /* 8  iInterface: (Sting index) */

    /* Class Specific AS Interface Descriptor */

    0x10,                           /* 0  bLength: 16 */
    CS_INTERFACE,                   /* 1  bDescriptorType: 0x24 */
    AS_GENERAL,                     /* 2  bDescriptorSubType */
    ID_OT_USB,                      /* 3  bTerminalLink */
    0x00,                           /* 4  bmControls */
    0x01,                           /* 5  bFormatType */
    PCM, 0x00, 0x00, 0x00,          /* 6:10  bmFormats (note this is a bitmap) */
    UAC2_INPUT_CHANNEL_COUNT,      /* 11 bNrChannels */
    0,0,0,0,                        /* 12:14: bmChannelConfig */
    INPUT_INTERFACE_STRING_INDEX,   /* 15 iChannelNames */

    /* Type 1 Format Type Descriptor */

    0x06,                             /* 0  bLength (in bytes): 6 */
    CS_INTERFACE,                     /* 1  bDescriptorType: 0x24 */
    FORMAT_TYPE,                     /* 2  bDescriptorSubtype: FORMAT_TYPE */
    FORMAT_TYPE_I,                     /* 3  bFormatType: FORMAT_TYPE_1 */
    (UAC2_INPUT_SAMPLE_SIZE/8),    /* 4  bSubslotSize (Number of bytes per subslot) */
    24,                             /* 5  bBitResolution (Number of bits used per subslot) */

    /* Standard AS Isochronous Audio Data Endpoint Descriptor (4.10.1.1) */

    0x07,                           /* 0  bLength: 7 */
    USB_DESCTYPE_ENDPOINT,         /* 1  bDescriptorType: ENDPOINT */
    0x82,                            /* 2  bEndpointAddress (D7: 0:out, 1:in) */
    5,                              /* 3  bmAttributes (bitmap)  */
    0,4,                            /* 4  wMaxPacketSize */
    1,                              /* 6  bInterval */

    /* Class-Specific AS Isochronous Audio Data Endpoint Descriptor (4.10.1.2) */
    0x08,                           /* 0   bLength */
    CS_ENDPOINT,                       /* 1   bDescriptorType */
    0x01,                           /* 2   bDescriptorSubtype */
    0x00,                              /* 3   bmAttributes */
    0x00,                           /* 4   bmControls (Bitmap: Pitch control, over/underun etc) */
    0x02,                           /* 5   bLockDelayUnits: Decoded PCM samples */
    8,0,                             /* 6:7 bLockDelay */
    #endif

    /* MIDI Descriptors */
    /* Table B-3: MIDI Adapter Standard AC Interface Descriptor */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x04,                            /* 1 bDescriptorType : INTERFACE descriptor. (field size 1 bytes) */
    _UAC2_USB_INTERFACE_INDEX__MIDI_STANDARD,/* 2 bInterfaceNumber : Index of this interface. (field size 1 bytes) */
    0x00,                            /* 3 bAlternateSetting : Index of this setting. (field size 1 bytes) */
    0x00,                            /* 4 bNumEndpoints : 0 endpoints. (field size 1 bytes) */
    0x01,                            /* 5 bInterfaceClass : AUDIO. (field size 1 bytes) */
    0x01,                            /* 6 bInterfaceSubclass : AUDIO_CONTROL. (field size 1 bytes) */
    0x00,                            /* 7 bInterfaceProtocol : Unused. (field size 1 bytes) */
    0x00,                            /* 8 iInterface : Unused. (field size 1 bytes) */

    /* Table B-4: MIDI Adapter Class-specific AC Interface Descriptor */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : 0x24. (field size 1 bytes) */
    0x01,                            /* 2 bDescriptorSubtype : HEADER subtype. (field size 1 bytes) */
    0x00,                            /* 3 bcdADC : Revision of class specification - 1.0 (field size 2 bytes) */
    0x01,                            /* 4 bcdADC */
    0x09,                            /* 5 wTotalLength : Total size of class specific descriptors. (field size 2 bytes) */
    0x00,                            /* 6 wTotalLength */
    0x01,                            /* 7 bInCollection : Number of streaming interfaces. (field size 1 bytes) */
    0x01,                            /* 8 baInterfaceNr(1) : MIDIStreaming interface 1 belongs to this AudioControl interface */

    /* Table B-5: MIDI Adapter Standard MS Interface Descriptor */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x04,                            /* 1 bDescriptorType : INTERFACE descriptor. (field size 1 bytes) */
    _UAC2_USB_INTERFACE_INDEX__MIDI_SPECIFIC,                            /* 2 bInterfaceNumber : Index of this interface. (field size 1 bytes) */
    0x00,                            /* 3 bAlternateSetting : Index of this alternate setting. (field size 1 bytes) */
    0x02,                            /* 4 bNumEndpoints : 2 endpoints. (field size 1 bytes) */
    0x01,                            /* 5 bInterfaceClass : AUDIO. (field size 1 bytes) */
    0x03,                            /* 6 bInterfaceSubclass : MIDISTREAMING. (field size 1 bytes) */
    0x00,                            /* 7 bInterfaceProtocol : Unused. (field size 1 bytes) */
    0x00,                            /* 8 iInterface : Unused. (field size 1 bytes) */

    /* Table B-6: MIDI Adapter Class-specific MS Interface Descriptor */
    0x07,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : CS_INTERFACE. (field size 1 bytes) */
    0x01,                            /* 2 bDescriptorSubtype : MS_HEADER subtype. (field size 1 bytes) */
    0x00,                            /* 3 BcdADC : Revision of this class specification. (field size 2 bytes) */
    0x01,                            /* 4 BcdADC */
    0x41,                            /* 5 wTotalLength : Total size of class-specific descriptors. (field size 2 bytes) */
    0x00,                            /* 6 wTotalLength */

    /* Table B-7: MIDI Adapter MIDI IN Jack Descriptor (Embedded) */

    0x06,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : CS_INTERFACE. (field size 1 bytes) */
    0x02,                            /* 2 bDescriptorSubtype : MIDI_IN_JACK subtype. (field size 1 bytes) */
    0x01,                            /* 3 bJackType : EMBEDDED. (field size 1 bytes) */
    0x01,                            /* 4 bJackID : ID of this Jack. (field size 1 bytes) */
    0x00,                            /* 5 iJack : Unused. (field size 1 bytes) */

    /* Table B-8: MIDI Adapter MIDI IN Jack Descriptor (External) */

    0x06,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : CS_INTERFACE. (field size 1 bytes) */
    0x02,                            /* 2 bDescriptorSubtype : MIDI_IN_JACK subtype. (field size 1 bytes) */
    0x02,                            /* 3 bJackType : EXTERNAL. (field size 1 bytes) */
    0x02,                            /* 4 bJackID : ID of this Jack. (field size 1 bytes) */
    MIDI_IN_STRING_INDEX,            /* 5 iJack : Unused. (field size 1 bytes) */

    /* Table B-9: MIDI Adapter MIDI OUT Jack Descriptor (Embedded) */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : CS_INTERFACE. (field size 1 bytes) */
    0x03,                            /* 2 bDescriptorSubtype : MIDI_OUT_JACK subtype. (field size 1 bytes) */
    0x01,                            /* 3 bJackType : EMBEDDED. (field size 1 bytes) */
    0x03,                            /* 4 bJackID : ID of this Jack. (field size 1 bytes) */
    0x01,                            /* 5 bNrInputPins : Number of Input Pins of this Jack. (field size 1 bytes) */
    0x02,                            /* 6 BaSourceID(1) : ID of the Entity to which this Pin is connected. (field size 1 bytes) */
    0x01,                            /* 7 BaSourcePin(1) : Output Pin number of the Entityt o which this Input Pin is connected. */
    0x00,                            /* 8 iJack : Unused. (field size 1 bytes) */

    /* Table B-10: MIDI Adapter MIDI OUT Jack Descriptor (External) */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x24,                            /* 1 bDescriptorType : CS_INTERFACE. (field size 1 bytes) */
    0x03,                            /* 2 bDescriptorSubtype : MIDI_OUT_JACK subtype. (field size 1 bytes) */
    0x02,                            /* 3 bJackType : EXTERNAL. (field size 1 bytes) */
    0x04,                            /* 4 bJackID : ID of this Jack. (field size 1 bytes) */
    0x01,                            /* 5 bNrInputPins : Number of Input Pins of this Jack. (field size 1 bytes) */
    0x01,                            /* 6 BaSourceID(1) : ID of the Entity to which this Pin is connected. (field size 1 bytes) */
    0x01,                            /* 7 BaSourcePin(1) : Output Pin number of the Entity to which this Input Pin is connected. */
    MIDI_OUT_STRING_INDEX,           /* 8 iJack : Unused. (field size 1 bytes) */

    /* Table B-11: MIDI Adapter Standard Bulk OUT Endpoint Descriptor */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x05,                            /* 1 bDescriptorType : ENDPOINT descriptor. (field size 1 bytes) */
    _UAC2_OUTPUT_ENDPOINT_ADDR__MIDI,                 /* 2 bEndpointAddress : OUT Endpoint 3. (field size 1 bytes) */
    0x02,                            /* 3 bmAttributes : Bulk, not shared. (field size 1 bytes) */
    0x00,                            /* 4 wMaxPacketSize : 64 bytes per packet. (field size 2 bytes) - has to be 0x200 for compliance*/
    0x02,                            /* 5 wMaxPacketSize */
    0x00,                            /* 6 bInterval : Ignored for Bulk. Set to zero. (field size 1 bytes) */
    0x00,                            /* 7 bRefresh : Unused. (field size 1 bytes) */
    0x00,                            /* 8 bSynchAddress : Unused. (field size 1 bytes) */

    /* Table B-12: MIDI Adapter Class-specific Bulk OUT Endpoint Descriptor */

    0x05,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x25,                            /* 1 bDescriptorType : CS_ENDPOINT descriptor (field size 1 bytes) */
    0x01,                            /* 2 bDescriptorSubtype : MS_GENERAL subtype. (field size 1 bytes) */
    0x01,                            /* 3 bNumEmbMIDIJack : Number of embedded MIDI IN Jacks. (field size 1 bytes) */
    0x01,                            /* 4 BaAssocJackID(1) : ID of the Embedded MIDI IN Jack. (field size 1 bytes) */

    /* Table B-13: MIDI Adapter Standard Bulk IN Endpoint Descriptor */

    0x09,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x05,                            /* 1 bDescriptorType : ENDPOINT descriptor. (field size 1 bytes) */
    _UAC2_INPUT_ENDPOINT_ADDR__MIDI,                  /* 2 bEndpointAddress : IN Endpoint 3. (field size 1 bytes) */
    0x02,                            /* 3 bmAttributes : Bulk, not shared. (field size 1 bytes) */
    0x00,                            /* 4 wMaxPacketSize : 64 bytes per packet. (field size 2 bytes) - has to be 0x200 for compliance*/
    0x02,                            /* 5 wMaxPacketSize */
    0x00,                            /* 6 bInterval : Ignored for Bulk. Set to zero. (field size 1 bytes) */
    0x00,                            /* 7 bRefresh : Unused. (field size 1 bytes) */
    0x00,                            /* 8 bSynchAddress : Unused. (field size 1 bytes) */

    /* Table B-14: MIDI Adapter Class-specific Bulk IN Endpoint Descriptor */

    0x05,                            /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x25,                            /* 1 bDescriptorType : CS_ENDPOINT descriptor (field size 1 bytes) */
    0x01,                            /* 2 bDescriptorSubtype : MS_GENERAL subtype. (field size 1 bytes) */
    0x01,                            /* 3 bNumEmbMIDIJack : Number of embedded MIDI OUT Jacks. (field size 1 bytes) */
    0x03,                            /* 4 BaAssocJackID(1) : ID of the Embedded MIDI OUT Jack. (field size 1 bytes) */

    #if 0
    // USB Bulk-Only Data Interface Descriptor
    0x09,                      /* 0 bLength */
    USB_DESCTYPE_INTERFACE,    /* 1 bDescriptorType */
    _UAC2_USB_INTERFACE_INDEX__BULK, /* 2 bInterfacecNumber */
    0x00,                      /* 3 bAlternateSetting */
    0x02,                      /* 4: bNumEndpoints */
    USB_CLASS_MASS_STORAGE,    /* 5: bInterfaceClass */
    0xFF, // USB_MASS_STORAGE_SUBCLASS, /* 6: bInterfaceSubClass */
    0x00, // USB_MASS_STORAGE_PROTOCOL, /* 7: bInterfaceProtocol */
    0x00,                      /* 8 iInterface */
    // Bulk-In Endpoint Descriptor
    0x07,                       /* 0  bLength */
    USB_DESCTYPE_ENDPOINT,      /* 1  bDescriptorType */
    _UAC2_INPUT_ENDPOINT_ADDR__BULK, /* 2  bEndpointAddress - EP1, IN */
    XUD_EPTYPE_BUL,             /* 3  bmAttributes */
    0x00,                       /* 4  wMaxPacketSize - Low */
    0x02,                       /* 5  wMaxPacketSize - High */
    0x00,                       /* 6  bInterval */
    // Bulk-Out Endpoint Descriptor
    0x07,                       /* 0  bLength */
    USB_DESCTYPE_ENDPOINT,      /* 1  bDescriptorType */
    _UAC2_OUTPUT_ENDPOINT_ADDR__BULK, /* 2  bEndpointAddress - EP1, OUT */
    XUD_EPTYPE_BUL,             /* 3  bmAttributes */
    0x00,                       /* 4  wMaxPacketSize - Low */
    0x02,                       /* 5  wMaxPacketSize - High */
    0x00,                       /* 6  bInterval */
    #endif
};

// Configuration Descriptor for Null device

unsigned char UAC2_cfgDesc_Null[] =
{
    0x09,                           /* 0  bLength */
    USB_DESCTYPE_CONFIGURATION,     /* 1  bDescriptorType */
    0x12,                           /* 2  wTotalLength */
    0x00,                           /* 3  wTotalLength */
    0x01,                           /* 4  bNumInterface: Number of interfaces*/
    0x01,                           /* 5  bConfigurationValue */
    0x00,                           /* 6  iConfiguration */
    #ifdef SELF_POWERED
    192,                            /* 7  bmAttributes */
    #else
    128,
    #endif
    USB_BUS_MAX_POWER,              /* 8  bMaxPower */
    0x09,                           /* 0 bLength : Size of this descriptor, in bytes. (field size 1 bytes) */
    0x04,                           /* 1 bDescriptorType : INTERFACE descriptor. (field size 1 bytes) */
    0x00,                           /* 2 bInterfaceNumber : Index of this interface. (field size 1 bytes) */
    0x00,                           /* 3 bAlternateSetting : Index of this setting. (field size 1 bytes) */
    0x00,                           /* 4 bNumEndpoints : 0 endpoints. (field size 1 bytes) */
    0x00,                           /* 5 bInterfaceClass :  */
    0x00,                           /* 6 bInterfaceSubclass */
    0x00,                           /* 7 bInterfaceProtocol : Unused. (field size 1 bytes) */
    0x00,                           /* 8 iInterface : Unused. (field size 1 bytes) */
    0x09,                           /* 0  bLength */
};

#endif

