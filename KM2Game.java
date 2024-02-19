package KM2Game;

import android.view.MotionEvent;

public class KM2Game {

    public static void main(String[] args) {

        System.out.println("Hello, World!");

        Input.injectTouch(MotionEvent.ACTION_DOWN, 0, 10, 540, 1170);
        Input.injectTouch(MotionEvent.ACTION_UP, 0, 0, 540, 1170);

    }

}
