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
import android.os.AsyncTask;

public class OverlayService extends Service {

    private WindowManager wm;
    private TextView status;

    private native void createSocket();
    private native boolean connectSocket();
    private native void disconnectSocket();
    private native void sendMsgSocket(String string);

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

        status = new TextView(this);

        WindowManager.LayoutParams params = new WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT);

        params.gravity = Gravity.START | Gravity.TOP;
        params.x = 0;
        params.y = 0;

        wm.addView(status, params);

        createSocket();
        if(connectSocket() == false) {
            status.setText("Disconnect socket");
            status.setTextColor(Color.RED);
        } else {
            status.setText("Connect socket");
            status.setTextColor(Color.GREEN);
            asyncReceiveMsgSocket.execute();
        }

    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        if (status != null) {
            wm.removeView(status);
            status = null;
            disconnectSocket();
            asyncReceiveMsgSocket.cancel(true);
        }
    }

}
