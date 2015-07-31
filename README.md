# USB 2 Mail
## Overview

Use this application if you want to automatically send a pre-written email to a user-specified address,
just by inserting a USB device in the hub.

It works kinda like an alarm; the loop never ends, so every time it recognizes your USB device on the computer it
sends the email.

## Prerequisites

This software makes use of the libusb open source library: [libusb on Github](https://github.com/libusb/libusb)

Also you need to install the Email::Send::SMTP::Gmail perl module: cpan install Email::Send::SMTP::Gmail

To run this program successfully on your machine please make sure that the perl script is on the same folder as the executable.

## TODO

Provide basic interface for accepting the emails and passwords as input from the keyboard, instead of having to edit the Perl script.

Make the code portable for use on Windows machines.

Add selection menu, for the user to choose any USB device from a list of all USB devices currently connected on the system. (DONE)

## Caveats

The USB device you chose initially has to be reinserted on the same Bus it was connected the first time or 
it won't work. I did this to avoid ID conflict, since some devices share the same IDs.

Also on Linux you need root privileges to run this application. I read there's a workaround by writing your own udev rules
but I don't have time to work on this now. Feel free to do your own research.

### License stuff

![GPLv3 logo](http://www.gnu.org/graphics/gplv3-127x51.png)

Copyright © 2015 G.K. <cyberchiller@gmail.com>
