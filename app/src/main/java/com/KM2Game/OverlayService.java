package com.KM2Game;

import android.app.Service;
import android.os.IBinder;
import android.content.Intent;
import android.content.Context;
import android.view.WindowManager;

public class OverlayService extends Service {

    private WindowManager wm;

    @Override
    public void OnCreate() {
        super.onCreate();

        wm = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
    }

    @Override
    public IBinder onBind(Intent intent) {
    }

    @Override
    public void onDestroy() {
    }
}
