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

#NDK_TOOLCHAIN_VERSION = 4.9

LOCAL_MODULE := ProcImage
LOCAL_CFLAGS += -O3
LOCAL_CPPFLAGS += -O3
LOCAL_ARM_NEON := true
LOCAL_SRC_FILES :=\
  com_alejandro_unizar_mobilecv_NativeClass.cpp \
  ./structures/Recognizer.cpp \
  ./structures/RecognizerDl.cpp \
  ./structures/Tester.cpp \
  ./structures/TestImage.cpp \
  ./structures/utils.cpp \
  ./structures/Result.cpp \
  ./structures/Network.cpp \
  ./structures/Time.cpp \
  ./structures/Timer.cpp \
  ./structures/Object.cpp \
  ./structures/ObjectKp.cpp \
  ./structures/ObjectDl.cpp \
  ./structures/ProtoResult.pb.cc
LOCAL_LDLIBS += -llog -lz
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true
LOCAL_STATIC_LIBRARIES += libprotobuf #boost_atomic_static

include $(BUILD_SHARED_LIBRARY)

#Add the protobuf module route
$(call import-add-path, /home/alejandro/Android/modules)
$(call import-module,protobuf-android)
#$(call import-module,boost/1.59.0)
