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
import android.view.Gravity;
import android.view.WindowManager;
import android.widget.TextView;
import android.view.View;
import androidx.core.app.NotificationCompat;
import android.util.Log;
import android.widget.ImageView;
import android.view.ViewGroup;

public class OverlayService extends Service {

    private WindowManager wm;
    public TextView status;
    public ImageView pointer;

    private native void createSocket();
    private native boolean connectSocket();
    private native void disconnectSocket();

    AsyncReceiveMsgSocket asyncReceiveMsgSocket = new AsyncReceiveMsgSocket();

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

        wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);

        // Status
        status = new TextView(this);

        WindowManager.LayoutParams statusParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT);
        statusParams.gravity = Gravity.START | Gravity.TOP;
        statusParams.x = 0;
        statusParams.y = 0;

        // Pointer
        pointer = new ImageView(this);
        pointer.setImageResource(R.drawable.ic_pointer);

        WindowManager.LayoutParams pointerParams = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE | WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT);

        pointerParams.gravity = Gravity.START | Gravity.TOP;
        pointerParams.x = 540;
        pointerParams.y = 1170;

        // Add view
        wm.addView(status, statusParams);
        wm.addView(pointer, pointerParams);

        // pointerParams.x = 800;
        // pointerParams.y = 1170;
        // wm.updateViewLayout(pointer, pointerParams);

        // Socket
        createSocket();
        if(connectSocket() == true) {
            status.setText("Socket connected");
            status.setTextColor(Color.GREEN);
            Log.i("KM2Game", "asyncReceiveMsgSocket");
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

    @Override
    public void onDestroy() {
        if (status != null && pointer != null) {
            wm.removeView(status);
            wm.removeView(pointer);
            status = null;
            pointer = null;
            asyncReceiveMsgSocket.cancel(true);
            disconnectSocket();
        }
    }

}
