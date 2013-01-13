#!/bin/sh

#configure gpio for use with wiringpi bit bang.  Required for irq and wiegand decoding.

#button irqs
gpio edge 17 rising
gpio edge 18 rising
gpio edge 21 rising

#wiegand gpio
gpio edge 23 falling
gpio edge 24 falling