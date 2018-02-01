function flexfx_preamp__create( n )
{
    x = "";
    x += "<div class='row'>";
    x += "<div class='column' style='margin-top:3%'>";
    x += "<h4>FLEXFX - PREAMP</h4>";
    x += "<p>This index.html page is a placeholder with the CSS, font and favicon. It's just waiting for you to add some content! If you need some help hit up the <a href='http:www.getskeleton.com'>Skeleton documentation</a>.</p>";
    x += "</div></div>";
    x += "<table class='preamp'>";
    x += "<thead>";
    x += "<tr><th>Preset</th><th>Stage</th><th>Low-Cut</th><th>Emphasis</th><th>Bias</th><th>Drive</th><th>High-Cut</th></tr>";
    x += "</thead>";
    x += "<tbody>";
    for( p = 1; p <= 9; ++p ) { x += "<tr> <td>"+p+"</td>";
    x += "<td><div style='display:inline-block'><div style='height:18px'>1</div><div style='height:18px'>2</div><div style='height:18px'>3</div></div></td>";
    for( e = 1; e <= 5; ++e ) { x += "<td>";
    x += "<div style='display:inline-block'>";
    for( s = 1; s <= 3; ++s ) {
    x += "<div style='height:18px'><input class='slider' type='range' min='1' max='9' id='preamp1"+p+""+e+""+s+"'></div>"
    x += "</div>"; } x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";
    x += "<table class='eq'>";
    x += "<thead>";
    x += "<tr><th>Preset</th><th>60Hz</th><th>120Hz</th><th>240Hz</th><th>480Hz</th>";
    x += "<th>960Hz</th><th>1.9kHz</th><th>3.8kHz</th><th>7.7kHz</th><th>15kHz</th>";
    x += "</thead>";
    x += "<tbody>";
    for( p = 1; p <= 9; ++p ) { x += "<tr><td>"+p+"</td>";
    for( e = 1; e <= 9; ++e ) { x += "<td>";
    x += "<div style='height:18px'><input class='slider2' type='range' min='1' max='9' id='grapheq1"+p+""+e+"'></div>";
    x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";

    return x;
}

function flexfx_preamp__initialize( instance_number, midi_port )
{
    n = instance_number;
    for( i = 1; i <= 9; ++i ) {
        for( j = 1; j <= 5; ++j ) {
            for( k = 1; k <= 3; ++k ) {
                $("preamp1"+i+""+j+""+k).oninput = on_preamp_input; }}}
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
