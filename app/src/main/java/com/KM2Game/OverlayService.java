package com.KM2Game;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.core.app.NotificationCompat;

public class OverlayService extends Service {

    private native void createSocket();
    private native boolean connectSocket();

    WindowManager wm;
    ImageView pointer;
    Button button;
    WindowManager.LayoutParams pointerParams;
    WindowManager.LayoutParams buttonParams;

    @Override
    public void onCreate() {

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {

            String CHANNEL_ID = "KM2Game";
            NotificationChannel channel =
                new NotificationChannel(CHANNEL_ID, "KM2Game Overlay",
                                        NotificationManager.IMPORTANCE_DEFAULT);

            ((NotificationManager)getSystemService(
                 Context.NOTIFICATION_SERVICE))
                .createNotificationChannel(channel);

            Notification notification =
                new NotificationCompat.Builder(this, CHANNEL_ID)
                    .setContentTitle("KM2Game")
                    .setContentText("Start Overlay")
                    .setSmallIcon(R.drawable.ic_notification)
                    .build();

            startForeground(1, notification);
        }

        // Pointer
        pointer = new ImageView(this);
        pointer.setImageResource(R.drawable.ic_pointer);

        pointerParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                // Full screen
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
            PixelFormat.TRANSLUCENT);
        pointerParams.gravity = Gravity.START | Gravity.TOP;

        pointerParams.x = 0;
        pointerParams.y = 0;

        // Button
        button = new Button(this);
        button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                v.requestPointerCapture();
            }
        });
        button.setAlpha(0.2f);

        buttonParams = new WindowManager.LayoutParams(
            100, // Width
            100, // Height
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                // Full screen
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
            PixelFormat.TRANSLUCENT);
        buttonParams.gravity = Gravity.LEFT | Gravity.BOTTOM;

        // Socket
        createSocket();
        if (connectSocket() == true) {
            // Add view
            wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
            wm.addView(pointer, pointerParams);
            wm.addView(button, buttonParams);

            // Receive msg socket
            AsyncReceiveMsgSocket asyncReceiveMsgSocket =
                new AsyncReceiveMsgSocket(this);
            asyncReceiveMsgSocket.execute();
        } else {
            stopSelf();
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public void stopOverlay() {
        if (pointer != null && button != null) {
            wm.removeView(pointer);
            wm.removeView(button);
            pointer = null;
            button = null;
            stopSelf();
        }
    }

    public void updatePointer(int x, int y) {
        pointerParams.x = x;
        pointerParams.y = y;
        wm.updateViewLayout(pointer, pointerParams);
    }
}
