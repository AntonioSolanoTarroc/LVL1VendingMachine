Vending Machine Opencart Fork
=============================

This is a fork of OpenCart design for the LVL1 Interactive Vending Machine.  It is a fork of OpenCart 1.5.4.1, and it is unlikely that upgrades will work as expected.  Some core functionality has been changed and some API functions added in order to facilitate interactivity with the LVL1 Vending Machine.

The Vending Machine uses this software to manage inventory and pricing.  The vending machine will not work without a valid OpenCart instance.

Key modifications
=================

Item Locations are integer values which represent the vending machine slot in which they're located.  Items which aren't stored in the vending machine should NOT have this value defined, or the behavior of the machine is undefined.

TODO: Fix parsing of data to filter out non-integer locations from API lookups.

After a user checks out, they are given an invoice code.  Invoice codes are generated automatically, and invoice prefix functionality has been crippled haphazardly.  Invoice codes are 6 digits integers with no digits equal to zero (0).  This is because the vending machine does not have a zero digit.  The user can then enter this code on the vending machine, and the products will dispense.

An API has been added with three functions: Lookup order product locations based on invoice number, mark order as completed by invoice number, and create new order with given first name, last name, email address and item slot number.

admin/model/sale/order.php
--------------------------

File is modified to allow generation of invoice numbers to my preference.

catalog/controller/api/api.php
------------------------------

Vending Machine API functions are here.

catalog/controller/checkout/confirm.php
---------------------------------------

Add invoice number to the session data for display on the confirmation screen.

catalog/controller/checkout/success.php
---------------------------------------

Add invoice number display strings for display on the confirmation screen.

language/english/checkout/success.php
-------------------------------------

English strings for displaying invoice number on the confirmation screen.

catalog/model/checkout/order.php
--------------------------------

Add automatic invoice number generation to the ordering model.

catalog/view/theme/default/template/checkout/guest.tpl
---------------------------------------------

Hid a bunch of fields that are irrelevant for a vending machine.  Even if the vending machine sells products that aren't in the physical machine, users will never need to give a shipping address, since I'm not in the business of shipping things out all over Louisville.  Simplifying this menu for quick and easy use is more important.  This is accomplished by hiding the data fields that I don't consider relevant, and filling them in with LVL1's address information.

catalog/view/theme/default/template/common/footer.tpl
-----------------------------------------------------

Remove affiliate program and returns links.

catalog/view/theme/default/template/common/success.tpl
------------------------------------------------------

Close the loop on displaying invoice number on the confirmation screen.

system/library/response.php
---------------------------

Add a function for returning JSON data with the correct header type.

config.php
----------

Add a global variable for the private API key.  This isn't very secure, but it will do for a vending machine.