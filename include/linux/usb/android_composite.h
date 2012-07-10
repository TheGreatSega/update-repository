/*
 * Platform data for Android USB
 *
 * Copyright (C) 2008 Google, Inc.
 * Author: Mike Lockwood <lockwood@android.com>
 * Copyright (C) 2010-2011 Sony Ericsson Mobile Communications AB.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef	__LINUX_USB_ANDROID_H
#define	__LINUX_USB_ANDROID_H

#include <linux/usb/composite.h>
#include <linux/if_ether.h>

/* Storage mode */
#define STORAGE_MODE_NONE	0
#define STORAGE_MODE_MSC	1
#define STORAGE_MODE_CDROM	2

struct android_usb_function {
	struct list_head	list;
	char			*name;
	int 			(*bind_config)(struct usb_configuration *c);
};

struct android_usb_product {
	/* Vendor ID for this set of functions.
	 * Default vendor_id in platform data will be used if this is zero.
	 */
	__u16 vendor_id;

	/* Product ID for this set of functions. */
	__u16 product_id;

	/* List of function names associated with this product.
	 * This is used to compute the USB product ID dynamically
	 * based on which functions are enabled.
	 */
	int num_functions;
	char **functions;
	int msc_mode;
};

struct android_usb_platform_data {
	/* USB device descriptor fields */
	__u16 vendor_id;

	/* Default product ID. */
	__u16 product_id;

	__u16 version;

	char *product_name;
	char *manufacturer_name;
	char *serial_number;

	/* List of available USB products.
	 * This is used to compute the USB product ID dynamically
	 * based on which functions are enabled.
	 * if num_products is zero or no match can be found,
	 * we use the default product ID
	 */
	int num_products;
	struct android_usb_product *products;

	/* List of all supported USB functions.
	 * This list is used to define the order in which
	 * the functions appear in the configuration's list of USB interfaces.
	 * This is necessary to avoid depending upon the order in which
	 * the individual function drivers are initialized.
	 */
	int num_functions;
	char **functions;
};

/* EUI-64 identifier format for Device Identification VPD page */
struct eui64_id {
	u8 ieee_company_id[3];
	u8 vendor_specific_ext_field[5];
};

/* Platform data for "usb_mass_storage" driver. */
struct usb_mass_storage_platform_data {
	/* Contains values for the SC_INQUIRY SCSI command. */
	char *vendor;
	char *product;
	int release;

	char can_stall;
	/* number of LUNS */
	int nluns;

	/* Information for CD-ROM */
	char *cdrom_vendor;
	char *cdrom_product;
	int cdrom_release;

	/* number of CD-ROM LUNS */
	int cdrom_nluns;

	char *serial_number;
	struct eui64_id eui64_id;
};

/* Platform data for USB ethernet driver. */
struct usb_ether_platform_data {
	u8	ethaddr[ETH_ALEN];
	u32	vendorID;
	const char *vendorDescr;
};

extern void android_register_function(struct android_usb_function *f);

extern int android_enable_function(struct usb_function *f, int enable);

#ifdef CONFIG_USB_ANDROID_GG
extern void android_enable_usb_gg(void);
#endif

extern int android_get_msc_mode(struct device *dev);

#endif	/* __LINUX_USB_ANDROID_H */