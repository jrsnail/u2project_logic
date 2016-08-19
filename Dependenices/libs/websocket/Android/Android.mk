LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := u2_websocket_static
LOCAL_MODULE_FILENAME := websocket
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libwebsockets.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../include
include $(PREBUILT_STATIC_LIBRARY)
