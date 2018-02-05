del %1.xe 2> nul

echo Building strings ...
python build_strings.py %1 > strings.c

echo Building firmware ...
xcc -report -O3 -lquadflash -fxscope %1.c %2 %3 %4 flexfx.xn flexfx.xc.o flexfx_dsp.c.o strings.c lib_assert.xc.o lib_gpio.xc.o otp_board_info.xc.o usb_audio.xc.o usb_buffers.c.o usb_control.xc.o usb_device.xc.o usb_dfu.xc.o usb_main.xc.o  usb_midi.xc.o xud_client.xc.o xud_crc5_table.s.o xud_device_attach.xc.o xud_ep_funcs.s.o xud_ep_functions.xc.o xud_get_done.c.o xud_glx.xc.o xud_io_loop.s.o xud_io_loop_call.xc.o xud_manager.xc.o xud_phy_reset_user.xc.o xud_ports.xc.o xud_power_sig.xc.o xud_set_crc_table_addr.c.o xud_set_dev_addr.xc.o xud_setup_chan_override.s.o xud_support.xc.o xud_test_mode.xc.o xud_uifm_pconfig.s.o xud_uifm_reg_access.s.o xud_user.c.o -o %1.xe

echo Creating firmware image ...
xflash --noinq --no-compression --factory-version 14.2 --upgrade 1 %1.xe -o %1.bin

del %1.xe
del string.c
echo Done.
