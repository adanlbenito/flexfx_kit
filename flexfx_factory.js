function flexfx_factory( tag )
{
    var x = "";
    x += "<div class='row'>";
    x += "<div class='column' style='margin-top:3%'>";
    x += "<h4>FLEXFX</h4>";
    x += "<p>";
    x += "This FlexFX device does not have effects firmware loaded into it. Use the ";
    x += "'LOAD FIRMWARE' button to select a firmware image to load into this device.";
    x += "</p>";
    x += "</div></div>";
    x += "<table class='flexfx'>";
    x += "<tbody><tr>";
    x += "<td><input type='file' style='display:none' id='"+tag+"_input'/><button id='"+tag+"_button'>Load Firmware</button></td>";
    x += "</tr></tbody></table>";
    return { "html":x, "initialize":_initialize };
}

function _initialize( tag )
{
    $(tag+"_button").onclick = _on_button;
    $(tag+"_input").onchange = _on_input;
    return _on_property;
}

function _on_input( event )
{
    var tag  = flexfx_tag_from_id( event.target.id );
    var port = flexfx_port_from_id( event.target.id );
    file_object = $(tag+"_input").files[0];
    //flexfx_write_firmware( port, file_object, _on_property );
}

function _on_button( event )
{
    var tag = flexfx_tag_from_id( event.target.id );
    $(tag+"_input").click();
}

function _on_property( property )
{
}
