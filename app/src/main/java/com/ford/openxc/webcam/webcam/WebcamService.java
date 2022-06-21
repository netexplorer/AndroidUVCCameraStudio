package com.ford.openxc.webcam.webcam;

import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

//import com.socks.library.KLog;

public class WebcamService extends Service {
    private final static String TAG = "WebcamManager";

    // USB攝像頭設備
    public final static String VIDEO = "/dev/video0";
    public final static String VIDEO1 = "/dev/video2";
    public final static String VIDEO2 = "/dev/video3";
    public final static String VIDEO3 = "/dev/video4";

    private String video = VIDEO;

    private final IBinder mBinder = new WebcamBinder();
    private IWebcam mWebcam;

    private IWebcam mWebcam2;
    private IWebcam mWebcam3;
    private IWebcam mWebcam4;

    public class WebcamBinder extends Binder {
        public WebcamService getService() {
            return WebcamService.this;
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.w(TAG, "Service onStartCommand");
        video = intent.getStringExtra("video");

        Log.w(TAG, "video: " + video);

        // TODO: 16-3-22 修改video設備號
        mWebcam = new NativeWebcam(video);

        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onCreate() {
        Log.w(TAG, "Service onCreate");
        super.onCreate();

        // TODO: 16-3-22 修改video設備號

        mWebcam = new NativeWebcam(VIDEO);
        mWebcam2 = new NativeWebcam(VIDEO1);
        mWebcam3 = new NativeWebcam(VIDEO2);
        mWebcam4 = new NativeWebcam(VIDEO3);

    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.w(TAG, "Service being destroyed");
        mWebcam.stop();
        mWebcam2.stop();
        mWebcam3.stop();
        mWebcam4.stop();
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.w(TAG, "Service binding in response to " + intent);
        return mBinder;
    }

    public Bitmap getFrame() {
/*      if (mWebcam == null) {
            return null;
        }*/

        if (!mWebcam.isAttached()) {
            stopSelf();
        }
        return mWebcam.getFrame();
    }

    public Bitmap getFrame2() {
/*      if (mWebcam == null) {
            return null;
        }*/

        if (!mWebcam2.isAttached()) {
            stopSelf();
        }
        return mWebcam2.getFrame();
    }

    public Bitmap getFrame3() {
/*      if (mWebcam == null) {
            return null;
        }*/

        if (!mWebcam3.isAttached()) {
            stopSelf();
        }
        return mWebcam3.getFrame();
    }

    public Bitmap getFrame4() {
/*      if (mWebcam == null) {
            return null;
        }*/

        if (!mWebcam4.isAttached()) {
            stopSelf();
        }
        return mWebcam4.getFrame();
    }
}
