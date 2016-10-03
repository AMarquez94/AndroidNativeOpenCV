package com.alejandro.nativeopencv;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;

public class CameraActivity extends AppCompatActivity
{
    private CameraPreview camPreview;
    private ImageView MyCameraPreview = null;
    private FrameLayout mainLayout;
    private int previewSizeWidth;
    private int previewSizeHeight;

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        //Set this APK Full screen
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        //Set this APK no title
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.main);

        //
        // Create my camera preview
        //
        MyCameraPreview = new ImageView(this);

        SurfaceView camView = new SurfaceView(this);
        SurfaceHolder camHolder = camView.getHolder();

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);

        this.previewSizeHeight = metrics.heightPixels;
        this.previewSizeWidth = metrics.widthPixels;

        Log.d("TAMAÑO", "Tamaño horizontal " + previewSizeWidth);
        Log.d("TAMAÑO", "Tamaño vertical " + previewSizeHeight);

        this.previewSizeHeight = 480;
        this.previewSizeWidth = 640;

        camPreview = new CameraPreview(previewSizeWidth, previewSizeHeight, MyCameraPreview);

        camHolder.addCallback(camPreview);
        camHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);


        mainLayout = (FrameLayout) findViewById(R.id.frameLayout1);
        mainLayout.addView(camView, new LayoutParams(1, 1));
        mainLayout.addView(MyCameraPreview, new LayoutParams(metrics.widthPixels,  metrics.heightPixels));
    }
    protected void onPause()
    {
        if ( camPreview != null)
            camPreview.onPause();
        super.onPause();
    }
}
