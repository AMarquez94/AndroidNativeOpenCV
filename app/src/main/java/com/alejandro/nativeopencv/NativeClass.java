package com.alejandro.nativeopencv;

public class NativeClass {
    public native static boolean ProcImage(int width, int height,
                                    byte[] NV21FrameData, int [] pixels);
}