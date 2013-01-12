LVL1 Vending Machine
====================

AKA Tom's Electronic Depot.

![Tom's Vending Machine](http://i.imgur.com/Bsojp.jpg)

Since the beginning, LVL1 has had this vending machine kicking around.  When we plugged it in for the first time, there were sparks and pops and lots of smoke.  Some enterprising folks managed to bypass the coin acceptor, and get the buttons to directly dispense snack.  For over a year, the machine served as a snack dispensery.  It was frustrating to fill, so the snacks simply got piled on top of and around the machine.  Thus, it was moved to make space for better snack storage space.

This project aims to retrofit the machine with a raspberry pi and control electronics, and an online store component.  The online store keeps inventory, collects money, and give the users an invoice code.  When the invoice code is entered on the vending machine, the selected products drop out!

The raspberry pi interfaces with two custom boards designed to interact with the vending machine's 1970's vintage electronics.  When the control lines to the vending motors are momentarily pulsed with 30 VAC, they turn a single revolution.  By connecting these control lines to relays instead of buttons, the raspberry pi has control over them.  The buttons are rerouted to IO expanders.  All vending machine interaction occurs with the help of these I2C based IO expanders.  An RFID reader allows LVL1 members to make spontaneous purchases, then get billed at the end of the month.

How to Use
----------

1. Rename API_KEY.py.example to API_KEY.py and change the API key to something secure.