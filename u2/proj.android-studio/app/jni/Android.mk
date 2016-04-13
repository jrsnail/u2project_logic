LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../../Dependenices/libs/Android/armeabi-v7a)

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
                    $(LOCAL_PATH)/../../../gamebase
					
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../Dependenices/include


# 遍历目录及子目录的函数
define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# 遍历Classes目录
ALLFILES = $(call walk, $(LOCAL_PATH)/../../../Classes)
ALLFILES += $(call walk, $(LOCAL_PATH)/../../../engine)
ALLFILES += $(call walk, $(LOCAL_PATH)/../../../gamebase)

FILE_LIST := hellocpp/main.cpp
# 从所有文件中提取出所有.cpp文件
FILE_LIST += $(filter %.cpp, $(ALLFILES))
# 从所有文件中提取出所有.c文件
FILE_LIST += $(filter %.c, $(ALLFILES))
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)




# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END


include $(BUILD_SHARED_LIBRARY)

$(call import-module,libzziplib) 
$(call import-module,.)


# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
