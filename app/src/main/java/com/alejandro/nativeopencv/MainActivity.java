package com.alejandro.nativeopencv;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity{

    final String TAG = "MainActivity";

    private Button mButtonCamera;
    private Button mButtonTest;

    static {
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("ProcImage");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = (TextView) findViewById(R.id.testTextView);

        mButtonCamera = (Button)this.findViewById(R.id.camera_button);
        mButtonCamera.setOnClickListener(new View.OnClickListener(){
            public void onClick(View arg0){

                Intent intent = new Intent(MainActivity.this, CameraActivity.class);
                startActivity(intent);
            }
        });

        mButtonTest = (Button)this.findViewById(R.id.test_button);
        mButtonTest.setOnClickListener(new View.OnClickListener(){
            public void onClick(View arg0){

                NativeClass.doTest();
            }
        });

        if(isStoragePermissionGranted()){
            NativeClass.initRecognizer();
        }
    }

    public  boolean isStoragePermissionGranted() {
        if (Build.VERSION.SDK_INT >= 23) {
            if ((checkSelfPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED) &&
                    (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
                    == PackageManager.PERMISSION_GRANTED) &&
                    (checkSelfPermission(Manifest.permission.CAMERA)
                    == PackageManager.PERMISSION_GRANTED)){
                Log.v(TAG,"Permission is granted");
                return true;
            } else {

                Log.v(TAG,"Permission is revoked");
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.READ_EXTERNAL_STORAGE,
                                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                Manifest.permission.CAMERA,}, 1);
                return false;
            }
        }
        else { //permission is automatically granted on sdk<23 upon installation
            Log.v(TAG,"Permission is granted");
            return true;
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(grantResults[0]== PackageManager.PERMISSION_GRANTED &&
                grantResults[1] == PackageManager.PERMISSION_GRANTED &&
                grantResults[2] == PackageManager.PERMISSION_GRANTED){
            Log.v(TAG,"Permission: "+permissions[0]+ "was "+grantResults[0]);
            Log.v(TAG,"Permission: "+permissions[1]+ "was "+grantResults[1]);
            Log.v(TAG,"Permission: "+permissions[2]+ "was "+grantResults[2]);
            //resume tasks needing this permission
            NativeClass.initRecognizer();
        }
    }
}