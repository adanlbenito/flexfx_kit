import sys, time, struct, rtmidi

def midi_list():

    midiout = rtmidi.MidiOut()
    midiin  = rtmidi.MidiIn ()
    sys.stdout.write( "MIDI Output Devices:" )
    index = 0
    for device in midiout.get_ports():
        sys.stdout.write( " %u=\'%s\'" % (index,device) )
    sys.stdout.write( "\nMIDI Input Devices: " )
    for device in midiin.get_ports():
        sys.stdout.write( " %u=\'%s\'" % (index,device) )
    sys.stdout.write( "\n" )

def midi_open( port_number ):

    midiout = rtmidi.MidiOut()
    midiin  = rtmidi.MidiIn ()
    if midiout == None and midiin == None: return
    midiin.ignore_types( sysex = False, timing = False, active_sense = True )
    midiout.open_port( port_number )
    midiin.open_port ( port_number )
    return (midiout, midiin)

def midi_read( midi_device ): return midi_device[1].get_message()

def midi_write( midi_device, midi_sysex_data ):

    midi_device[0].send_message( midi_sysex_data )

def midi_wait( midi_device ):

    while( True ):
        data = midi_read( midi_device )
        if data == None: continue
        data = data[0]
        #if data[0] != 240: continue
        break;
    return data

def midi_close( midi_device ): a = 1

if len(sys.argv) < 2: # Usage 1 - Show help message

    print "Usage 1: python flexfx.py"
    print "         Show this message and list MIDI device names and port numbers."
    print ""
    print "Usage 2: python flexfx.py <midi_port>"
    print "         List MIDI device names and port numbers. Indefinately wait for FlexFX"
    print "         MIDI properties to arrive and print them out. Use <ctrl-c> to terminate."
    print ""
    print "Usage 3: python flexfx.py <midi_port> <firmware_image>.bin"
    print "         Burn a FlexFX firmware image into FLASH memory. The firmware image must"
    print "         have a filename extension of .bin"
    print ""
    print "Usage 4: python flexfx.py <midi_port> <data_file>.dat <offset>"
    print "         Burn application data into FLASH memory starting at <offset>. The firmware"
    print "         image must have a filename extension of .dat"
    print ""
    print "Usage 5: python flexfx.py <midi_port> <ir_data.wav>"
    print "         Load WAVE-file formatted impulse response data into DSP RAM for the"
    print "         cabinet simulation firmware example."
    print ""
    print "Usage 6: python flexfx.py <midi_port> <properties_file>.txt"
    print "         Load FlexFX properties contained in text file to device via USB MIDI."
    print "         Each property consists of a 32-bit ID and five 32-bit values. The text"
    print "         file contains one property per line with property data rendered as"
    print "         ASCII/HEX (e.g. FFFF0000 11111111 22222222 33333333 44444444 55555555)"
    print ""
    print "Usage 7: python flexfx.py <midi_port> <prop_id> <prop_values ...>"
    print "         Write one FlexFX property to the FlexFX device. Each property consists of"
    print "         a 32-bit ID and five 32-bit values. The <prop_id> and <prop_values>"
    print "         represent one property per line with property data rendered as ASCII/HEX"
    print "         (e.g. FFFF0000 11111111 22222222 33333333 44444444 55555555)"
    print ""

    midi_list()
    exit(0)

