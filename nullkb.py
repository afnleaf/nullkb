import time
from pynput import keyboard
import threading

class KeyboardMonitor:
    def __init__(self):
        self.pressed_keys = set()
        self.listener = None

    def on_press(self, key):
        try:
            self.pressed_keys.add(key.char)
        except AttributeError:
            self.pressed_keys.add(str(key))

    def on_release(self, key):
        try:
            self.pressed_keys.remove(key.char)
        except AttributeError:
            self.pressed_keys.remove(str(key))
        except KeyError:
            # Key wasn't in the set, which is fine
            pass

    def start(self):
        self.listener = keyboard.Listener(on_press=self.on_press, on_release=self.on_release)
        self.listener.start()

    def stop(self):
        if self.listener:
            self.listener.stop()

def background_process():
    monitor = KeyboardMonitor()
    monitor.start()

    try:
        while True:
            # Process the pressed keys here
            if monitor.pressed_keys:
                print(f"Currently pressed keys: {monitor.pressed_keys}")
            time.sleep(0.1)  # Adjust the sleep time as needed
    except KeyboardInterrupt:
        print("Stopping the background process...")
    finally:
        monitor.stop()

if __name__ == "__main__":
    # Start the background process in a separate thread
    bg_thread = threading.Thread(target=background_process)
    bg_thread.start()

    print("Background process started. Press Ctrl+C to stop.")

    try:
        # Your main program can continue here
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Main program interrupted. Waiting for background process to stop...")
        bg_thread.join()
    
    print("Program ended.")

