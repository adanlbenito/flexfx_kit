function flexfx_preamp__create( n )
{
    x = "";
    x += "<div class='row'>";
    x += "<div class='column' style='margin-top:3%'>";
    x += "<h4>FLEXFX - PREAMP</h4>";
    x += "<p>Three-stage tube preamp. Each stage implements a triode preamp model. The three stages are followed by a grapic EQ.</p>";
    x += "</div></div>";

    x += "<table class='flexfx'><thead>";
    x += "<tr><th>Preset</th><th>Stage</th><th>Low-Cut</th><th>Emphasis</th><th>Bias</th><th>Drive</th><th>High-Cut</th></tr>";
    x += "</thead>";
    x += "<tbody id='preampR'>";
    for( p = 1; p <= 9; ++p ) { x += "<tr><td class='preset' id='preampP"+p+"'>"+p+"</td>";
    x += "<td><div style='display:inline-block'><div style='height:18px'>1</div><div style='height:18px'>2</div><div style='height:18px'>3</div></div></td>";
    for( e = 1; e <= 5; ++e ) { x += "<td>";
    x += "<div style='display:inline-block'>";
    for( s = 1; s <= 3; ++s ) {
    x += "<div style='height:18px'><input class='slider' style='width:96px' type='range' min='1' max='9' id='preamp1"+p+""+e+""+s+"'></div>"
    x += "</div>"; } x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";

    x += "<table class='flexfx'><thead>";
    x += "<tr><th>Preset</th><th>60Hz</th><th>120Hz</th><th>240Hz</th><th>480Hz</th>";
    x += "<th>960Hz</th><th>1.9kHz</th><th>3.8kHz</th><th>7.7kHz</th><th>15kHz</th>";
    x += "</thead>";
    x += "<tbody>";
    for( p = 1; p <= 9; ++p ) { x += "<tr><td class='preset' id='preampQ"+p+"'>"+p+"</td>";
    for( e = 1; e <= 9; ++e ) { x += "<td>";
    x += "<div style='height:18px'><input class='slider' style='width:56px' type='range' min='1' max='9' id='grapheq1"+p+""+e+"'></div>";
    x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";

    return x;
}

function flexfx_preamp__initialize( instance_number, midi_port )
{
    n = instance_number;
    for( preset = 1; preset <= 9; ++preset ) {
        $("preampP"+preset).onclick = on_preamp_select;
        $("preampQ"+preset).onclick = on_preamp_select;
        for( param = 1; param <= 5; ++param ) {
            for( stage = 1; stage <= 3; ++stage ) {
                $("preamp1"+preset+""+param+""+stage).oninput = on_preamp_input; }}
    }
}

function on_preamp_select( event )
{
    if( event.target.innerHTML[0] == '[' ) preset = parseInt( event.target.innerHTML[1] );
    else preset = parseInt( event.target.innerHTML );

    parent = $("preampP"+preset).parentNode.parentNode;
    for( i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    parent = $("preampQ"+preset).parentNode.parentNode;
    for( i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    //port.send( flexfx_property_to_midi( property ));
}

function on_preamp_input( event )
{
    unit   = parseInt( event.target.id[6] );
    preset = parseInt( event.target.id[7] );
    stage  = parseInt( event.target.id[8] );
    param  = parseInt( event.target.id[9] );

    port = output_ports["FlexFX Preamp"];
    //port.send( flexfx_property_to_midi( property ));
}

function on_eq_input( event )
{
    unit   = parseInt( event.target.id[6] );
    preset = parseInt( event.target.id[7] );
    stage  = parseInt( event.target.id[8] );
    param  = parseInt( event.target.id[9] );

    port = output_ports["FlexFX Preamp"];
    //port.send( flexfx_property_to_midi( property ));
}
