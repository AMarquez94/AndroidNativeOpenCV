package com.alejandro.unizar_mobilecv;

public class NativeClass {
    public native static boolean ProcImage(int width, int height,
                                    byte[] NV21FrameData, int [] pixels);

    public native static boolean initRecognizer();

    public native static boolean FindObjects(int width, int height,
                                             byte[] NV21FrameData, int [] pixels);

    public native static void doTest(boolean remote);

    public native static boolean startClient();

    public native static String sendRemote(int width, int height,
                                           byte[] NV21FrameData, int [] pixels);

    public native static boolean endClient();

    public native static void showStats();
}