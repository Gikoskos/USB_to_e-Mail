/*   usb2mail is free software: you can redistribute it and/or modify
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

/*****************************************************************************/
/*                                                                           */
/* sends e-mail automatically after you insert a USB device of your choosing */
/*                                                                           */
/*              by cyberchiller@gmail.com    Copyright 2015                  */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <libusb.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>


#define SUCCESS 0
#define FAILURE 1



struct Indexed_idev_node{
	unsigned short idx;
	uint16_t vend_ID;
	uint16_t dev_ID;
	uint8_t d_bus;
	struct Indexed_idev_node *nxt;
};

union Error_codes {
	ssize_t err_init;
	ssize_t err_handle;
	ssize_t err_desc;
	ssize_t err_str;
	ssize_t err_print;
	ssize_t err_inp;
};

static union Error_codes err_num;


/* function prototypes */
void clear_buffer (void);
void print_error (ssize_t);
int create_ilist (libusb_device**, ssize_t, struct Indexed_idev_node**);
struct Indexed_idev_node *add_dev_node (struct Indexed_idev_node*, uint16_t, uint16_t, uint8_t);
void print_idev_list (struct Indexed_idev_node*);
void destroy_idev_list (struct Indexed_idev_node*);
int valid_index (struct Indexed_idev_node*, char, uint16_t*, uint16_t*, uint8_t*);
int find_device (libusb_device**, ssize_t, uint16_t, uint16_t, uint8_t);
void sig_handle (int);


void clear_buffer (void)
{
	char clbuf;
	while ((clbuf=getchar()) != '\n');
}

struct Indexed_idev_node *add_dev_node (struct Indexed_idev_node *idevllist, 
								uint16_t vID, uint16_t dID, uint8_t ibus)
{
	struct Indexed_idev_node *new_idev = malloc(sizeof(struct Indexed_idev_node));
	int i = 1;

	new_idev->nxt = NULL;
	new_idev->vend_ID = vID;
	new_idev->dev_ID = dID;
	new_idev->d_bus = ibus;

	if (!idevllist) {
		idevllist = malloc(sizeof(struct Indexed_idev_node));
		new_idev->idx = i;
		idevllist = new_idev;
		return idevllist;
	} else {
		struct Indexed_idev_node *curr = idevllist;
		i++;
		while (curr->nxt) {
			curr = curr->nxt;
			i++;
		}
		new_idev->idx = i;
		curr->nxt = new_idev;
	}
	return idevllist;
}

int create_ilist (libusb_device **devs, ssize_t c, struct Indexed_idev_node **idevllist)
{
	ssize_t i = 0;
	libusb_device_handle *temp_dev_handle = NULL;
	struct libusb_device_descriptor temp_desc;
	unsigned char temp_product[265], temp_mnf[265];

	for (; i < c; i++) {
		err_num.err_handle = libusb_open(devs[i], &temp_dev_handle);
		if (err_num.err_handle) {
			print_error(err_num.err_handle);
			return FAILURE;
		}
		err_num.err_desc = libusb_get_device_descriptor(devs[i], &temp_desc);
		if (err_num.err_desc) {
			print_error(err_num.err_desc);
			libusb_close(temp_dev_handle);
			return FAILURE;
		}
		err_num.err_str = libusb_get_string_descriptor_ascii(temp_dev_handle, 
									temp_desc.iProduct, temp_product, 264);
		if (!err_num.err_str) {
			print_error(err_num.err_str);
			libusb_close(temp_dev_handle);
			return FAILURE;
		}
		err_num.err_str = libusb_get_string_descriptor_ascii(temp_dev_handle, 
									temp_desc.iManufacturer, temp_mnf, 264);
		if (!err_num.err_str) {
			print_error(err_num.err_str);
			libusb_close(temp_dev_handle);
			return FAILURE;
		}
		*idevllist = add_dev_node(*idevllist, temp_desc.idVendor, temp_desc.idProduct, 
							 libusb_get_bus_number(devs[i])); 
		printf("(%d)%s, %s ID = %04x:%04x\n", i+1, temp_mnf, 
			  temp_product, temp_desc.idVendor, temp_desc.idProduct);
		libusb_close(temp_dev_handle);
	}
	return SUCCESS;
}

void print_error (int error_number)
{
	fprintf(stderr, "%s\n", libusb_error_name(error_number));
}

