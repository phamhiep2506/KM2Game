package KM2Game;

import android.view.MotionEvent;

public class KM2Game {

    public static final int UP = 0, DOWN = 1, MOVE = 2;

    static {
        System.load("/data/local/tmp/libKM2Game.so");
    }

    private native void startConvert();

    public void injectEvent(float x, float y, int action, int pointerId) {
        Input input = new Input();
        switch(action) {
            case UP:
                input.injectTouch(MotionEvent.ACTION_UP, pointerId, 0.0f, x, y);
                break;
            case DOWN:
                input.injectTouch(MotionEvent.ACTION_DOWN, pointerId, 1.0f, x, y);
                break;
            case MOVE:
                input.injectTouch(MotionEvent.ACTION_MOVE, pointerId, 1.0f, x, y);
                break;
        }
    }

    public static void main(String[] args) throws InterruptedException {

        KM2Game km2game = new KM2Game();

        km2game.startConvert();

    }

}
