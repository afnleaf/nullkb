#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

// global variables for a sort of state
HHOOK keyboardHook;
bool aPressed = false;
bool dPressed = false;
bool aSimulated = false;
bool dSimulated = false;

LRESULT CALLBACK KeebProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if(nCode >= 0) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        
        if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // "A" key 
            if(kbdStruct->vkCode == 65) {
                if(!aPressed && !aSimulated) {
                    printf("A pressed\n");
                    aPressed = true;
                    if(dPressed) {
                        keybd_event(68, 0, KEYEVENTF_KEYUP, 0);  // Release 'D'
                        dSimulated = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            // "D" key
            } else if(kbdStruct->vkCode == 68) {
                if(!dPressed && !dSimulated) {
                    printf("D pressed\n");
                    dPressed = true;
                    if(aPressed) {
                        keybd_event(65, 0, KEYEVENTF_KEYUP, 0);  // Release 'A'
                        aSimulated = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            }
        } else if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            // "A" key
            if(kbdStruct->vkCode == 65) {
                printf("A released\n");
                aPressed = false;
                if(dSimulated) {
                    keybd_event(68, 0, 0, 0);  // Press 'D'
                    dSimulated = false;
                    dPressed = true;
                }
                aSimulated = false;
            // "D" key
            } else if(kbdStruct->vkCode == 68) {
                printf("D released\n");
                dPressed = false;
                if(aSimulated) {
                    keybd_event(65, 0, 0, 0);  // Press 'A'
                    aSimulated = false;
                    aPressed = true;
                }
                dSimulated = false;
            }
        }
    }
    
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeebProc, NULL, 0);

    if(keyboardHook == NULL) {
        printf("Failed to set keyboard hook!\n");
        return 1;
    }

    printf("Keyboard hook set. Press Ctrl+C to exit...\n");
    printf("\"A\" and \"D\" keys will interact: holding one while tapping the other will maintain activation.\n");

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}