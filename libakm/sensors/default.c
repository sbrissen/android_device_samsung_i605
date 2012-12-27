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

#include <unistd.h>
#include <fcntl.h>
#include <errno.h> 
#include <sys/types.h>
#include <dirent.h>
#include <linux/input.h>

#include "akm.h"
#include "sensors/akm_sensors.h"

#define SENSOR_NAME	"default"

/* These are the standard functions to enable and disable a sensor. */
int default_enable(struct akm_sensor *sensor)
{
	int rc;

	/* Skip if the sensor is already enabled. */
	if(sensor->enabled)
	{
		ALOGV("%s already enabled.\n", SENSOR_NAME);
		return 0;
	}

	/* Init the sensor chip. Will be skiped if already initialized. */
	rc=sensor->chip->init(sensor->chip);

	/* Set the sensor as enabled. */
	sensor->enabled=1;

	return rc;
}

int default_disable(struct akm_sensor *sensor)
{
	/* Skip if the sensor is already disabled. */
	if(!sensor->enabled)
	{
		ALOGV("%s already disabled.\n", SENSOR_NAME);
		return 0;
	}

	/* Set the sensor as disabled. */
	sensor->enabled=0;

	return 0;
}
