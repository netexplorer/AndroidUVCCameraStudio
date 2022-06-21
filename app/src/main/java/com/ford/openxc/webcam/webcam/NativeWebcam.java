package com.ford.openxc.webcam.webcam;

import android.graphics.Bitmap;
import android.util.Log;
import java.util.ArrayList;

//import com.socks.library.KLog;

import java.io.File;

public class NativeWebcam implements IWebcam {

    private static String TAG = "NativeWebcam";
    private static final int DEFAULT_IMAGE_WIDTH = 1280;
    private static final int DEFAULT_IMAGE_HEIGHT = 720;

    private Bitmap mBitmap;
    private int mWidth;
    private int mHeight;
    private long mhandle;

    private native long startCamera(String deviceName, int width, int height);
    private native void processCamera();
    private native boolean cameraAttached(long handle);
    private native void stopCamera(long handle);
    private native void loadNextFrame(Bitmap bitmap, long handle);

    static {
        System.loadLibrary("webcam");
        System.loadLibrary("yuv");
    }

    public NativeWebcam(String deviceName, int width, int height) {
        mWidth = width;
        mHeight = height;
        mBitmap = Bitmap.createBitmap(mWidth, mHeight, Bitmap.Config.ARGB_8888);
        connect(deviceName, mWidth, mHeight);
    }

    public NativeWebcam(String deviceName) {
        this(deviceName, DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT);
    }

    private void connect(String deviceName, int width, int height) {
        boolean deviceReady = true;

        File deviceFile = new File(deviceName);
        if(deviceFile.exists()) {
            if(!deviceFile.canRead()) {
                Log.w(TAG, "Insufficient permissions on " + deviceName +
                        " -- does the app have the CAMERA permission?");
                //需要有讀許可權。或通過命令列執行chmod命令。否則會出現預覽黑屏
                try {
                    Process su = Runtime.getRuntime().exec("/system/xbin/su");
                    String cmd = "";
                    for (int i = 0; i < 5; ++i)
                        cmd += String.format("chmod 777 dev/video%d\n", i);
                    cmd += "exit\n";
                    su.getOutputStream().write(cmd.getBytes(), 0, cmd.getBytes().length);
                    su.getOutputStream().flush();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            if(!deviceFile.canRead()) {
                Log.w(TAG, "Insufficient permissions on " + deviceName +
                        " -- does the app have the CAMERA permission?");
                deviceReady = false;
            }else{
                deviceReady = true;
            }
        } else {
            Log.w(TAG, deviceName + " does not exist");
            deviceReady = false;
        }

        if(deviceReady) {
            Log.w(TAG, "Preparing camera with device name " + deviceName);
            mhandle = startCamera(deviceName, width, height);
        }
    }

    public Bitmap getFrame() {
        loadNextFrame(mBitmap, mhandle);
        return mBitmap;
    }

    public void stop() {
        stopCamera(mhandle);
    }

    public boolean isAttached() {
        return cameraAttached(mhandle);
    }
}
