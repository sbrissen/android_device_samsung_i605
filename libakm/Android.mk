# libakm: this is a free replacement for the non-free libakm that comes with
# android devices such as the Nexus S.
# Copyright (C) 2011  Paul Kocialkowski
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

BUILD_LIBAKM := true

ifneq ($(BUILD_LIBAKM),)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := akm.c \
    sensors/default.c \
    sensors/ak8963.c \
    sensors/lsm330dlc.c \

LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_CFLAGS := -DLOG_TAG=\"libakm_free\"
LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libakm
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif # BUILD_LIBAKM
