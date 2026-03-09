RP2040 C/C++ SDK implementation of a flicker-free 7-segment multiplexer. Uses timer interrupts and GPIO masking to achieve precise timing and 0% CPU load in the main loop.*

Hardware:
Raspberry Pi Pico
7 Segment Display 4 digits 20mm.

Segment/Digit,Pico GPIO
A,GPIO 4
B,GPIO 1
C,GPIO 18 (0x12)
D,GPIO 20 (0x14)
E,GPIO 21 (0x15)
F,GPIO 5
G,GPIO 17 (0x11)
D1 - D4,"GPIO 3, 2, 0, 16"
