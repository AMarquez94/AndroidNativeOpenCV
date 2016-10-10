LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
#OPENCVROOT_ANDROID is a system environment variable that points toe OpenCv-android-sdk
#example: OPENCVROOT_ANDROID = C:\Users\{USERNAME}\AppData\Local\Android\OpenCv-android-sdk

OPENCVROOT:= /home/alejandro/Android/OpenCv
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk



LOCAL_MODULE := ProcImage
LOCAL_SRC_FILES := com_alejandro_nativeopencv_NativeClass.cpp ./structures/Object.cpp ./structures/Recognizer.cpp ./structures/Tester.cpp ./structures/TestImage.cpp ./structures/utils.cpp ./structures/Result.cpp
LOCAL_LDLIBS += -llog
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true

include $(BUILD_SHARED_LIBRARY)