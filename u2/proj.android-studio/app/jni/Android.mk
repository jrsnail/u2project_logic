LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../Dependenices/libs)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

#LOCAL_SRC_FILES := hellocpp/main.cpp \
#                   ../../../Classes/AppDelegate.cpp \
#                   ../../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes \
                    $(LOCAL_PATH)/../../../engine \
                    $(LOCAL_PATH)/../../../engine/include \
                    $(LOCAL_PATH)/../../../engine/include/core \
					$(LOCAL_PATH)/../../../engine/include/core/Android \
                    $(LOCAL_PATH)/../../../engine/include/core/streams \
                    $(LOCAL_PATH)/../../../engine/include/core/threading \
                    $(LOCAL_PATH)/../../../engine/include/mvc \
					$(LOCAL_PATH)/../../../engine/include/lua \
					$(LOCAL_PATH)/../../../engine/include/rapidjson \
					$(LOCAL_PATH)/../../../engine/include/rapidjson/internal \
					$(LOCAL_PATH)/../../../engine/include/rapidjson/error \
					$(LOCAL_PATH)/../../../engine/include/xml \
                    $(LOCAL_PATH)/../../../gamebase \
					$(LOCAL_PATH)/../../../luabridge
					
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../Dependenices/include

#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../cocos2d/cocos


# 遍历目录及子目录的函数
define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# 遍历指定目录的函数，不含子目录
define walk_weak
    $(wildcard $(1))
endef


# 遍历Classes目录
ALLFILES = $(call walk, $(LOCAL_PATH)/../../../Classes)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/core/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/core/streams/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/core/threading/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/core/Android/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/mvc/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/lua/*)
ALLFILES += $(wildcard $(LOCAL_PATH)/../../../engine/src/xml/*)
ALLFILES += $(call walk, $(LOCAL_PATH)/../../../gamebase)
ALLFILES += $(call walk, $(LOCAL_PATH)/../../../luabridge)

FILE_LIST := hellocpp/main.cpp
# 从所有文件中提取出所有.cpp文件
FILE_LIST += $(filter %.cpp, $(ALLFILES))
# 从所有文件中提取出所有.c文件
FILE_LIST += $(filter %.c, $(ALLFILES))
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)




# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += u2_zzip_static
LOCAL_STATIC_LIBRARIES += u2_curl_static
LOCAL_STATIC_LIBRARIES += u2_ssl_static
LOCAL_STATIC_LIBRARIES += u2_crypto_static



LOCAL_LDLIBS += -latomic


# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END


include $(BUILD_SHARED_LIBRARY)

$(call import-module, zzip/Android)
$(call import-module, curl/Android)
$(call import-module, websocket/Android)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
