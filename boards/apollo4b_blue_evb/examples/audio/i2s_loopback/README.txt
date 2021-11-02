Name:
=====
 i2s_loopback


Description:
============
 An example to show basic I2S operation.


Purpose:
========
This example enables the I2S interfaces to loop back data from
each other.  Either I2S0 or I2S1 can be selected as the master.
The required pin connections are as follows.
Apollo4 (BGA):
J9-2 I2S1DIN  to J9-16 I2S0DOUT
J9-4 I2S1WS   to J9-14 I2S0WS
J9-6 I2S1DOUT to J9-12 I2S0DIN
J9-8 I2S1CLK  to J11-7 I2S0CK

Apollo4 Blue (SIP):
J9-1 I2S1DIN  to J9-8  I2S0DOUT
J9-2 I2S1WS   to J9-7  I2S0WS
J9-3 I2S1DOUT to J9-6  I2S0DIN
J9-4 I2S1CLK  to J11-7 I2S0CK

Printing takes place over the ITM at 1M Baud.



******************************************************************************


