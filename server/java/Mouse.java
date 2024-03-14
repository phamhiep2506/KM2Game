import android.hardware.input.InputManager;
import java.lang.reflect.Array;

class Mouse
{
    public static void main(String[] args)
    {
        try {
            InputManager inputManager = (InputManager) InputManager.class.getMethod("getInstance").invoke(InputManager.class);
            InputManager.class.getMethod("setPointerIconType", int.class).invoke(inputManager, 0);
            Object obj = InputManager.class.getDeclaredMethod("getInputDeviceIds").invoke(inputManager);
            System.out.println(Array.get(obj, 0));
            System.out.println(Array.get(obj, 1));
            System.out.println(Array.get(obj, 2));
            System.out.println(Array.get(obj, 3));
            System.out.println(Array.get(obj, 4));
            System.out.println(Array.get(obj, 5));
            System.out.println(Array.get(obj, 6));

            InputManager.class.getDeclaredMethod("disableInputDevice", int.class).invoke(inputManager, 26);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
