package com.KM2Game;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    static { System.loadLibrary("socket"); }

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // service
        Intent intent = new Intent(this, OverlayService.class);

        // button run game
        Button btnRunGame = findViewById(R.id.btnRunGame);
        btnRunGame.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                v.requestPointerCapture();
                startService(intent);
            }
        });
    }
}
