import android.hardware.input.InputManager;
import android.view.InputDevice;
import java.util.Scanner;

class Device {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int[] devices;
        int choose;
        int devId;

        try {
            InputManager inputManager =
                (InputManager)InputManager.class.getMethod("getInstance")
                    .invoke(InputManager.class);

            // Show input devices
            devices = InputDevice.getDeviceIds();

            for (int i = 0; i < devices.length; i++) {
                System.out.println(
                    "Device ID: " + InputDevice.getDevice(devices[i]).getId() +
                    " => " +
                    InputDevice.getDevice(devices[i]).getName().trim());
            }

            // Get choose
            System.out.print("Enable Device = 1, Disable Device = 0 : ");
            choose = scanner.nextInt();
            System.out.print("Input Device ID: ");
            devId = scanner.nextInt();

            switch (choose) {
            case 1:
                // Enable Input Device
                InputManager
                    .class.getDeclaredMethod("enableInputDevice", int.class)
                    .invoke(inputManager, devId);
                break;
            case 0:
                // Disable Input Device
                InputManager
                    .class.getDeclaredMethod("disableInputDevice", int.class)
                    .invoke(inputManager, devId);
                break;
            default:
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
