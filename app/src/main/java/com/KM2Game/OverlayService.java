package com.KM2Game;

import android.app.Service;
import android.os.IBinder;
import android.content.Intent;
import android.content.Context;
import android.util.Log;

public class OverlayService extends Service {

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("com.KM2Game", "onStartCommand service");
        return 0;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

}
