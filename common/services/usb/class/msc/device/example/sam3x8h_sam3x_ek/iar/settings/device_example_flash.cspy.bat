@REM This batch file has been generated by the IAR Embedded Workbench
@REM C-SPY Debugger, as an aid to preparing a command line for running
@REM the cspybat command line utility using the appropriate settings.
@REM
@REM Note that this file is generated every time a new debug session
@REM is initialized, so you may want to move or rename the file before
@REM making changes.
@REM
@REM You can launch cspybat by typing the name of this batch file followed
@REM by the name of the debug file (usually an ELF/DWARF or UBROF file).
@REM
@REM Read about available command line parameters in the C-SPY Debugging
@REM Guide. Hints about additional command line parameters that may be
@REM useful in specific cases:
@REM   --download_only   Downloads a code image without starting a debug
@REM                     session afterwards.
@REM   --silent          Omits the sign-on message.
@REM   --timeout         Limits the maximum allowed execution time.
@REM 


"C:\Program Files\IAR Systems\Embedded Workbench 6.4\common\bin\cspybat" "C:\Program Files\IAR Systems\Embedded Workbench 6.4\arm\bin\armproc.dll" "C:\Program Files\IAR Systems\Embedded Workbench 6.4\arm\bin\armjlink.dll"  %1 --plugin "C:\Program Files\IAR Systems\Embedded Workbench 6.4\arm\bin\armbat.dll" --macro "E:\_PY\_work\_GPS\asf-3.7.3\common\services\usb\class\msc\device\example\sam3x8h_sam3x_ek\iar\../../../../../../../../../sam/boards/sam3x_ek/debug_scripts/iar/sam3x_ek_flash.mac" --flash_loader "C:\Program Files\IAR Systems\Embedded Workbench 6.4\arm\config\flashloader\Atmel\SAM3X8\sam3x8-flash.board" --backend -B "--endian=little" "--cpu=Cortex-M3" "--fpu=None" "-p" "C:\Program Files\IAR Systems\Embedded Workbench 6.4\arm\CONFIG\debugger\Atmel\iosam3x8h.svd" "--drv_verify_download" "--semihosting=none" "--device=SAM3X8" "--drv_communication=USB0" "--jlink_speed=adaptive" "--jlink_reset_strategy=0,0" "--drv_catch_exceptions=0x000" "--drv_swo_clock_setup=72000000,0,2000000" 


