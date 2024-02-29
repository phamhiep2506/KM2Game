package com.KM2Game;

import android.os.AsyncTask;
import android.util.Log;

public class AsyncReceiveMsgSocket extends AsyncTask<Void, String, Void> {

    private native String receiveMsgSocket();

    @Override
    protected Void doInBackground(Void... params) {
        while(true) {
            publishProgress(receiveMsgSocket());
        }
    }

    @Override
    protected void onProgressUpdate(String... values) {
        String msg = values[0];
        Log.i("com.KM2Game", msg);
    }
}
