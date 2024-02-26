package com.KM2Game;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {

        Intent intent = new Intent(this, OverlayService.class);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btnStartOverlay = (Button)findViewById(R.id.btnStartOverlay);
        btnStartOverlay.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                startService(intent);
            }
        });
    }
}
