package com.KM2Game;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import androidx.appcompat.app.AppCompatActivity;
import android.view.PointerIcon;

public class MainActivity extends AppCompatActivity {

    static { System.loadLibrary("socket"); }

    private native void createSocket();
    private native boolean connectSocket();
    private native void disconnectSocket();

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // create socket
        createSocket();

        // service
        Intent intent = new Intent(this, OverlayService.class);

        // button
        Button btnRunGame = findViewById(R.id.btnRunGame);

        Button btnConnectSocket = findViewById(R.id.btnConnectSocket);
        Button btnDisconnectSocket = findViewById(R.id.btnDisconnectSocket);

        Button btnStartOverlay = findViewById(R.id.btnStartOverlay);
        Button btnStopOverlay = findViewById(R.id.btnStopOverlay);

        btnConnectSocket.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                connectSocket();
            }
        });

        btnDisconnectSocket.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                disconnectSocket();
            }
        });

        btnStartOverlay.setOnClickListener(new View.OnClickListener() {
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
