package KM2Game;

import android.os.SystemClock;
import android.view.MotionEvent;

public class Controller {

    private static final int DEFAULT_DEVICE_ID = 0;

    private long lastTouchDown;
    private final PointersState pointersState = new PointersState();
    private final MotionEvent.PointerProperties[] pointerProperties = new MotionEvent.PointerProperties[PointersState.MAX_POINTERS];
    private final MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[PointersState.MAX_POINTERS];

    private boolean injectTouch(int action, long pointerId, float pressure, float x, float y) {
        long now = SystemClock.uptimeMillis();
        return true;
    }

}