def property_to_midi_sysex( property ):

    midi_data = [0xF0]

    midi_data.append( (property[0] >> 28) & 15 )
    midi_data.append( (property[0] >> 24) & 15 )
    midi_data.append( (property[0] >> 20) & 15 )
    midi_data.append( (property[0] >> 16) & 15 )
    midi_data.append( (property[0] >> 12) & 15 )
    midi_data.append( (property[0] >>  8) & 15 )
    midi_data.append( (property[0] >>  4) & 15 )
    midi_data.append( (property[0] >>  0) & 15 )

    midi_data.append( (property[1] >> 28) & 15 )
    midi_data.append( (property[1] >> 24) & 15 )
    midi_data.append( (property[1] >> 20) & 15 )
    midi_data.append( (property[1] >> 16) & 15 )
    midi_data.append( (property[1] >> 12) & 15 )
    midi_data.append( (property[1] >>  8) & 15 )
    midi_data.append( (property[1] >>  4) & 15 )
    midi_data.append( (property[1] >>  0) & 15 )

    midi_data.append( (property[2] >> 28) & 15 )
    midi_data.append( (property[2] >> 24) & 15 )
    midi_data.append( (property[2] >> 20) & 15 )
    midi_data.append( (property[2] >> 16) & 15 )
    midi_data.append( (property[2] >> 12) & 15 )
    midi_data.append( (property[2] >>  8) & 15 )
    midi_data.append( (property[2] >>  4) & 15 )
    midi_data.append( (property[2] >>  0) & 15 )

    midi_data.append( (property[3] >> 28) & 15 )
    midi_data.append( (property[3] >> 24) & 15 )
    midi_data.append( (property[3] >> 20) & 15 )
    midi_data.append( (property[3] >> 16) & 15 )
    midi_data.append( (property[3] >> 12) & 15 )
    midi_data.append( (property[3] >>  8) & 15 )
    midi_data.append( (property[3] >>  4) & 15 )
    midi_data.append( (property[3] >>  0) & 15 )

    midi_data.append( (property[4] >> 28) & 15 )
    midi_data.append( (property[4] >> 24) & 15 )
    midi_data.append( (property[4] >> 20) & 15 )
    midi_data.append( (property[4] >> 16) & 15 )
    midi_data.append( (property[4] >> 12) & 15 )
    midi_data.append( (property[4] >>  8) & 15 )
    midi_data.append( (property[4] >>  4) & 15 )
    midi_data.append( (property[4] >>  0) & 15 )

    midi_data.append( (property[5] >> 28) & 15 )
    midi_data.append( (property[5] >> 24) & 15 )
    midi_data.append( (property[5] >> 20) & 15 )
    midi_data.append( (property[5] >> 16) & 15 )
    midi_data.append( (property[5] >> 12) & 15 )
    midi_data.append( (property[5] >>  8) & 15 )
    midi_data.append( (property[5] >>  4) & 15 )
    midi_data.append( (property[5] >>  0) & 15 )

    midi_data.append( 0xF7 )
    return midi_data

def midi_sysex_to_property( midi_data ):

    property = [0,0,0,0,0,0]

    property[0] = (midi_data[ 1] << 28) + (midi_data[ 2] << 24) \
                + (midi_data[ 3] << 20) + (midi_data[ 4] << 16) \
                + (midi_data[ 5] << 12) + (midi_data[ 6] <<  8) \
                + (midi_data[ 7] <<  4) + (midi_data[ 8] <<  0)

    property[1] = (midi_data[ 9] << 28) + (midi_data[10] << 24) \
                + (midi_data[11] << 20) + (midi_data[12] << 16) \
                + (midi_data[13] << 12) + (midi_data[14] <<  8) \
                + (midi_data[15] <<  4) + (midi_data[16] <<  0)

    property[2] = (midi_data[17] << 28) + (midi_data[18] << 24) \
                + (midi_data[19] << 20) + (midi_data[20] << 16) \
                + (midi_data[21] << 12) + (midi_data[22] <<  8) \
                + (midi_data[23] <<  4) + (midi_data[24] <<  0)

    property[3] = (midi_data[25] << 28) + (midi_data[26] << 24) \
                + (midi_data[27] << 20) + (midi_data[28] << 16) \
                + (midi_data[29] << 12) + (midi_data[30] <<  8) \
                + (midi_data[31] <<  4) + (midi_data[32] <<  0)

    property[4] = (midi_data[33] << 28) + (midi_data[34] << 24) \
                + (midi_data[35] << 20) + (midi_data[36] << 16) \
                + (midi_data[37] << 12) + (midi_data[38] <<  8) \
                + (midi_data[39] <<  4) + (midi_data[40] <<  0)

    property[5] = (midi_data[41] << 28) + (midi_data[42] << 24) \
                + (midi_data[43] << 20) + (midi_data[44] << 16) \
                + (midi_data[45] << 12) + (midi_data[46] <<  8) \
                + (midi_data[47] <<  4) + (midi_data[48] <<  0)

    return property

