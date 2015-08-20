/*   usb2mailID is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

/*****************************************************************************
 *                                                                           *
 * sends e-mail automatically after you insert a USB device of your choosing *
 *            (you need to type in your device's ID in this version)         *
 *                                                                           *
 *              by cyberchiller@gmail.com    Copyright 2015                  *
 *                                                                           *
 *****************************************************************************/

#include <stdio.h>
#include <libusb.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define FAILURE 1

int is_target (libusb_device*, int, int);
void print_error (ssize_t);
int find_device (libusb_device**, ssize_t);

int targetVid, targetPid;	/*IDs of the device you want to be found*/

int find_device (libusb_device **devs, ssize_t c)
{
	libusb_device *current = NULL;
	ssize_t i = 0;
	
	for (; i < c; i++) {
		current = devs[i];
		if (is_target(current, targetVid, targetPid) == SUCCESS)
			return SUCCESS;
		fprintf(stderr, "NOT FOUND\n");
	}
	return FAILURE;
}

void print_error (ssize_t error_number)
{
	fprintf(stderr, "%s\n", libusb_error_name(error_number));
}

int is_target (libusb_device *curr_dev, int targVid, int targPid)
{
	struct libusb_device_descriptor curr_desc;
	int err_num;
	
	err_num = libusb_get_device_descriptor(curr_dev, &curr_desc);
	if (err_num < 0) {
		print_error(err_num);
		return FAILURE;
	} else if (curr_desc.idVendor == targVid && curr_desc.idProduct == targPid) {
		return SUCCESS;
	}
	return FAILURE;
}

int main (int argc, char *argv[], char *environ[])
{
	int err_num, f_status = FAILURE;
	libusb_device **usb_devs;
	ssize_t device_count;
	
	err_num = libusb_init(NULL);
	if (err_num) {
		print_error((ssize_t)err_num);
		return FAILURE;
	} else 
		libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_WARNING);

	printf("Please enter your USB device's Vendor ID in hexadecimal format: ");
	scanf("%x", &targetVid);
	printf("Please enter your USB device's Product ID in hexadecimal format: ");
	scanf("%x", &targetPid);
	
	do {
		device_count = libusb_get_device_list(NULL, &usb_devs);
		if (device_count < 0) {
			print_error(device_count);
			libusb_exit(NULL);
			return FAILURE;
		}
		
		if (find_device(usb_devs, device_count) == SUCCESS) {
			f_status = SUCCESS;
		}
		
		libusb_free_device_list(usb_devs, 0);
		sleep(2);
	} while (f_status == FAILURE);
	printf("DEVICE FOUND\n");
	system("perl tinymailclient.pl");
	printf("EMAIL SENT\n");
	libusb_exit(NULL);
	return 0;
}
