CC = gcc
CFLAGS = -g -Wall -I/usr/include/libusb-1.0 -o
NDEBUG = -DNDEBUG
LINKER = -lusb-1.0

usb2mail: usb2mail.c
	@$(CC) $(NDEBUG) $(CFLAGS) $@ $< $(LINKER)

debug: usb2mail.c
	@$(CC) $(CFLAGS) $@ $< $(LINKER)

usb2mailID: usb2mailID.c
	@$(CC) $(NDEBUG) $(CFLAGS) $@ $< $(LINKER)

debugID: usb2mailID.c
	@$(CC) $(CFLAGS) $@ $< $(LINKER)

.PHONY: clean

clean:
	@rm -rf usb2mail usb2mailID debug debugID a.out