function flexfx_factory__create( instance_number )
{
    n = instance_number;
    x = "";
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
    x += "<td><input type='file' style='display:none' id='flexfx_factoryI'/><button id='flexfx_factoryB'>Load Firmware</button></td>";
    x += "</tr></tbody></table>";
    return x;
}

function flexfx_factory__initialize( instance_number, midi_port )
{
    n = instance_number;
    $("flexfx_factoryB").onclick  = _flexfx_factory__on_button;
    $("flexfx_factoryI").onchange = _flexfx_factory__on_input;
}

function _flexfx_factory__on_input( event )
{
    file_object = $("flexfx_factoryI").files[0];
    flexfx_load_firmware( file_object, _load_firmware_status );
}

function _flexfx_factory__on_button( event )
{
    $("flexfx_factoryI").click();
}

function _load_firmware_status( event )
{
}
