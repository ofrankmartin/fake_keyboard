#pragma once

#include <memory>
#include <vector>

class FakeKeyboard {
public:
    /// Creates a virtual keyboard and enables the supported key event codes.
    /// @param name Name reported for the virtual input device.
    /// @throws std::runtime_error if the virtual device cannot be initialized.
    explicit FakeKeyboard(const char* name = "fake keyboard");

    /// Destroys the virtual keyboard device.
    ~FakeKeyboard();

    /// Copying is disabled because this object owns a virtual input device.
    /// @param other Source keyboard; copying is not supported.
    FakeKeyboard(const FakeKeyboard&) = delete;

    /// Copy assignment is disabled because this object owns a virtual input device.
    /// @param other Source keyboard; copy assignment is not supported.
    FakeKeyboard& operator=(const FakeKeyboard&) = delete;

    /// Emits a key-down event for the given Linux input key code.
    /// @param key Linux input key code to press.
    void keyDown(int key);

    /// Emits a key-up event for the given Linux input key code.
    /// @param key Linux input key code to release.
    void keyUp(int key);

    /// Presses and immediately releases the given Linux input key code.
    /// @param key Linux input key code to tap.
    void keyPress(int key);

    /// Presses the keys in order, then releases them in reverse order.
    /// @param keys Linux input key codes to press as a combination.
    void keyCombo(const std::vector<int>& keys);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};