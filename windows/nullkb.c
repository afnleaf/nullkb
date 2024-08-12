#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

// enum
enum VirtualKey {
    VK_A = 0x41, // 65
    VK_D = 0x44, // 68
    VK_W = 0x57, // 87
    VK_S = 0x53  // 83
};

// global variables for a sort of state
HHOOK keyboardHook;
// side-to-side
bool aPressed = false;
bool dPressed = false;
bool aSim = false;
bool dSim = false;
// forward and backward
bool wPressed = false;
bool sPressed = false;
bool wSim = false;
bool sSim = false;


// Recursive process to check for keyboard events and respond
LRESULT CALLBACK kbProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if(nCode >= 0) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        
        // Keydown event
        if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            
            // Lateral movement
            // Check "A" key 
            if(kbdStruct->vkCode == VK_A) {
                if(!aPressed && !aSim) {
                    printf("A pressed\n");
                    aPressed = true;
                    if(dPressed) {
                        // Release "D"
                        keybd_event(VK_D, 0, KEYEVENTF_KEYUP, 0);
                        dSim = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            // Check "D" key
            } else if(kbdStruct->vkCode == VK_D) {
                if(!dPressed && !dSim) {
                    printf("D pressed\n");
                    dPressed = true;
                    if(aPressed) {
                        // Release "A"
                        keybd_event(VK_A, 0, KEYEVENTF_KEYUP, 0);
                        aSim = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            }

            // Forward and Backward movement
            // Check "W" key
            if(kbdStruct->vkCode == VK_W) {
                if(!wPressed && !wSim) {
                    printf("W pressed\n");
                    wPressed = true;
                    if(sPressed) {
                        // Release "S"
                        keybd_event(VK_S, 0, KEYEVENTF_KEYUP, 0);
                        sSim = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            // Check "S" key
            } else if(kbdStruct->vkCode == VK_S) {
                if(!sPressed && !sSim) {
                    printf("S pressed\n");
                    sPressed = true;
                    if(wPressed) {
                        // Release "W"
                        keybd_event(VK_W, 0, KEYEVENTF_KEYUP, 0);
                        wSim = true;
                    }
                }
                return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
            }

        // Keyup event
        } else if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            
            // Lateral movement
            // Check "A" key
            if(kbdStruct->vkCode == VK_A) {
                printf("A released\n");
                aPressed = false;
                if(dSim) {
                    // Press "D"
                    keybd_event(VK_D, 0, 0, 0);
                    dSim = false;
                    dPressed = true;
                }
                aSim = false;
            // Check "D" key
            } else if(kbdStruct->vkCode == VK_D) {
                printf("D released\n");
                dPressed = false;
                if(aSim) {
                    // Press "A"
                    keybd_event(VK_A, 0, 0, 0);
                    aSim = false;
                    aPressed = true;
                }
                dSim = false;
            }

            // Forward and Backward movement
            // Check "W" key
            if(kbdStruct->vkCode == VK_W) {
                printf("W released\n");
                wPressed = false;
                if(sSim) {
                    // Press "S"
                    keybd_event(VK_S, 0, 0, 0);
                    sSim = false;
                    sPressed = true;
                }
                wSim = false;
            // Check "S" key
            } else if(kbdStruct->vkCode == VK_S) {
                printf("S released\n");
                sPressed = false;
                if(wSim) {
                    // Press "W"
                    keybd_event(VK_W, 0, 0, 0);
                    wSim = false;
                    wPressed = true;
                }
                sSim = false;
            }
        }
    }
    
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, kbProc, NULL, 0);

    if(keyboardHook == NULL) {
        printf("Failed to set keyboard hook!\n");
        return 1;
    }

    printf("Keyboard hook set. Press Ctrl+C to exit...\n");
    printf("(\"A\", \"D\") and (\"W\", \"S\") keys will interact as pairs: holding one while tapping the other will maintain activation.\n");

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
