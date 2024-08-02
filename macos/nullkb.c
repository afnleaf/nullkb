#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGEvent.h>
#include <ApplicationServices/ApplicationServices.h>
#include <Quartz/Quartz.h>

CGEventRef myCGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userData) {
    // Handle the event here
    CGEventType eventType = CGEventGetType(event);
    if (eventType == kCGEventKeyDown) {
        int keyCode = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        if (keyCode == 0x00) { // "a"
            printf("Key 'a' pressed\n");
        } else if (keyCode == 0x02) { // "d"
            printf("Key 'd' pressed\n");
        }
    }
    return event;
}

int main() {
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, // tap at the session level
        kCGHeadInsertEventTap, // insert at the head of the tap chain
        0, // no flags
        CGEventMaskBit(kCGEventKeyDown), // mask for key down events
        myCGEventCallback, // callback function
        NULL // no user data
    );

    if (!eventTap) {
        printf("Failed to create event tap\n");
        return 1;
    }

    // Create a CFRunLoopSourceRef to handle keyboard events
    CFRunLoopSourceRef keyboardSource = CFRunLoopSourceCreate(NULL, 0, NULL);

    if (!keyboardSource) {
        printf("Failed to create run loop source\n");
        CFRelease(eventTap);
        return 1;
    }

    // Add the event tap to the run loop source
    CGEventTapEnable(eventTap, true);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), keyboardSource, kCFRunLoopCommonModes);

    // Start the run loop
    CFRunLoopRun();

    // Clean up
    CFRelease(keyboardSource);
    CFRelease(eventTap);

    return 0;
}
