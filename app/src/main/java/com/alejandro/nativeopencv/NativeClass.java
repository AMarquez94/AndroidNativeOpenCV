package com.alejandro.nativeopencv;

public class NativeClass {
    public native static boolean ProcImage(int width, int height,
                                    byte[] NV21FrameData, int [] pixels);

    public native static boolean loadImages();

    public native static boolean FindObjects(int width, int height,
                                             byte[] NV21FrameData, int [] pixels);
}