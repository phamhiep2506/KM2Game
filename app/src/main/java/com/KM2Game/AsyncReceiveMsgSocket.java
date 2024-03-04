package com.KM2Game;

import android.content.Intent;
import android.os.AsyncTask;
import android.util.Log;
import android.view.View;
import org.json.JSONException;
import org.json.JSONObject;

public class AsyncReceiveMsgSocket extends AsyncTask<Void, String, Void> {

    private native String receiveMsgSocket();

    OverlayService overlayService;
    public AsyncReceiveMsgSocket(OverlayService overlayService) {
        this.overlayService = overlayService;
    }

    @Override
    protected Void doInBackground(Void... params) {

        Log.i("com.KM2Game", "Start AsyncTask ReceiveMsgSocket");

        while (!isCancelled()) {
            publishProgress(receiveMsgSocket());
        }
        return null;
    }

    @Override
    protected void onProgressUpdate(String... values) {
        String msg = values[0];

        try {
            JSONObject pointerJson = new JSONObject(msg);
            if (Integer.parseInt(pointerJson.getString("mouse")) == 0) {
                overlayService.pointer.setVisibility(View.GONE);
            }
            if (Integer.parseInt(pointerJson.getString("mouse")) == 1) {
                overlayService.pointer.setVisibility(View.VISIBLE);
                overlayService.updatePointer(
                    Integer.parseInt(pointerJson.getString("y")),
                    Integer.parseInt(pointerJson.getString("x")));
            }
        } catch (JSONException e) {
            cancel(true);
            overlayService.stopOverlay();
            Log.e("com.KM2Game", e.toString());
        }
    }
}
