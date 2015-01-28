# SCI_echo_back_28335
SCI echo back test program

This program is for TMS320F28335. It uses the SCI A port, GPIO28 set to SCIRXDA and GPIO29 set to SCITXDA.

The data format is hardcoded:

* Baudrate: 115200
* Byte size: 8
* Parity: None
* Stop Bits: 1
* Data Flow Control: None

Frame format is hardcoded as well, two bytes are used as starting bytes, now they are "SS", tailing bytes are "EE". 8 customs bytes are in between. The MCU treats these 8 bytes as 4 16-bits word. Low bytes are transmitted first then the high bytes. So an example frame could be:

```
SS12345678EE
```

When the MCU receives a frame it will echo back what it receives.

## How to change parameters

* Don't forget to chose the building configurations as "flash" not "Debug"

* Frame format can be changed in RS232.c file, the headers and tailing bytes can be changed to any arbitrary characters. data length can be changed as well by `SCI_LENGTH` in the define.

* ENABLE_CRC16 can enable CRC16 which assumes two bytes CRC16 codes will be appended at the end of tailing bytes.

* Baud rate can be changed in RS232.h

```bash
$ tree
.
├── 28335_RAM_lnk.cmd
├── DSP2833x_Headers_nonBIOS.cmd
├── F28335.cmd
├── incl
│   ├── AD.h
│   ├── algorithm.h
│   ├── C28x_FPU_FastRTS.h
│   ├── CommApp.h
│   ├── CRC16.h
│   ├── DA_Driver.h
│   ├── DSP2833x_DefaultIsr.h
│   ├── DSP2833x_Dma_defines.h
│   ├── DSP2833x_EPwm_defines.h
│   ├── DSP2833x_Examples.h
│   ├── DSP2833x_GlobalPrototypes.h
│   ├── DSP2833x_I2c_defines.h
│   ├── DSP28x_Project.h
│   ├── flash.h
│   ├── Flash2833x_API_Config.h
│   ├── Flash2833x_API_Library.h
│   ├── GPIO_Driver.h
│   ├── I2C.h
│   ├── LED_Driver.h
│   ├── PushButtonDriver.h
│   ├── pwm.h
│   ├── RS232.h
│   ├── SysCtrl.h
│   ├── Timer.h
│   └── usDelay.h
├── lib
│   ├── include
│   │   ├── DSP2833x_Adc.h
│   │   ├── DSP2833x_CpuTimers.h
│   │   ├── DSP2833x_DevEmu.h
│   │   ├── DSP2833x_Device.h
│   │   ├── DSP2833x_DMA.h
│   │   ├── DSP2833x_ECan.h
│   │   ├── DSP2833x_ECap.h
│   │   ├── DSP2833x_EPwm.h
│   │   ├── DSP2833x_EQep.h
│   │   ├── DSP2833x_Gpio.h
│   │   ├── DSP2833x_I2c.h
│   │   ├── DSP2833x_Mcbsp.h
│   │   ├── DSP2833x_PieCtrl.h
│   │   ├── DSP2833x_PieVect.h
│   │   ├── DSP2833x_Sci.h
│   │   ├── DSP2833x_Spi.h
│   │   ├── DSP2833x_SysCtrl.h
│   │   ├── DSP2833x_Xintf.h
│   │   └── DSP2833x_XIntrupt.h
│   └── libs
│       ├── include
│       │   ├── C28x_FPU_FastRTS.h
│       │   ├── EvMgr.h
│       │   └── StateMgr.h
│       └── lib
│           ├── CoreFun_DSP2833x.lib
│           └── rts2800_fpu32_fast_supplement.lib
├── LICENSE
├── README.md
├── src
│   ├── DSP2833x_CodeStartBranch.asm
│   ├── DSP2833x_DefaultIsr.c
│   ├── DSP2833x_GlobalVariableDefs.c
│   ├── DSP2833x_MemCopy.c
│   ├── DSP2833x_PieCtrl.c
│   ├── DSP2833x_PieVect.c
│   ├── DSP2833x_SysCtrl.c
│   ├── DSP2833x_usDelay.asm
│   ├── EvMgr.c
│   ├── GPIO_Driver.c
│   ├── main.c
│   ├── RS232.c
│   ├── StateMgr.c
│   └── Timer.c
└── TMS320F28335.ccxml

7 directories, 69 files

```