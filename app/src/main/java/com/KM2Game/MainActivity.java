package com.KM2Game;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Color;

public class MainActivity extends AppCompatActivity {

    static { System.loadLibrary("socket"); }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // service
        Intent intent = new Intent(this, OverlayService.class);

        // button view
        Button btnRunGame = findViewById(R.id.btnRunGame);

        Button btnStopOverlay = findViewById(R.id.btnStopOverlay);

        btnRunGame.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startService(intent);
            }
        });

        btnStopOverlay.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                stopService(intent);
            }
        });
    }
}
