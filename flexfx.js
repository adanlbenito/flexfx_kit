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

function _return_interface()
{
	s = "";
    s += "PROPERTY  DIRECTION        DESCRIPTION";
    s += "0001      Bidirectional    Return 3DEGFLEX, versions and props[4:5] to host";
    s += "0002      Bidirectional    Return the device product name (up to 40 bytes)";
    s += "0003      Bidirectional    Return DSP processing loads (audio cycles)";
    s += "0004      Device to Host   Start full page read (return all properties)";
    s += "0005      Bidirectional    Load/save page buffer from/to FLASH block N";
    s += "0006      Device to Host   (Obsolete) Start a controller javascript code dump";
    s += "0007      Bidirectional    (Obsolete) Next 40 bytes of javascript code text";
    s += "0008      Host to Device   (Obsolete) End of controller javascript code text";
    s += "0009      Bidirectional    Begin firmware upgrade, echoed back to host";
    s += "000A      Bidirectional    Next 32 bytes of firmware image data, echoed";
    s += "000B      Bidirectional    End firmware upgrade, echoed back to host";
    s += "000C      Reserved         Reserved";
    s += "000D      Reserved         Reserved";
    s += "000E      Reserved         Reserved";
    s += "000F      Bidirectional    Return volume,tone,preset control settings";
    s += "8nnn      Bidirectional    Read 5-word property N from the page buffer";
    s += "9nnn      Bidirectional    Write 5-word property N to the page buffer";
    s += "Annn      Bidirectional    Write 5-word property N to app control";
    s += "Bnnn      Bidirectional    Write 5-word property N to control and to buffer";
    s += "Cnnn      Host to Device   5-word property N from the page buffer dump";
	return s;
}
