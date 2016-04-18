LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := u2_zzip_static
LOCAL_MODULE_FILENAME := zziplib
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libzziplib.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../include
include $(PREBUILT_STATIC_LIBRARY)
