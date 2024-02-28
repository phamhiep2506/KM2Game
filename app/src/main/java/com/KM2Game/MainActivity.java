package com.KM2Game;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    static { System.loadLibrary("socket"); }

    private native void socket();

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Intent intent = new Intent(this, OverlayService.class);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnStartOverlay = findViewById(R.id.btnStartOverlay);
        btnStartOverlay.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                socket();
                startService(intent);
            }
        });
    }
}