void destroy_idev_list (struct Indexed_idev_node *idevllist)
{
	struct Indexed_idev_node *curr = idevllist, *temp;
	
	while (curr) {
		temp = curr;
		curr = curr->nxt;
		free(temp);
	}
}

void print_idev_list (struct Indexed_idev_node *idevllist) 
{
	struct Indexed_idev_node *curr = idevllist;
	if (curr != NULL) {
		while(curr) {
			printf("\t%hu %04x:%04x %d\n", curr->idx, curr->vend_ID, curr->dev_ID, curr->d_bus);
			curr = curr->nxt;
		}
	} else
		printf("empty list");
}

int valid_index (struct Indexed_idev_node *idevllist, char inp, 
			  uint16_t *tvID, uint16_t *tdID, uint8_t *tbus)
{
	struct Indexed_idev_node *curr = idevllist;
	int c = inp - '0';
	if (curr != NULL) {
		while (curr) {
			if (curr->idx == c) {
				*tvID = curr->vend_ID;
				*tdID = curr->dev_ID;
				*tbus = curr->d_bus;
				return SUCCESS;
			}
			curr = curr->nxt;
		}
	}
	return FAILURE;
}

int find_device (libusb_device **list, ssize_t dcount, 
				  uint16_t venID, uint16_t devID, uint8_t bus)
{
	libusb_device *curr_dev = NULL;
	struct libusb_device_descriptor curr_desc;
	ssize_t i = 0;
	
	for (; i < dcount; i++) {
		curr_dev = list[i];
		err_num.err_desc = libusb_get_device_descriptor(curr_dev, &curr_desc);
		if (libusb_get_bus_number(curr_dev) == bus && curr_desc.idVendor == venID &&
			curr_desc.idProduct == devID)
			return SUCCESS;
	}
	return FAILURE;
}

#ifdef NDEBUG
void sig_handle (int sig)
{
	signal(sig, SIG_IGN);
}
#endif

int main (int argc, char *argv[], char *environ[])
{
	libusb_device **usb_devs;
	ssize_t device_count;
	char user_input;
	uint16_t target_vID, target_dID;
	uint8_t target_bus;
	int ready = FAILURE;
#ifdef NDEBUG
	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);
#endif
	STRT:
	puts("\033[2J");
	err_num.err_init = libusb_init(NULL);
	if (err_num.err_init) {
		print_error(err_num.err_init);
		return FAILURE;
	} else 
		libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_WARNING);
	do {
		struct Indexed_idev_node *idevllist = NULL;
		device_count = libusb_get_device_list(NULL, &usb_devs);
		if (device_count < 0) {
			print_error(device_count);
			libusb_exit(NULL);
			return FAILURE;
		}
		
		err_num.err_print = create_ilist(usb_devs, device_count, &idevllist);
		if (err_num.err_print) {
			print_error(LIBUSB_ERROR_OTHER);
			return FAILURE;
		}
		printf("Please enter the index of your USB device or R to rescan the devices: ");
		user_input = getchar();
		clear_buffer();
		if (user_input != 'R' && user_input != 'r') {
			err_num.err_inp = valid_index(idevllist, user_input, 
									&target_vID, &target_dID, &target_bus);
			if (err_num.err_inp) {
				fprintf(stderr, "Bad input\n");
				sleep(2);
				libusb_exit(NULL);
				destroy_idev_list(idevllist);
				goto STRT;
			}
			destroy_idev_list(idevllist);
			break;
		}
		sleep(3);
		puts("\033[2J");
		destroy_idev_list(idevllist);
	} while (1);
	printf("Remove your USB device and press ENTER");
	libusb_free_device_list(usb_devs, 0);
	clear_buffer();
	do {
		device_count = libusb_get_device_list(NULL, &usb_devs);
		if (device_count < 0) {
			print_error(device_count);
			libusb_exit(NULL);
			return FAILURE;
		}
		ready = find_device(usb_devs, device_count, target_vID, target_dID, target_bus);
		if (!ready) {
			system("perl tinymailclient.pl");
			printf("Mail sent!\n");
			sleep(10);
		}
		sleep(1);
	} while (1);
	libusb_free_device_list(usb_devs, 0);
	libusb_exit(NULL);
	return SUCCESS;
}
