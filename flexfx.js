function flexfx_create( tag )
{
    var x = "";
    x += "<p>";
    x += "This FlexFX device does not have effects firmware loaded into it. Use the ";
    x += "'LOAD FIRMWARE' button to select a firmware image to load into this device.";
    x += "</p>";
    return x;
}

function flexfx_initialize( tag )
{
    return _on_property_received;
}

function _on_property_received( property )
{
}

function _on_firmware_status( status )
{
}

