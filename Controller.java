package KM2Game;

import android.util.Log;
import android.os.SystemClock;
import android.view.InputDevice;
import android.view.MotionEvent;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Controller {
    private long lastTouchDown;
    private final PointersState pointersState = new PointersState();
    private final MotionEvent.PointerProperties[] pointerProperties = new MotionEvent.PointerProperties[PointersState.MAX_POINTERS];
    private final MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[PointersState.MAX_POINTERS];

    static Method injectInputEventMethod;
    static Object inputManager;

    private void initPointers() {
        for(int i = 0; i < PointersState.MAX_POINTERS; ++i) {
            MotionEvent.PointerProperties props = new MotionEvent.PointerProperties();
            props.toolType = MotionEvent.TOOL_TYPE_FINGER;

            MotionEvent.PointerCoords coords = new MotionEvent.PointerCoords();
            coords.orientation = 0;
            coords.size = 0;

            pointerProperties[i] = props;
            pointerCoords[i] = coords;
        }
    }

    public Controller() {
        initPointers();
    }

    public void injectTouch(int action, int pointerId, float pressure, float x, float y) {
        long now = SystemClock.uptimeMillis();
        Point point = new Point(x, y);

        int pointerIndex = pointersState.getPointerIndex(pointerId);
        if (pointerIndex == -1) {
            System.out.println("Too many pointers for touch event");
        }
        Pointer pointer = pointersState.get(pointerIndex);
        pointer.setPoint(point);
        pointer.setPressure(pressure);
        pointer.setUp(action == MotionEvent.ACTION_UP);

        int source = InputDevice.SOURCE_TOUCHSCREEN;

        int pointerCount = pointersState.update(pointerProperties, pointerCoords);
        if(pointerCount == 1) {
            if(action == MotionEvent.ACTION_DOWN) {
                lastTouchDown = now;
            }
        } else {
            if(action == MotionEvent.ACTION_UP) {
                action = MotionEvent.ACTION_POINTER_UP | (pointerIndex << MotionEvent.ACTION_POINTER_INDEX_SHIFT);
            } else if(action == MotionEvent.ACTION_DOWN) {
                action = MotionEvent.ACTION_POINTER_DOWN | (pointerIndex << MotionEvent.ACTION_POINTER_INDEX_SHIFT);
            }
        }

        MotionEvent event = MotionEvent.obtain(lastTouchDown, now, action, pointerCount, pointerProperties, pointerCoords,
                0, 0, 1f, 1f,
                0, 0, source, 0);

        try {
            injectInputEventMethod.invoke(inputManager, event, 0);
        } catch(IllegalAccessException | InvocationTargetException e) {
            Log.e("KM2Game", "Error inject input event method", e);
        }

    }

    static {
        String methodName = "getInstance";
        Object[] objArr = new Object[0];
        try {
            Class<?> inputManagerClass;
            inputManagerClass = android.hardware.input.InputManager.class;

            inputManager = inputManagerClass.getDeclaredMethod(methodName)
                .invoke(null, objArr);
            //Make MotionEvent.obtain() method accessible
            methodName = "obtain";
            MotionEvent.class.getDeclaredMethod(methodName)
                 .setAccessible(true);

            //Get the reference to injectInputEvent method
            methodName = "injectInputEvent";

            injectInputEventMethod = inputManagerClass.getMethod(methodName, android.view.InputEvent.class, Integer.TYPE);

        } catch(Exception e) {
            Log.e("KM2Game", "Error invoke method inject input event", e);
        }
    }

}
