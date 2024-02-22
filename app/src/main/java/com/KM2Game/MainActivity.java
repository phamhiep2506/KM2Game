package com.KM2Game;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity implements View.OnClickListener {

    Button btnPointerCapture;

    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnPointerCapture = (Button)findViewById(R.id.btnPointerCapture);
        btnPointerCapture.setOnClickListener(this);

    }

    @Override
    public void onClick(View view) {
        view.requestPointerCapture();
    }


}
