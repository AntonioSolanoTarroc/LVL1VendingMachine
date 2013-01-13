Raspberry Pi Code for Vending Machine
=====================================

This is the code running on the vending machine, interacting with the raspberry pi.  There is a C portion and a Python portion.  The python code interacts with the high-level web functions of the PHP web store.  The C portion interacts with hardware on the raspberry pi.  [WiringPi](https://projects.drogon.net/raspberry-pi/wiringpi/) is used to interact with the GPIO in order to read IRQs and data from the Wiegand reader.  IOCTLs are used to interact with the I2C bus.