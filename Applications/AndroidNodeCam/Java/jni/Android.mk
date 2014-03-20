LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE  :=  hal
LOCAL_SRC_FILES := libhal.so
LOCAL_CPPFLAGS += -std=c++11
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE  := andcam
LOCAL_SRC_FILES := libAndCam.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := boostthread
LOCAL_SRC_FILES := libboost_thread.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := node
LOCAL_SRC_FILES := libnode.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pbmsgs
LOCAL_SRC_FILES := libpbmsgs.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := rightstdc
LOCAL_SRC_FILES := /Users/faradazerage/code/android-dev/Toolchain/arm-linux-androideabi/lib/armv7-a/libstdc++.a
include $(PREBUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := opencvjava
LOCAL_SRC_FILES := libopencv_java.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gnushared
LOCAL_SRC_FILES := /Users/faradazerage/code/android-dev/Toolchain/arm-linux-androideabi/lib/armv7-a/libgnustl_shared.so 
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := miniglog
LOCAL_SRC_FILES := /Users/faradazerage/code/rslam/rslam/CoreDev/libs/armeabi-v7a/libminiglog.so 
include $(PREBUILT_SHARED_LIBRARY)
    
include $(CLEAR_VARS)
LOCAL_MODULE := AndNodeCam
LOCAL_SRC_FILES := AndNodeCam.cpp
LOCAL_C_INCLUDES := /Users/faradazerage/code/rslam/rslam/CoreDev/HAL
LOCAL_C_INCLUDES += /Users/faradazerage/code/rslam/rslam/CoreDev/HAL/PbMsgs
LOCAL_C_INCLUDES += /Users/faradazerage/code/rslam/rslam/CoreDev/abuild/HAL
LOCAL_C_INCLUDES += /Users/faradazerage/code/android-dev/Toolchain/user/include
LOCAL_C_INCLUDES += /Users/faradazerage/code/android-dev/Toolchain/include/c++/4.8
LOCAL_C_INCLUDES += /Users/faradazerage/code/android-dev/Toolchain/include/c++/4.8/arm-linux-androideabi
LOCAL_SHARED_LIBRARIES := hal
LOCAL_SHARED_LIBRARIES += node
LOCAL_SHARED_LIBRARIES += gnushared
LOCAL_SHARED_LIBRARIES += opencvjava
LOCAL_SHARED_LIBRARIES += miniglog
LOCAL_SHARED_LIBRARIES += pbmsgs
LOCAL_STATIC_LIBRARIES := andcam
LOCAL_STATIC_LIBRARIES += boostthread
LOCAL_STATIC_LIBRARIES += rightstdc
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS := -D__GXX_EXPERIMENTAL_CXX0X__
LOCAL_CPPFLAGS  := -g -std=c++11
include $(BUILD_SHARED_LIBRARY)
