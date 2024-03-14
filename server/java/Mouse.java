import android.hardware.input.InputManager;

class Mouse
{
    public static void main(String[] args)
    {
        try {
            InputManager inputManager = (InputManager) InputManager.class.getMethod("getInstance").invoke(InputManager.class);
            InputManager.class.getMethod("setPointerIconType", int.class).invoke(inputManager, 0);
        } catch(Exception e) {
            e.printStackTrace();
        }
    }
}