def _assert( condition, message ):

    if not condition:
        print message
        exit( 0 )

def _parse_wave( file ):

    rate = 0; samples = None
    (group_id,total_size,type_id) = struct.unpack( "<III", file.read(12) )
    _assert( group_id == 0x46464952, "Unknown File Format" ) # Signature for 'RIFF'
    _assert( type_id  == 0x45564157, "Unknown File Format" ) # Signature for 'WAVE'
    while True:
        if total_size <= 8: break
        data = file.read(8)
        if len(data) < 8: break;
        (blockid,blocksz) = struct.unpack( "<II", data )
        #print "WaveIn: BlockID=0x%04X BlockSize=%u" % (blockid,blocksz)
        #print "WaveIn: ByteCount=%u" % (blocksz)
        total_size -= 8
        if blockid == 0x20746D66: # Signature for 'fmt'
            if total_size <= 16: break
            (format,channels,rate,thruput,align,width) = struct.unpack( "<HHIIHH", file.read(16) )
            #print "WaveIn: ByteCount=%u Channels=%u Rate=%u WordSize=%u" % (blocksz,channels,rate,width)
            #print "WaveIn: Format=%u Alignment=%u" % (format,align)
            total_size -= 16
        elif blockid == 0x61746164: # Signature for 'data'
            samples = [0] * (blocksz / (width/8))
            count = 0
            data = file.read( blocksz )
            if channels == 1:
                while len(data) >= width/8:
                    if width == 8:
                        samples[count]  = struct.unpack( "b", data[0:1] )[0] * 256 * 256 * 256
                        data = data[1:]
                    if width == 16:
                        samples[count]  = struct.unpack( "b", data[1:2] )[0] * 256 * 256 * 256
                        samples[count] += struct.unpack( "B", data[0:1] )[0] * 256 * 256
                        data = data[2:]
                    if width == 24:
                        samples[count]  = struct.unpack( "b", data[2:3] )[0] * 256 * 256 * 256
                        samples[count] += struct.unpack( "B", data[1:2] )[0] * 256 * 256
                        samples[count] += struct.unpack( "B", data[0:1] )[0] * 256
                        data = data[3:]
                    if width == 32:
                        samples[count]  = struct.unpack( "b", data[3:4] )[0] * 256 * 256 * 256
                        samples[count] += struct.unpack( "B", data[2:3] )[0] * 256 * 256
                        samples[count] += struct.unpack( "B", data[1:2] )[0] * 256
                        samples[count] += struct.unpack( "B", data[0:1] )[0]
                        data = data[4:]
                    #print float(samples[count]) / (2 ** 31)
                    count += 1
            total_size -= blocksz
    return samples

if len(sys.argv) < 3: # Usage 2 - Wait for properties and print them to STDOUT
    midi = midi_open( int(sys.argv[1]) )
    while True:
        prop = midi_sysex_to_property( midi_wait( midi ));
        p1 = float(prop[1]) / 2**28
        p2 = float(prop[2]) / 2**28
        p3 = float(prop[3]) / 2**28
        p4 = float(prop[4]) / 2**28
        p5 = float(prop[5]) / 2**28
        if prop[1] & 0x80000000: p1 = -float((~prop[1] + 1) & 0xFFFFFFFF) / (2**28)
        if prop[2] & 0x80000000: p2 = -float((~prop[2] + 1) & 0xFFFFFFFF) / (2**28)
        if prop[3] & 0x80000000: p3 = -float((~prop[3] + 1) & 0xFFFFFFFF) / (2**28)
        if prop[4] & 0x80000000: p4 = -float((~prop[4] + 1) & 0xFFFFFFFF) / (2**28)
        if prop[5] & 0x80000000: p5 = -float((~prop[5] + 1) & 0xFFFFFFFF) / (2**28)
        print "%08x  %08x %08x %08x %08x %08x  %+01.5f %+01.5f %+01.5f %+01.5f %+01.5f" % \
              (prop[0],prop[1],prop[2],prop[3],prop[4],prop[5], p1,p2,p3,p4,p5 )

name = sys.argv[2]

