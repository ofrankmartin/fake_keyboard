#include "fake_keyboard.h"

#include <chrono>
#include <exception>
#include <linux/input-event-codes.h>
#include <stdio.h>
#include <thread>

int main()
{
    try {
        FakeKeyboard keyboard;

        // Wait for a short period before starting to simulate key presses
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        for (int key = KEY_A; key <= KEY_G; ++key) {
            keyboard.keyPress(key);
        }

        keyboard.keyCombo({ KEY_LEFTALT, KEY_TAB });
    } catch (const std::exception& error) {
        fprintf(stderr, "%s\n", error.what());
        return 1;
    }
    return 0;
}
