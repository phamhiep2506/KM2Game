package com.KM2Game;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import androidx.core.app.NotificationCompat;

public class OverlayService extends Service {

    private String CHANNEL_ID = "KM2Game";
    private int NOTIFICATION_ID = 1;
    private WindowManager wm;
    private TextView t;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onCreate();

        createNotificationChannel();

        Notification notification =
            new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.drawable.ic_notification)
                .setContentTitle("KM2Game")
                .setContentText("Start Overlay")
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                .build();

        startForeground(NOTIFICATION_ID, notification);

        wm = (WindowManager)getSystemService(Context.WINDOW_SERVICE);

        t = new TextView(this);
        t.setText("KM2Game: Off");

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
        wm.addView(t, params);

        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if (t != null) {
            wm.removeView(t);
            t = null;
        }
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is not in the Support Library.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel =
                new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the
            // importance or other notification behaviors after this.
            NotificationManager notificationManager =
                getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }
}
