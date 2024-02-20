package KM2Game;

import android.view.MotionEvent;

public class KM2Game {

    public static void main(String[] args) throws InterruptedException {

    Controller controller = new Controller();

    controller.injectTouch(MotionEvent.ACTION_DOWN, 1, 1.0f, 1170.0f, 540.0f);
    float i = 1160.0f;
    while(i > -5000.0f) {
        Thread.sleep(10);
        i = i - 10.0f;
        controller.injectTouch(MotionEvent.ACTION_MOVE, 1, 1.0f, i, 540.0f);
    }
    controller.injectTouch(MotionEvent.ACTION_UP, 1, 1.0f, 1170.0f, 540.0f);

    }

}
