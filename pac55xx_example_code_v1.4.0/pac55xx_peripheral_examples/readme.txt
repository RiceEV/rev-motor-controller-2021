The PAC55xx Peripheral Example contains source code and projects for IAR, Keil MDK (uVision), and Eclipse IDEs.

Make sure to download the appropriate PAC55xx support package for the desired IDE using a myActive login from the following
link:
https://active-semi.com/products/intelligent-motor-control/pac5523#et_pb_tab_4


The source code provides examples of various aspects of PAC55xx devices.
1)  Start in the .\src folder where main.c is found along with config.h.  Choose the specific PAC55xx
device in config.h and also uncomment a #define for the desired example code.
2) Choose additional #defines within a specific example source file.
3) Before running, connect the PAC55xx UART to the PC using an adapter
4) Configure a PC terminal app to see printfs from the code: 115200 bps 1 Start bit, 1 Stop bit, no parity

See the .\resources folder for additional documentation.

project_version.h defines the current version and also includes revision history

The projects are known to work with the following IDE versions
Keil MDK (uVision) v5.25
IAR v7.50.2 and v8.20.1 (Note: for v8.x, IAR will convert the IAR 7.x project automatically; no additional modifications are required)
Eclipse with PAC Support v1.1.0 (GCC arm-7.0-2017-q4-major)