if name[len(name)-4:] == ".bin": # Usage 3 - Burn firmware image to FLASH boot partition

    midi = midi_open( int(sys.argv[1]) )
    file = open( sys.argv[2], "rb" )
    count = 0
    data = property_to_midi_sysex( [1,0,0,0,0,0] )
    midi_write( midi, data )
    time.sleep( 5 )
    while True:
        line = file.read( 16 )
        if len(line) == 0: break
        while len(line) < 16: line += chr(0)
        data = [ 2, (ord(line[ 0])<<24)+(ord(line[ 1])<<16)+(ord(line[ 2])<<8)+ord(line[ 3]), \
                    (ord(line[ 4])<<24)+(ord(line[ 5])<<16)+(ord(line[ 6])<<8)+ord(line[ 7]), \
                    (ord(line[ 8])<<24)+(ord(line[ 9])<<16)+(ord(line[10])<<8)+ord(line[11]), \
                    (ord(line[12])<<24)+(ord(line[13])<<16)+(ord(line[14])<<8)+ord(line[15]), \
                    0, 0, 0, 0 ]
        midi_write( midi, property_to_midi_sysex( data ))
        #prop = midi_sysex_to_property( midi_wait( midi ));
        if count == 0 or (count % 16) == 0:
            sys.stdout.write(".")
            sys.stdout.flush()
            time.sleep( 0.01 )
        time.sleep( 0.001 )
        count += 1
    data = property_to_midi_sysex( [3,0,0,0,0,0] )
    midi_write( midi, data )
    file.close()
    print( "Done." )

elif name[len(name)-4:] == ".dat": # Usage 4 - Burn raw data file info FLASH data partition

    midi = midi_open( int(sys.argv[1]) )
    file = open( sys.argv[2], "rb" )
    file.close()

elif name[len(name)-4:] == ".wav": # Usage 5 - Load IR data (WAVE file) to DSP RAM

    midi = midi_open( int(sys.argv[1]) )
    file = open( sys.argv[2], "rb" )
    samplesL = _parse_wave( file )
    #_assert( rate == 48000, "Sample rate must be 48000" )
    _assert( len(samplesL) > 0, "No sample data exists" )
    samplesR = list(samplesL)

    offset = 0
    while len(samplesL) > 0 and offset < 1440:

        block = samplesL[0:5]
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        samplesL = samplesL[5:]
        data = [ 0x01018000 + offset/5, block[0]/8,block[1]/8,block[2]/8,block[3]/8,block[4]/8 ]
        midi_write( midi, property_to_midi_sysex( data ))
        while True:
            prop = midi_sysex_to_property( midi_wait( midi ))
            if prop[0] == 0x01018000 + offset/5: break
        sys.stdout.flush()
        offset += 5

    while len(samplesR) > 0 and offset < 2880:

        block = samplesR[0:5]
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        if len(block) < 5: block.append(0)
        samplesR = samplesR[5:]
        data = [ 0x01018000 + offset/5, block[0]/8,block[1]/8,block[2]/8,block[3]/8,block[4]/8 ]
        midi_write( midi, property_to_midi_sysex( data ))
        while True:
            prop = midi_sysex_to_property( midi_wait( midi ))
            if prop[0] == 0x01018000 + offset/5: break
        sys.stdout.flush()
        offset += 5

    file.close()
    sys.stdout.write("Done.\n")

elif name[len(name)-4:] == ".txt": # Usage 6

    #midi = midi_open( int(sys.argv[1]) )
    file = open( sys.argv[2], "rt" )
    while True:
        line = file.readline().split()
        if len(line) < 6: exit(0)
        prop = [int(line[0],16),int(line[1],16),int(line[2],16),int(line[3],16), \
                int(line[4],16),int(line[5],16)]
        print "%08x %08x %08x %08x %08x %08x" % (prop[0],prop[1],prop[2],prop[3],prop[4],prop[5])
    file.close()

elif len(sys.argv) == 8: # Usage 7

    prop = [int(sys.argv[2],16),int(sys.argv[3],16),int(sys.argv[4],16),int(sys.argv[5],16), \
            int(sys.argv[6],16),int(sys.argv[7],16)]
    print "%08x %08x %08x %08x %08x %08x" % (prop[0],prop[1],prop[2],prop[3],prop[4],prop[5])
