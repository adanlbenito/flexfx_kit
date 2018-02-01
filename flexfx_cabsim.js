function flexfx_cabsim__create( instance_number )
{
    n = instance_number;
    x = "";
    x += "<div class='row'>";
    x += "<div class='column' style='margin-top:3%'>";
    x += "<h4>FLEXFX - CABSIM</h4>";
    x += "<p>";
    x += "Stereo Cabinet Simulator using impulse responses. Impulse responses to upload must be ";
    x += "stored in a wave file (RIFF/WAV format) and have a sampling frequency of 48 kHz. Both ";
    x += "mono and stereo source data is supported. Stereo can also be employed by specifying two ";
    x += "separate mono WAV files.";
    x += "</p>";
    x += "</div></div>";
    x += "<table class='cabsim'>";
    x += "<thead>";
    x += "<tr><th>Left/Mono</th><th>Right/Stereo</th><th>File Name(s)</th></tr>";
    x += "</thead>";
    x += "<tbody>";
    for( p = 1; p <= 9; ++p ) { x += " <tr>";
    x += "<td><input type='file' style='display:none' id='flexfx_cabsimI1"+p+"L'/><button id='flexfx_cabsimB1"+p+"L'>Preset "+p+" (L)</button></td>";
    x += "<td><input type='file' style='display:none' id='flexfx_cabsimI1"+p+"R'/><button id='flexfx_cabsimB1"+p+"R'>Preset "+p+" (R)</button></td>";
    x += "<td><div style='display:inline-block'>";
    x += "<div id='flexfx_cabsimT1"+p+"L'>G12H Ann 152 O Hi-Gn All+Room Stereo Celestion.wav</div>";
    x += "<div id='flexfx_cabsimT1"+p+"R'>G12H Ann 152 O Hi-Gn All+Room Stereo Celestion.wav</div>";
    x += "</div></td>"; x += "</tr>"; }
    x += "</tbody></table>";
    return x;
}

function flexfx_cabsim__initialize( instance_number, midi_port )
{
    n = instance_number;
    for( i = 1; i <= 9; ++i ) $("flexfx_cabsimB1"+i+"L").onclick  = _flexfx_cabsim__on_button;
    for( i = 1; i <= 9; ++i ) $("flexfx_cabsimB1"+i+"R").onclick  = _flexfx_cabsim__on_button;
    for( i = 1; i <= 9; ++i ) $("flexfx_cabsimI1"+i+"L").onchange = _flexfx_cabsim__on_input;
    for( i = 1; i <= 9; ++i ) $("flexfx_cabsimI1"+i+"R").onchange = _flexfx_cabsim__on_input;
}

function _flexfx_cabsim__on_input( event )
{
    unit   = parseInt( event.target.id["flexfx_cabsimI".length+0] );
    preset = parseInt( event.target.id["flexfx_cabsimI".length+1] );
    side   = event.target.id[9];

    file   = $("flexfx_cabsimI"+unit+""+preset+side).files[0];
    $("flexfx_cabsimT"+unit+""+preset+side).textContent = file.name;

    var reader = new FileReader();
    reader.onload = function(e)
    {
        samples = wave_to_samples( new Uint8Array( reader.result ));
        console.log( samples.length );
        //port = output_ports["FlexFX Cabsim"];
        port = output_ports["Example"];

        offset = 0;
        while( offset < 1200 ) {
            if( samples.length >= 4 ) {
                property = [ 0x01018000+offset/5, samples[0],samples[1],samples[2],samples[3],samples[4] ];
                samples = samples.slice( 5 );
                flexfx_property = [0,0,0,0,0,0];
                port.send( flexfx_property_to_midi( property ));
            }
            while( 1 ) { if( flexfx_property[0] == 0x01018000+offset/5 ) break; }
            offset += 5;
        }
    }
    reader.readAsArrayBuffer( file );
}

function _flexfx_cabsim__on_button( event )
{
    unit   = parseInt( event.target.id["flexfx_cabsimI".length+0] );
    preset = parseInt( event.target.id["flexfx_cabsimI".length+1] );
    side   =           event.target.id["flexfx_cabsimI".length+2];
    $("flexfx_cabsimI"+unit+""+preset+side).click();
}

function _flexfx_cabsim__wave_to_samples( data )
{
    var format, channels, rate, thruput, align, width;
    var samples = new Array();
    rate = 0; samples = [];
    group_id   = array_to_ui4le(data); data = data.slice(4); // Signature for 'RIFF'
    total_size = array_to_ui4le(data); data = data.slice(4);
    type_id    = array_to_ui4le(data); data = data.slice(4); // Signature for 'WAVE'

    while( 1 )
    {
        if( data.length < 8 ) break;
        blockid = array_to_ui4le(data); data = data.slice(4);
        blocksz = array_to_ui4le(data); data = data.slice(4);

        if( blockid == 0x20746D66) // Signature for 'fmt'
        {
            if( data.length < 16 ) break;
            format   = array_to_ui2le(data); data = data.slice(2);
            channels = array_to_ui2le(data); data = data.slice(2);
            rate     = array_to_ui4le(data); data = data.slice(4);
            thruput  = array_to_ui4le(data); data = data.slice(4);
            align    = array_to_ui2le(data); data = data.slice(2);
            width    = array_to_ui2le(data); data = data.slice(2);
            console.log( "WaveIn: ByteCount=%u Channels=%u Rate=%u WordSize=%u", blocksz,channels,rate,width );
        }
        else if( blockid == 0x61746164 ) // Signature for 'data'
        {
            if( data.length < blocksz ) break;
            count = blocksz / (width/8);

            if( channels == 1 ) {
                while( count-- > 0  ) {
                    if( width == 8 ) {
                        samples.push( data[0]*256*256*256 );
                        data = data.slice(1);
                    }
                    if( width == 16 ) {
                        samples.push( data[1]*256*256*256 + data[0]*256*256 );
                        data = data.slice(2);
                    }
                    if( width == 24 ) {
                        samples.push( data[2]*256*256*256 + data[1]*256*256 + data[0]*256 );
                        data = data.slice(3);
                    }
                    if( width == 32 ) {
                        samples.push( data[3]*256*256*256 + data[2]*256*256 + data[1]*256 + data[0] );
                        value += data[0];
                        data = data.slice(4);
                    }
                }
            }
        }
    }
    return samples;
}
