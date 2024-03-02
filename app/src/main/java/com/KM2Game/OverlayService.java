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
import android.widget.TextView;
import androidx.core.app.NotificationCompat;

public class OverlayService extends Service {

    private native void createSocket();
    private native boolean connectSocket();

    WindowManager wm;
    TextView status;
    ImageView pointer;
    WindowManager.LayoutParams statusParams;
    WindowManager.LayoutParams pointerParams;

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

        // Status
        status = new TextView(this);

        statusParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT);
        statusParams.gravity = Gravity.START | Gravity.TOP;

        // Pointer
        pointer = new ImageView(this);
        pointer.setImageResource(R.drawable.ic_pointer);
        pointer.setId(123);

        pointerParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT);
        pointerParams.gravity = Gravity.START | Gravity.TOP;

        pointerParams.x = 540;
        pointerParams.y = 1170;

        // Add view
        wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        wm.addView(status, statusParams);
        wm.addView(pointer, pointerParams);

        // Socket
        createSocket();
        if (connectSocket() == true) {
            status.setText("Socket connected");
            status.setTextColor(Color.GREEN);

            AsyncReceiveMsgSocket asyncReceiveMsgSocket = new AsyncReceiveMsgSocket(this);
            asyncReceiveMsgSocket.execute();
        } else {
            status.setText("Socket disconnected");
            status.setTextColor(Color.RED);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public void updatePointer(int x, int y) {
        pointerParams.x = x;
        pointerParams.y = y;
        wm.updateViewLayout(pointer, pointerParams);
    }

}
