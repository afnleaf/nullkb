#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

HHOOK keyboardHook;
bool blockNextKey = false;

// Function to check if a key should be blocked
bool shouldBlockKey(DWORD vkCode) {
    // Example: Block the 'A' key (virtual key code 65)
    return vkCode == 65;
    return vkCode == 68;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            printf("Key pressed: %d\n", kbdStruct->vkCode);
            
            if (shouldBlockKey(kbdStruct->vkCode))
            {
                printf("Blocking key: %d\n", kbdStruct->vkCode);
                blockNextKey = true;
                return 1; // Block the key
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            printf("Key released: %d\n", kbdStruct->vkCode);
            
            if (blockNextKey)
            {
                blockNextKey = false;
                return 1; // Block the key-up event for consistency
            }
        }
    }
    
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main()
{
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    
    if (keyboardHook == NULL)
    {
        printf("Failed to install keyboard hook!\n");
        return 1;
    }
    
    printf("Keyboard hook installed. Press any key (Ctrl+C to exit)...\n");
    printf("The 'A' key (vkCode 65) will be blocked as an example.\n");
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}
