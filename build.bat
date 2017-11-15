del %1.xe 2> nul

xcc -report -O3 -lquadflash -fxscope %1.c flexfx.xn flexfx.xc.o lib_assert.xc.o lib_gpio.xc.o otp_board_info.xc.o  usb_audio.xc.o usb_buffers.c.o usb_control.xc.o usb_device.xc.o usb_dfu.xc.o usb_main.xc.o  usb_midi.xc.o xud_client.xc.o xud_crc5_table.s.o xud_device_attach.xc.o xud_ep_funcs.s.o xud_ep_functions.xc.o xud_get_done.c.o xud_glx.xc.o xud_io_loop.s.o xud_io_loop_call.xc.o xud_manager.xc.o xud_phy_reset_user.xc.o xud_ports.xc.o xud_power_sig.xc.o xud_set_crc_table_addr.c.o  xud_set_dev_addr.xc.o xud_setup_chan_override.s.o xud_support.xc.o xud_test_mode.xc.o xud_uifm_pconfig.s.o xud_uifm_reg_access.s.o xud_user.c.o lib_dsp_idct.xc lib_dsp_fft.xc lib_dsp_fft_forward.S lib_dsp_fft_inverse.S lib_dsp_fft_merge_spectra.S lib_dsp_fft_split_spectrum.S lib_dsp_long_short_conversion.S lib_dsp_adaptive.c lib_dsp_dct.xc lib_dsp_design.c lib_dsp_filters.c lib_dsp_math.c lib_dsp_matrix.c lib_dsp_statistics.c lib_dsp_tables.c lib_dsp_vector.c -o %1.xe

echo Creating firmware image ...

xflash --factory-version 14.2 --upgrade 1 %1.xe -o %1.bin

rm %1.xe

echo Done.
