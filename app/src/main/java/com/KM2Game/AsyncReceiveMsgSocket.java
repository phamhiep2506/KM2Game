package com.KM2Game;

import android.os.AsyncTask;
import android.util.Log;
import org.json.JSONException;
import org.json.JSONObject;

public class AsyncReceiveMsgSocket extends AsyncTask<Void, String, Void> {

    private native String receiveMsgSocket();
    private int lastX = 540;
    private int lastY = 1170;

    OverlayService overlayService;
    public AsyncReceiveMsgSocket(OverlayService overlayService) {
        this.overlayService = overlayService;
    }

    @Override
    protected Void doInBackground(Void... params) {

        Log.i("com.KM2Game", "Start AsyncTask ReceiveMsgSocket");
        while (true) {
            publishProgress(receiveMsgSocket());
        }
    }

    @Override
    protected void onProgressUpdate(String... values) {
        String msg = values[0];
        Log.i("com.KM2Game", msg);

        try {
            JSONObject pointerJson = new JSONObject(msg);
            Log.i("com.KM2Game", "X: " + pointerJson.getString("x"));
            Log.i("com.KM2Game", "Y: " + pointerJson.getString("y"));
            lastX = lastX + Integer.parseInt(pointerJson.getString("x"));
            lastY = lastY + Integer.parseInt(pointerJson.getString("y"));
            overlayService.updatePointer(lastX, lastY);
        } catch (JSONException e) {
            Log.e("com.KM2Game", e.toString());
        }
    }

}
