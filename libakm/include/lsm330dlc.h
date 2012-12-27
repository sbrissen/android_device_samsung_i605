/*
 *  STMicroelectronics lsm330dlc acceleration sensor driver
 *
 *  Copyright (C) 2010 Samsung Electronics Co.Ltd
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __LSM330DLC_ACC_HEADER__
#define __LSM330DLC__ACC_HEADER__

#include <linux/types.h>
#include <linux/ioctl.h>

struct lsm330dlc_acceldata {
	__s16 x;
	__s16 y;
	__s16 z;
};

/* dev info */
#define ACC_DEV_NAME "accelerometer_sensor"

/* lsm330dlc ioctl command label */
#define LSM330DLC_IOCTL_BASE 'a'
#define LSM330DLC_IOCTL_SET_DELAY       _IOW(LSM330DLC_IOCTL_BASE, 0, int64_t)
#define LSM330DLC_IOCTL_GET_DELAY       _IOR(LSM330DLC_IOCTL_BASE, 1, int64_t)
#define LSM330DLC_IOCTL_READ_ACCEL_XYZ  _IOR(LSM330DLC_IOCTL_BASE, 8, \
						struct lsm330dlc_acceldata)

#ifdef __KERNEL__
struct lsm330dlc_platform_data {
	int gpio_acc_int;	/* gpio for kr3dm int output */
	s8 *rotation;		/* rotation matrix, if NULL assume Id */
};
#endif /* __KERNEL__ */

#endif
