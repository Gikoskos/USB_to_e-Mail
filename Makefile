CC = gcc
CFLAGS = -g -Wall -I/usr/include/libusb-1.0 -o
LINKER = -lusb-1.0

usb2mail: usb2mail.c
	$(CC) $(CFLAGS) $@ $< $(LINKER)

.PHONY: clean

clean: usb2mail
	rm -f $<