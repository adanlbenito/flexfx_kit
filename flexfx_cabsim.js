function flexfx_cabsim( tag )
{
    var x = "";

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
    x += "<table class='flexfx'>";
    x += "<thead>";
    x += "<tr><th>Preset</th><th>Left/Mono</th><th>Right/Stereo</th><th>File Name(s)</th></tr>";
    x += "</thead>";

    x += "<tbody>";
    for( var p = 1; p <= 9; ++p ) {
    x += "<tr><td class='preset' id='"+tag+"_preset"+p+"'>"+p+"</td>";
    x += "<td><input type='file' style='display:none' id='"+tag+"_input"+p+"L'/><button id='"+tag+"_button"+p+"L'>Select IR</button></td>";
    x += "<td><input type='file' style='display:none' id='"+tag+"_input"+p+"R'/><button id='"+tag+"_button"+p+"R'>Select IR</button></td>";
    x += "<td><div style='display:inline-block'>";
    x += "<div id='"+tag+"_text"+p+"L'>Celestion G12H Ann 152 Open Room.wav</div>";
    x += "<div id='"+tag+"_text"+p+"R'>Celestion G12H Ann 152 Open Room.wav</div>";
    x += "</div></td>"; x += "</tr>"; }
    x += "</tbody></table>";

    return { "html":x, "initialize":_initialize };
}

function _initialize( tag )
{
    for( var i = 1; i <= 9; ++i )
    {
        $(tag+"_preset"+i).onclick = on_cabsim_select;

        $(tag+"_button"+i+"L").onclick = _on_button;
        $(tag+"_button"+i+"R").onclick = _on_button;
        $(tag+"_input"+i+"L").onchange = _on_input;
        $(tag+"_input"+i+"R").onchange = _on_input;
    }
    return _on_property_received;
}

function on_cabsim_select( event )
{
    var tag  = flexfx_tag_from_id( event.target.id );
    var port = flexfx_port_from_id( event.target.id );

    if( event.target.innerHTML[0] == '[' ) preset = parseInt( event.target.innerHTML[1] );
    else preset = parseInt( event.target.innerHTML );

    parent = $(tag+"_preset"+preset).parentNode.parentNode;
    for( var i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    //flexfx_send_property( port, property );
}

function _on_input( event )
{
    var tag    = flexfx_tag_from_id( event.target.id );
    var port   = flexfx_port_from_id( event.target.id );
    var unit   = parseInt( event.target.id[tag+"_input".length+0] );
    var preset = parseInt( event.target.id[tag+"_input".length+1] );
    var side   = event.target.id[9];
    var file   = $(tag+"_input"+unit+""+preset+side).files[0];

    $(tag+"_text"+unit+""+preset+side).textContent = file.name;

    var reader = new FileReader();
    reader.onload = function(e)
    {
        var samples = flexfx_wave_to_samples( new Uint8Array( reader.result ));
        console.log( samples.length );
        //var tag = output_ports["FlexFX Cabsim"];
        var tag = output_ports["Example"];

        var offset = 0;
        while( offset < 1200 ) {
            if( samples.length >= 4 ) {
                var property = [ 0x01018000+offset/5, samples[0],samples[1],samples[2],samples[3],samples[4] ];
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

function _on_button( event )
{
    var tag    = flexfx_tag_from_id( event.target.id );
    var preset = parseInt( event.target.id[(tag+"_button").length+0] );
    var side   = event.target.id[(tag+"_button").length+1];
    $(tag+"_input"+preset+""+side).click();
}

function _on_property_received( property )
{
}

function _on_firmware_status( status )
{
}
