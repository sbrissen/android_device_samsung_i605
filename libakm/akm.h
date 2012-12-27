/*
 * libakm: this is a free replacement for the non-free libakm that comes with
 * android devices such as the Nexus S.
 * Copyright (C) 2011  Paul Kocialkowski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __AKM_H__
#define __AKM_H__

#include <stdio.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <pthread.h>

#include "hardware/sensors.h"

/* This structure contains the sensor-related infos. */
struct akm_sensor
{
	uint8_t type;
	uint8_t enabled;

	int (*enable)(struct akm_sensor *sensor_info);
	int (*disable)(struct akm_sensor *sensor_info);
	int (*set_delay)(struct akm_sensor *sensor_info, uint64_t delay);

	struct akm_chip_sensors *chip;
};

/* This structure contains the publisher-realted infos. */
struct akm_publisher
{
	int fd;
	char *input_device_name;
	char *input_node_name;
	uint8_t inited;
	int (*init)(struct akm_chip_sensors *chip);
	int (*deinit)(struct akm_chip_sensors *chip);
	void (*data_publish)(struct akm_chip_sensors *chip, uint8_t type, void *data);
};

/* 
 * This structure contains the sensors for one chip. Since a chip can hold 
 * various sensors types, it holds a list of the sensors attached to the chip.
 */
struct akm_chip_sensors
{
	struct akm_publisher *publisher;
	void (*data_get)(struct akm_chip_sensors *chip);
	int (*init)(struct akm_chip_sensors *chip);
	int (*deinit)(struct akm_chip_sensors *chip);
	uint8_t inited;
	char *device_name;
	int fd;
	int data_lock;
	int sensors_count;
	struct akm_sensor *sensors[];
};

/* This structure holds basic 3-dimensional vector infos (x, y and z coords). */
struct akm_publish_vector
{
	int x;
	int y;
	int z;
};

/* Functions definitions, as defined in akm.c. */

void akm_init(void);
void akm_deinit(void);
int akm_sanity_check(struct akm_chip_sensors *device_chips[], int device_chips_count);
struct akm_sensor *akm_get_sensor(struct akm_chip_sensors *device_chips[], int device_chips_count, uint32_t sensor_type);
int akm_is_sensor_enabled(uint32_t sensor_type);
int akm_enable_sensor(uint32_t sensor_type);
int akm_disable_sensor(uint32_t sensor_type);
int akm_set_delay(uint32_t sensor_type, uint64_t delay);

#endif
