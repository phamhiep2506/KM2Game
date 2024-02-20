package KM2Game;

import android.view.MotionEvent;

public class KM2Game {

    public static void main(String[] args) {

        Input input = new Input();

        input.injectTouch(MotionEvent.ACTION_DOWN, 1, 1.0f, 1970, 800);

        input.injectTouch(MotionEvent.ACTION_MOVE, 1, 1.0f, 1950 , 800);

        // input.injectTouch(MotionEvent.ACTION_UP, 1, 0.0f, 1950, 800);

    }

}
