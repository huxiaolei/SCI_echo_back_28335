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

* Frame format can be changed in RS232.c file, the headers and tailing bytes can be changed to any arbitrary characters. data length can be changed as well by `SCI_LENGTH` in the define.

* ENABLE_CRC16 can enable CRC16 which assumes two bytes CRC16 codes will be appended at the end of tailing bytes.

* Baud rate can be changed in RS232.h