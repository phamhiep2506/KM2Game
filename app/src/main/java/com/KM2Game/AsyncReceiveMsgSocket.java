package com.KM2Game;

import android.os.AsyncTask;
import android.view.View;
import android.util.Log;

public class AsyncReceiveMsgSocket extends AsyncTask<String, String, String> {

    private native String receiveMsgSocket();
    private View v;

    @Override
    protected String doInBackground(String... params) {
        while(true) {
            Log.i("com.KM2Game", receiveMsgSocket());
        }
    }
}
