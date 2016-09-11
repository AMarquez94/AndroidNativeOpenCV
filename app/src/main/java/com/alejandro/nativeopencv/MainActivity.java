package com.alejandro.nativeopencv;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.testTextView);
        tv.setText(NativeClass.getStringFromNative());

        Mat image = new Mat();
    }

    static {
        System.loadLibrary("MyLib");
        System.loadLibrary("opencv_java3");
        System.loadLibrary("gnustl_shared");
    }
}