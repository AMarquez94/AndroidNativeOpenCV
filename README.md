# NativeOpenCV-310
### Template project for running OpenCV 3.1.0 Native on Android.

This template project will allow you to run native OpenCV code on Android devices via the 
JNI. This project already includes the OpenCV Android Java library as a separate module.

### Prerequisites:
 
1. Android Studio development environment setup with the Android SDK and NDK
 
	- SDK Version: 23
 
	- NDK Version: 1.0
 
 	- Build tools version: 23.0.2
  
2. OpenCV Android Native Library

      This can be installed from [here](http://sourceforge.net/projects/opencvlibrary/files/opencv-android/3.1.0/OpenCV-3.1.0-android-sdk.zip/download).
    
3. System environment variable `OPENCVROOT_ANDROID` pointing to the OPENCV Android Native Library. See [Android.mk](NativeOpenCV-310/app/src/main/jni/Android.mk) for example.
 
I will try to keep this as up-to-date as possible. Currently, the project is using **OpenCV 3.1.0**
