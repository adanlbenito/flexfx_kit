function flexfx_create( tag )
{
    var x = "";

    x += "<p>Three-stage tube preamp. Each stage implements a triode preamp model. The three stages are followed by a grapic EQ.</p>";

    x += "<table class='flexfx'><thead>";
    x += "<tr><th>Preset</th><th>Stage</th><th>Low-Cut</th><th>Emphasis</th><th>Bias</th><th>Drive</th><th>High-Cut</th></tr>";
    x += "</thead>";
    x += "<tbody id='preampR'>";
    for( var p = 1; p <= 9; ++p ) { x += "<tr><td class='preset' id='"+tag+"_presetA"+p+"'>"+p+"</td>";
    x += "<td><div style='display:inline-block'><div style='height:18px'>1</div><div style='height:18px'>2</div><div style='height:18px'>3</div></div></td>";
    for( var e = 1; e <= 5; ++e ) { x += "<td>";
    x += "<div style='display:inline-block'>";
    for( var s = 1; s <= 3; ++s ) {
    x += "<div style='height:18px'><input class='slider' style='width:96px' type='range' min='1' max='9' id='"+tag+"_rangeA"+p+""+e+""+s+"'></div>"
    x += "</div>"; } x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";

    x += "<table class='flexfx'><thead>";
    x += "<tr><th>Preset</th><th>60Hz</th><th>120Hz</th><th>240Hz</th><th>480Hz</th>";
    x += "<th>960Hz</th><th>1.9kHz</th><th>3.8kHz</th><th>7.7kHz</th><th>15kHz</th>";
    x += "</thead>";
    x += "<tbody>";
    for( var p = 1; p <= 9; ++p ) { x += "<tr><td class='preset' id='"+tag+"_presetB"+p+"'>"+p+"</td>";
    for( var e = 1; e <= 9; ++e ) { x += "<td>";
    x += "<div style='height:18px'><input class='slider' style='width:56px' type='range' min='1' max='9' id='"+tag+"_rangeB"+p+""+e+"'></div>";
    x += "</td>"; } x += "</tr>"; }
    x += "</tbody></table>";

    return x;
}

function flexfx_initialize( tag )
{
    for( var preset = 1; preset <= 9; ++preset ) {
        $(tag+"_presetA"+preset).onclick = _on_preamp_select;
        $(tag+"_presetB"+preset).onclick = _on_preamp_select;
        for( var param = 1; param <= 5; ++param ) {
            for( var stage = 1; stage <= 3; ++stage ) {
                $(tag+"_rangeA"+preset+""+param+""+stage).oninput = _on_preamp_input; }}
    }
    return _on_property_received;
}

function _on_factory_input( event )
{
    var tag = flexfx_get_tag( event.target.id );
    file_object = $(tag+"_input").files[0];
    //flexfx_write_firmware( id, file_object, _on_property );
}

function _on_factory_button( event )
{
    var tag = flexfx_get_tag( event.target.id );
    $(tag+"_input").click();
}

function _on_preamp_select( event )
{
    var tag = flexfx_get_tag( event.target.id );

    if( event.target.innerHTML[0] == '[' ) preset = parseInt( event.target.innerHTML[1] );
    else preset = parseInt( event.target.innerHTML );

    parent = $(tag+"_presetA"+preset).parentNode.parentNode;
    for( var i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    parent = $(tag+"_presetB"+preset).parentNode.parentNode;
    for( var i = 1; i <= 9; ++i ) parent.children[i-1].children[0].innerHTML = i;
    parent.children[preset-1].children[0].innerHTML = "[" + preset + "]";

    if( event.target == $(tag+"_presetA"+preset) )
    {
        flexfx_send_property( tag, [12,34,56,78,90,90] );
    }
    if( event.target == $(tag+"_presetB"+preset) )
    {
        //flexfx_send_property( tag, property );
    }
}

function _on_preamp_input( event )
{
    var tag    = flexfx_get_tag( event.target.id );
    var preset = parseInt( event.target.id[(tag+"_range").length+0] );
    var stage  = parseInt( event.target.id[(tag+"_range").length+1] );
    var param  = parseInt( event.target.id[(tag+"_range").length+2] );

    //flexfx_send_property( tag, property );
}

function _on_property_received( property )
{
}

function _on_firmware_status( status )
{
}

