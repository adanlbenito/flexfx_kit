function flexfx_create( tag )
{
    var s = "";

    s += "<p>";
    s += "Stereo Cabinet Simulator using impulse responses. Impulse responses to upload must be ";
    s += "stored in a wave file (RIFF/WAV format) and have a sampling frequency of 48 kHz. Both ";
    s += "mono and stereo source data is supported. Stereo can also be employed by specifying two ";
    s += "separate mono WAV files.";
    s += "</p>";
    s += "<table class='flexfx'>";
    s += "<thead>";
    s += "<tr><th>Preset</th><th>Left/Mono</th><th>Right/Stereo</th><th>File Name(s)</th></tr>";
    s += "</thead>";

    s += "<tbody>";
    for( var p = 1; p <= 9; ++p ) {
    s += "<tr><td class='preset' id='"+tag+"_preset"+p+"'>"+p+"</td>";
    s += "<td><input type='file' style='display:none' id='"+tag+"_input"+p+"L'/><button id='"+tag+"_button"+p+"L'>Select IR</button></td>";
    s += "<td><input type='file' style='display:none' id='"+tag+"_input"+p+"R'/><button id='"+tag+"_button"+p+"R'>Select IR</button></td>";
    s += "<td><div style='display:inline-block'>";
    s += "<div id='"+tag+"_text"+p+"L'>Celestion G12H Ann 152 Open Room.wav</div>";
    s += "<div id='"+tag+"_text"+p+"R'>Celestion G12H Ann 152 Open Room.wav</div>";
    s += "</div></td>"; s += "</tr>"; }
    s += "</tbody></table>";

    return s;
}

function flexfx_initialize( tag )
{
    for( var i = 1; i <= 9; ++i )
    {
        $(tag+"_preset"+i).onclick     = _on_cabsim_select;
        $(tag+"_button"+i+"L").onclick = _on_cabsim_button;
        $(tag+"_button"+i+"R").onclick = _on_cabsim_button;
        $(tag+"_input"+i+"L").onchange = _on_cabsim_input;
        $(tag+"_input"+i+"R").onchange = _on_cabsim_input;
    }
    return _on_property_received;
}

function _on_cabsim_select( event )
{
    var tag = flexfx_get_tag( event.target.id );

    if( event.target.innerHTML[0] == '[' ) preset = parseInt( event.target.innerHTML[1] );
    else preset = parseInt( event.target.innerHTML );

    parent = $(tag+"_preset"+preset).parentNode.parentNode;
    for( var i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    //flexfx_send_property( tag, property );
}

function _on_cabsim_input( event )
{
    var tag    = flexfx_get_tag( event.target.id );
    var unit   = parseInt( event.target.id[(tag+"_input").length+0] );
    var preset = parseInt( event.target.id[(tag+"_input").length+1] );
    var side   =           event.target.id[(tag+"_input").length+2];
    var file   = $(tag+"_input"+unit+""+preset+side).files[0];

    $(tag+"_text"+unit+""+preset+side).textContent = file.name;

    var reader = new FileReader();
    reader.onload = function(e)
    {
        var samples = flexfx_wave_to_samples( new Uint8Array( reader.result ));
        console.log( samples.length );

        var offset = 0;
        while( offset < 1200 ) {
            if( samples.length >= 4 ) {
                var property = [ 0x01018000+offset/5, samples[0],samples[1],samples[2],samples[3],samples[4] ];
                samples = samples.slice( 5 );
                flexfx_property = [0,0,0,0,0,0];
                flexfx_send_property( tag, property );
            }
            while( 1 ) { if( flexfx_property[0] == 0x01018000+offset/5 ) break; }
            offset += 5;
        }
    }
    reader.readAsArrayBuffer( file );
}

function _on_cabsim_button( event )
{
    var tag    = flexfx_get_tag( event.target.id );
    var preset = parseInt( event.target.id[(tag+"_button").length+0] );
    var side   =           event.target.id[(tag+"_button").length+1];
    $(tag+"_input"+preset+""+side).click();
}

function _on_property_received( property )
{
}

function _on_firmware_status( status )
{
}

function _return_interface()
{
	s = "";
    s += "FLEXFX STEREO CABSIM Property Interface";
	s += "";
    s += "Stereo Cabinet Simulator using impulse responses. Impulse responses to upload";
    s += "must be stored in a wave file (RIFF/WAV format) and have a sampling frequency";
    s += "of 48 kHz. Both mono and stereo source data is supported.  Stereo can also be";
    s += "employed by specifying two mono WAV files.";
	s += "";
    s += "PROP      DESCRIPTION";
    s += "1000      Volume,tone,preset control settings";
    s += "1n01      Up to 20 charactr name for preset N (1<=N<=9)";
    s += "1n03      Five IR data words for preset N or echoed data";
    s += "1n04      Bidirectional    End data upload for preset N or end upload ACK";
    s += "1n05      Bidirectional    First 20 chars of data file name for preset N";
    s += "1n06      Bidirectional    Next 20 chars of data file name for preset N";
    s += "1n07      Bidirectional    Last 20 chars of data file name for preset N";
	s += "";
    s += "Property layout for control (knobs, pushbuttons, etc) Values shown are 32-bit";
    s += "values represented in ASCII/HEX format or as floating point values ranging";
    s += "from +0.0 up to (not including) +1.0.";
	s += "";
    s += "+------- Effect parameter identifier (Property ID)";
    s += "|";
    s += "|    +-------------------------------- Volume level";
    s += "|    |     +-------------------------- Tone setting";
    s += "|    |     |     +-------------------- Reserved";
    s += "|    |     |     |     +-------------- Reserved";
    s += "|    |     |     |     |     +-------- Preset selection (1 through 9)";
    s += "|    |     |     |     |     |+------- Enabled (1=yes,0=bypassed)";
    s += "|    |     |     |     |     ||+------ InputL  (1=plugged,0=unplugged)";
    s += "|    |     |     |     |     |||+----- OutputL (1=plugged,0=unplugged)";
    s += "|    |     |     |     |     ||||+---- InputR  (1=plugged,0=unplugged)";
    s += "|    |     |     |     |     |||||+--- OutputR (1=plugged,0=unplugged)";
    s += "|    |     |     |     |     ||||||+-- Expression (1=plugged,0=unplugged)";
    s += "|    |     |     |     |     |||||||+- USB Audio (1=active)";
    s += "|    |     |     |     |     ||||||||";
    s += "1001 0.500 0.500 0.500 0.500 91111111";
	s += "";
    s += "Property layout for preset data loading (loading IR data). Values shown are";
    s += "32-bit values represented in ASCII/HEX format.";
	s += "";
    s += "+---------- Effect parameter identifier (Property ID)";
    s += "|";
    s += "|+--- Preset number (1 through 9)";
    s += "||";
    s += "1n02 0 0 0 0 0     Begin IR data loading for preset N";
    s += "1n03 A B C D E     Five of the next IR data words to load into preset N";
    s += "1n04 0 0 0 0 0     End IR data loading for preset N";
	return s;
}

