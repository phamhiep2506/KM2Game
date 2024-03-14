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
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;
import androidx.core.app.NotificationCompat;

public class OverlayService extends Service {

    private native void createSocket();
    private native boolean connectSocket();
    private native void disconnectSocket();

    public boolean statusSocket;

    WindowManager wm;
    ImageView pointer;
    WindowManager.LayoutParams params;

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

        params = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                // Full screen
                WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |
                WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN,
            PixelFormat.TRANSLUCENT);
        params.gravity = Gravity.START | Gravity.TOP;

        // Add view
        wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        wm.addView(pointer, params);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (statusSocket == false) {
            // Socket
            createSocket();
            if (connectSocket() == true) {
                statusSocket = true;
                // Receive msg socket
                AsyncReceiveMsgSocket asyncReceiveMsgSocket =
                    new AsyncReceiveMsgSocket(OverlayService.this);
                asyncReceiveMsgSocket.execute();
            } else {
                statusSocket = false;
                Toast.makeText(this, "Error connect socket", Toast.LENGTH_SHORT)
                    .show();
                disconnectSocket();
            }
        }
        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public void updatePointer(int x, int y) {
        params.x = x;
        params.y = y;
        wm.updateViewLayout(pointer, params);
    }
}
