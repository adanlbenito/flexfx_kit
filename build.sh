#!/bin/bash

# ./build.sh app_cabsim.bin app_cabsim.c flexfx.txt flexfx.js
# ./build.sh efx_cabsim.bin efx_cabsim.c efx_cabsim.txt efx_cabsim.js efx_cabsim.c.o

echo Building strings ...

#xxd -p $1.txt_ | tr -d '\n' > temp.hex
#echo 'const char interface_string[] = "\' > temp.c
#cat temp.hex >> temp.c
#echo "\"; " >> temp.c
#xxd -p $1.js_ | tr -d '\n' > temp.hex
#echo 'const char controller_string[] = "\' >> temp.c
#cat temp.hex >> temp.c
#echo "\";" >> temp.c

python build_strings.py $3 $4 > strings.c

echo Building firmware ...

xcc -report -O3 -lquadflash -fxscope $2 $5 strings.c flexfx.xn flexfx.xc.o flexfx_dsp.c.o \
lib_assert.xc.o lib_gpio.xc.o otp_board_info.xc.o \
usb_audio.xc.o usb_buffers.c.o usb_control.xc.o usb_device.xc.o usb_dfu.xc.o usb_main.xc.o usb_midi.xc.o \
xud_client.xc.o xud_crc5_table.s.o xud_device_attach.xc.o xud_ep_funcs.s.o xud_ep_functions.xc.o xud_get_done.c.o xud_glx.xc.o xud_io_loop.s.o xud_io_loop_call.xc.o xud_manager.xc.o xud_phy_reset_user.xc.o xud_ports.xc.o xud_power_sig.xc.o xud_set_crc_table_addr.c.o xud_set_dev_addr.xc.o xud_setup_chan_override.s.o xud_support.xc.o xud_test_mode.xc.o xud_uifm_pconfig.s.o xud_uifm_reg_access.s.o xud_user.c.o \
-o temp.xe

echo Creating firmware image ...
xflash --noinq --no-compression --factory-version 14.3 --upgrade 1 temp.xe -o $1

rm temp.xe 2> /dev/null
#rm strings.c 2> /dev/null
echo Done.
