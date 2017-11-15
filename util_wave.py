import sys, struct

def _assert( condition, message ):

    if not condition:
        print message
        exit( 0 )

def _parse_wave( wave_file ):

    rate = 0; samples = None
    (group_id,total_size,type_id) = struct.unpack( "<III", wave_file.read(12) )
    _assert( group_id == 0x46464952, "Unknown File Format" ) # Signature for 'RIFF'
    _assert( type_id  == 0x45564157, "Unknown File Format" ) # Signature for 'WAVE'
    while True:
        if total_size <= 8: break
        data = wave_file.read(8)
        if len(data) < 8: break;
        (blockid,blocksz) = struct.unpack( "<II", data )
        #print "WaveIn: BlockID=0x%04X BlockSize=%u" % (blockid,blocksz)
        #print "WaveIn: ByteCount=%u" % (blocksz)
        total_size -= 8
        if blockid == 0x20746D66: # Signature for 'fmt'
            if total_size <= 16: break
            (format,channels,rate,thruput,align,width) = struct.unpack( "<HHIIHH", wave_file.read(16) )
            #print "WaveIn: ByteCount=%u Channels=%u Rate=%u WordSize=%u" % (blocksz,channels,rate,width)
            #print "WaveIn: Format=%u Alignment=%u" % (format,align)
            total_size -= 16
        elif blockid == 0x61746164: # Signature for 'data'
            samples = [0] * (blocksz / (width/8))
            count = 0
            data = wave_file.read( blocksz )
            if channels == 1:
                while len(data) >= width/8:
                    if width == 8:
                        samples[count] = struct.unpack( "b", data[0:1] )[0] * 256 * 256 * 256
                        samples[count]
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
                    count += 1
            total_size -= blocksz
    return (rate,samples)

data = []
count = 99999999

for name in sys.argv[1:]:

    try: file = open( name, "rb" )
    except IOError as err: file = None
    _assert( file != None, "Unable to open file" )
    if file != None:
        (rate,samples) = _parse_wave( file )
        if count > len(samples): count = len(samples)
        data.append( samples )
        file.close()

for row in range(0,count):
    for col in range(0,len(data)):
        #sys.stdout.write( "%08x " % (data[col][row] & 0xFFFFFFFF) )
        val = float(data[col][row]) / (2 ** 31)
        sys.stdout.write( "%+1.8f " % val )
    sys.stdout.write( "\n" )
