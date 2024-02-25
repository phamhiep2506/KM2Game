package com.KM2Game;

import android.app.Service;
import android.os.IBinder;
import android.content.Intent;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.support.v4.app.NotificationCompat;
import android.util.Log;

public class OverlayService extends Service {

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("com.KM2Game", "onStartCommand service");

        String CHANNEL_ID = "KM2Game";

        NotificationChannel channel = new NotificationChannel(CHANNEL_ID, "KM2Game overlay notification", NotificationManager.IMPORTANCE_DEFAULT);

        NotificationManager notificationManager = getSystemService(NotificationManager.class);
        notificationManager.createNotificationChannel(channel);

        Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
            .setContentTitle("KM2Game overlay")
            .setContentText("overlay")
            .setSmallIcon(R.drawable.ic_notification)
            .build();

        startForeground(1, notification);

        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

}
