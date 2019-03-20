#ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_ABI=armeabi-v7a APP_PLATFORM=android-19

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)



ZZ_INCLUDE := $(LOCAL_PATH)/include \
			$(LOCAL_PATH)/src

ZZ_SRC := $(wildcard $(LOCAL_PATH)/src/*.c) \
			$(wildcard $(LOCAL_PATH)/src/zzdeps/common/*.c) \
			$(wildcard $(LOCAL_PATH)/src/zzdeps/linux/*.c) \
			$(wildcard $(LOCAL_PATH)/src/zzdeps/posix/*.c) \
			$(wildcard $(LOCAL_PATH)/src/platforms/backend-linux/*.c) \
			$(wildcard $(LOCAL_PATH)/src/platforms/backend-posix/*.c) 
			
			
ifeq ($(TARGET_ARCH), arm)
	ZZ_SRC += $(wildcard $(LOCAL_PATH)/src/platforms/arch-arm/*.c) \
			$(wildcard $(LOCAL_PATH)/src/platforms/backend-arm/*.c)
else ifeq ($(TARGET_ARCH), arm64)
	ZZ_SRC += $(wildcard $(LOCAL_PATH)/src/platforms/arch-arm64/*.c) \
			$(wildcard $(LOCAL_PATH)/src/platforms/backend-arm64/*.c)
endif


LOCAL_MODULE := hookzz
LOCAL_C_INCLUDES := $(ZZ_INCLUDE)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SRC_FILES := 	$(ZZ_SRC:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
MAIN_LOCAL_PATH := $(call my-dir)
LOCAL_MODULE := CustomHook
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -fno-rtti -fno-exceptions
LOCAL_SRC_FILES := CustomHook/hookDahuaxiyou.c CustomHook/utils.c CustomHook/backtrace.cpp

LOCAL_LDLIBS :=  -llog
LOCAL_STATIC_LIBRARIES := hookzz

include $(BUILD_SHARED_LIBRARY)