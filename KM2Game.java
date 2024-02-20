package KM2Game;

import android.view.MotionEvent;

public class KM2Game {

    static {
        System.load("/data/local/tmp/libKM2Game.so");
    }

    private native void sayHello();

    public static void main(String[] args) throws InterruptedException {

        KM2Game km2game = new KM2Game();

        km2game.sayHello();

    // Input input = new Input();

    // input.injectTouch(MotionEvent.ACTION_DOWN, 1, 1.0f, 250.0f, 500.0f);
    // input.injectTouch(MotionEvent.ACTION_DOWN, 2, 1.0f, 800.0f, 500.0f);

    // input.injectTouch(MotionEvent.ACTION_UP, 1, 1.0f, 250.0f, 500.0f);
    // input.injectTouch(MotionEvent.ACTION_UP, 2, 1.0f, 800.0f, 500.0f);

    }

}
