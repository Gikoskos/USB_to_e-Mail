# USB 2 Mail
## Overview

Use this application if you want to automatically send a pre-written email to a user-specified address,
just by inserting a USB device in the hub.

If you have a USB storage device for example, and have its IDs (on Linux you can get the IDs of all your devices by typing
lsusb on a terminal) you can use this application to send an email every time you insert the USB device in the computer.

## Prerequisites

This software makes use of the libusb open source library: [libusb on Github](https://github.com/libusb/libusb)

Also need to have Perl installed and a couple of Perl packages:
[Email::Send::SMTP::Gmail dependencies](http://deps.cpantesters.org/?module=Email%3A%3ASend%3A%3ASMTP%3A%3AGmail;perl=latest)

To run this program safely on your machine please make sure that the Perl script is on the same folder as the executable.

## TODO

Provide basic interface for accepting the emails and passwords as input from the keyboard, instead of having to edit the Perl script.

Make the code portable for use on Windows machines.

Add selection menu, for the user to choose any USB device from a list of all USB devices currently connected on the system.

### License stuff

![GPLv3 logo](http://www.gnu.org/graphics/gplv3-127x51.png)

Copyright © 2015 G.K. <cyberchiller@gmail.com>
