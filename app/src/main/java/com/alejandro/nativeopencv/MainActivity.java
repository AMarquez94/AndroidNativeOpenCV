package com.alejandro.nativeopencv;

import android.content.Intent;
import android.hardware.Camera;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import org.opencv.core.Mat;

public class MainActivity extends AppCompatActivity {

    private Button mButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.testTextView);
        tv.setText(NativeClass.getStringFromNative());

        Mat image = new Mat();

        mButton = (Button)this.findViewById(R.id.camera_button);
        mButton.setOnClickListener(new View.OnClickListener(){
            public void onClick(View arg0){

                Intent intent = new Intent(MainActivity.this, CameraActivity.class);
                startActivity(intent);
            }
        });
    }

    static {
        System.loadLibrary("MyLib");
        System.loadLibrary("opencv_java3");
        System.loadLibrary("gnustl_shared");
    }
}