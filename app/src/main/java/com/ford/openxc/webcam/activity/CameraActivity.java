package com.ford.openxc.webcam.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.ford.openxc.webcam.R;
import com.ford.openxc.webcam.webcam.WebcamPreview;

public class CameraActivity extends Activity {
    private EditText mEtVideo;
    private WebcamPreview mPreview;
    private WebcamPreview mPreview2;
    private WebcamPreview mPreview3;
    private WebcamPreview mPreview4;
    private Button mButton;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

       // mEtVideo = (EditText) findViewById(R.id.et_video);

        mPreview = (WebcamPreview) findViewById(R.id.preview);
        mPreview2 = findViewById(R.id.preview2);
        mPreview3 = findViewById(R.id.preview3);
        mPreview4 = findViewById(R.id.preview4);

        mPreview.init(1);
        mPreview2.init(2);
        mPreview3.init(3);
        mPreview4.init(4);


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
}
