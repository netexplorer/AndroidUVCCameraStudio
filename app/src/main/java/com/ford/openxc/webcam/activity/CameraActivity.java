package com.ford.openxc.webcam.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.os.Build;
import android.os.Handler;
import android.os.Message;

import android.os.Bundle;
//import android.support.v4.app.ActivityCompat;
//import android.support.v4.content.ContextCompat;
import android.util.DisplayMetrics;
import android.util.Log;

import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import java.io.IOException;
import java.util.List;


import com.ford.openxc.webcam.R;
import com.ford.openxc.webcam.webcam.WebcamPreview;

public class CameraActivity extends Activity {
    private EditText mEtVideo;
    private WebcamPreview mPreview1;
    private WebcamPreview mPreview2;
    private WebcamPreview mPreview3;
    private WebcamPreview mPreview4;
    private Button mButton;
    private SurfaceView sView0, sView1, sView2, sView3;
    private SurfaceHolder sHolder0, sHolder1, sHolder2, sHolder3;
    int mScreenWidth;
    int mScreenHeight;
    private final static String TAG = "CameraActivity";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams. FLAG_FULLSCREEN ,
                WindowManager.LayoutParams. FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main);

       // mEtVideo = (EditText) findViewById(R.id.et_video);

        mPreview1 = (WebcamPreview) findViewById(R.id.preview);
        mPreview2 = findViewById(R.id.preview2);
        mPreview3 = findViewById(R.id.preview3);
        mPreview4 = findViewById(R.id.preview4);

        mPreview1.init(1);
        mPreview2.init(2);
        mPreview3.init(3);
        mPreview4.init(4);
/*
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (ContextCompat.checkSelfPermission(CameraActivity.this, Manifest.permission.CAMERA)!= PackageManager.PERMISSION_GRANTED ||
                    ContextCompat.checkSelfPermission(CameraActivity.this,Manifest.permission.WRITE_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED){
                //没有权限则申请权限
                ActivityCompat.requestPermissions(CameraActivity.this,new String[]{Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE},1);
                Log.e(TAG, "无访摄像头权限，申请");

            }else {
                Log.e(TAG, "已经获摄像头权限");
                //有权限直接执行,docode()不用做处理

            }
        }else {
            Log.e(TAG, "小于6.0，不用申请权限，直接执行");
        }
*/
        DisplayMetrics displayMetrics = new DisplayMetrics();
        this.getWindow().getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        mScreenWidth = displayMetrics.widthPixels;
        mScreenHeight = displayMetrics.heightPixels;
        Log.e(TAG, "w=" + mScreenWidth + ",H=" + mScreenHeight);

        //mButton = (Button) findViewById(R.id.bt_preview);
        /*
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPreview.stopPreview();
               // mPreview.startPreview(mEtVideo.getText().toString());
            }
        });
        */
    }

    @Override
    protected void onResume() {
        super.onResume();
        resizeSview(mPreview1, 0, 0, mScreenWidth/2-2, mScreenHeight/2-2);
        resizeSview(mPreview2, mScreenWidth/2+2, 0, mScreenWidth-2, mScreenHeight/2-2);
        resizeSview(mPreview3, 0, mScreenHeight/2+2, mScreenWidth/2-2, mScreenHeight);
        resizeSview(mPreview4, mScreenWidth/2+2, mScreenHeight/2+2, mScreenWidth, mScreenHeight);
        mPreview1.setZOrderOnTop(true);
        mPreview2.setZOrderOnTop(true);
        mPreview3.setZOrderOnTop(true);
        mPreview4.setZOrderOnTop(true);
    }

    public void resizeSview(SurfaceView view, int x1, int y1, int x2, int y2){

        RelativeLayout.LayoutParams params  =
                new RelativeLayout.LayoutParams(x2, y2);

        params.leftMargin = x1;
        params.topMargin  = y1;

        view.setLayoutParams(params);

    }

}
