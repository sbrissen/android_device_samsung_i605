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

#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <cutils/log.h>

#include "akm.h"
#include "sensors/akm_sensors.h"


struct akm_chip_sensors *akm_device_chips[]=
{
	&lsm330dlc,
	&ak8963
};

/* This function is called when the lib is dlopened. */
__attribute__ ((__constructor__)) 
void akm_init(void)
{
	int i;
	int device_chips_count;

	/* Count the number of chips that are defined for the device. */
	device_chips_count=sizeof(akm_device_chips) / sizeof(void *);

	/*
	 * Check that the device doesn't have two chips with the same sensor 
	 * type. If it's the case, it'll abort the publisher initialisation.
	 */
	if(!akm_sanity_check(akm_device_chips, device_chips_count))
	{
		ALOGE("Sanity check failed.\n");
		return;
	}

	/* Init the data publisher for each chip of the device. */
	for(i=0 ; i < device_chips_count ; i++)
		akm_device_chips[i]->publisher->init(akm_device_chips[i]);
}

/* This function is called when the lib is dlclosed. */
__attribute__ ((__destructor__)) 
void akm_deinit(void)
{
	int i;
	int device_chips_count;

	/* Count the number of chips that are defined for the device. */
	device_chips_count=sizeof(akm_device_chips) / sizeof(void *);

	/* Deinit the publisher for each chip of the device. */
	for(i=0 ; i < device_chips_count ; i++)
		if(!akm_device_chips[i]->publisher->inited)
			akm_device_chips[i]->publisher->deinit(akm_device_chips[i]);
}

/* 
 * This function is to check that the device doesn't have two chips with the same 
 * sensor type.
 */
int akm_sanity_check(struct akm_chip_sensors *device_chips[], int device_chips_count)
{
	int rc=1;

	int i, j;

	int check=0;
	int t;

	/* Look in each chip of the device. */
	for(i=0 ; i < device_chips_count ; i++)
	{
		/* Look in each sensor of the chip. */
		for(j=0 ; j < device_chips[i]->sensors_count ; j++)
		{
			/* Create a flag for the device chip. */
			t=0xf << (device_chips[i]->sensors[j]->type * 4);

			/* 
			 * If the flag is already in the precedently-collected
			 * flags, print an error message and break the loops.
			 */
			if(check & t)
			{
				ALOGE("Sensor type %d is already registered on the device.\n");
				rc=0;
				break;
			}

			/* Add the flag to the already collected ones. */
			check+=t;
		}
	}

	return rc;
}

/* 
 * This function is to get the sensor from the devices chips and the sensor 
 * type.
 */
struct akm_sensor *akm_get_sensor(struct akm_chip_sensors *device_chips[], int device_chips_count, uint32_t sensor_type)
{
	int i, j;
	ALOGI("device_chips_count (%d)\n", device_chips_count);
	/* Look in each chip of the device. */
	for(i=0 ; i < device_chips_count ; i++){
		/* Look in each sensor of the chip. */
		for(j=0 ; j < device_chips[i]->sensors_count ; j++){
			/* 
			 * If the sensor type is the requested one, return the
			 * pointer to the sensor. 
			 */
			if(device_chips[i]->sensors[j]->type == sensor_type){
				return device_chips[i]->sensors[j];
			 }
		}
	}

	/* If nothing was returned, print an error and return NULL. */
	ALOGE("Failed to get the asked sensor (%d)\n", sensor_type);

	return (struct akm_sensor *) NULL;
}

/* This is to check if the sensor of the given type is enabled. */
int akm_is_sensor_enabled(uint32_t sensor_type)
{
	struct akm_sensor *sensor=NULL;

	/* Get the pointer to the sensor. */
	sensor=akm_get_sensor(akm_device_chips, sizeof(akm_device_chips) / sizeof(void *), sensor_type);

	/* If it's not NULL, return its enabled state. Otherwise, return 0. */
	if(sensor == NULL)
		return 0;
	else
		return sensor->enabled;
}

/* This is to enable the sensor of the given type. */
int akm_enable_sensor(uint32_t sensor_type)
{
	int rc;
	struct akm_sensor *sensor=NULL;

	/* Get the pointer to the sensor. */
	sensor=akm_get_sensor(akm_device_chips, sizeof(akm_device_chips) / sizeof(void *), sensor_type);

	/* If it's not NULL, enable the sensor and return the return code. */
	if(sensor == NULL)
		return 1;

	rc=sensor->enable(sensor);

	return rc;
}

/* This is to disable the sensor of the given type. */
int akm_disable_sensor(uint32_t sensor_type)
{
	int rc;
	struct akm_sensor *sensor=NULL;

	/* Get the pointer to the sensor. */
	sensor=akm_get_sensor(akm_device_chips, sizeof(akm_device_chips) / sizeof(void *), sensor_type);

	/* If it's not NULL, disable the sensor and return the return code. */
	if(sensor == NULL)
		/* 
		 * Return 0 because it's nothing bad to return success while 
		 * disabeling a non-existent sensor.
		 */
		return 0;
	
	rc=sensor->disable(sensor);

	return rc;
}

/* This is to set the data acquisition delay on the sensor of the given type. */
int akm_set_delay(uint32_t sensor_type, uint64_t delay)
{
	int rc;
	struct akm_sensor *sensor=NULL;

	akm_enable_sensor(1);
	/* Get the pointer to the sensor. */
	sensor=akm_get_sensor(akm_device_chips, sizeof(akm_device_chips) / sizeof(void *), sensor_type);

	/* If it's not NULL, set the sensor delay and return the return code. */
	if(sensor == NULL)
		return 1;

	rc=sensor->set_delay(sensor, delay);

	return rc;
}
