#include "fake_keyboard.h"

#include <cstring>
#include <libevdev/libevdev-uinput.h>
#include <stdexcept>
#include <string>
#include <vector>

struct FakeKeyboard::Impl {
    libevdev_uinput* device = nullptr;

    explicit Impl(const char* name)
    {
        libevdev* dev = libevdev_new();
        if (!dev) {
            throw std::runtime_error("Failed to allocate libevdev device");
        }

        libevdev_set_name(dev, name);
        int result = libevdev_enable_event_type(dev, EV_KEY);
        for (int code = KEY_ESC; result == 0 && code <= KEY_MAX; ++code) {
            result = libevdev_enable_event_code(dev, EV_KEY, code, nullptr);
        }

        if (result == 0) {
            result = libevdev_uinput_create_from_device(
                dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &device);
        }
        libevdev_free(dev);

        if (result < 0) {
            throw std::runtime_error(
                std::string("Failed to create virtual keyboard: ") + std::strerror(-result));
        }
    }

    ~Impl()
    {
        if (device) {
            libevdev_uinput_destroy(device);
        }
    }
};

FakeKeyboard::FakeKeyboard(const char* name)
    : pimpl(std::make_unique<Impl>(name))
{
}

FakeKeyboard::~FakeKeyboard() = default;

void FakeKeyboard::keyDown(int key)
{
    libevdev_uinput_write_event(pimpl->device, EV_KEY, key, 1);
    libevdev_uinput_write_event(pimpl->device, EV_SYN, SYN_REPORT, 0);
}

void FakeKeyboard::keyUp(int key)
{
    libevdev_uinput_write_event(pimpl->device, EV_KEY, key, 0);
    libevdev_uinput_write_event(pimpl->device, EV_SYN, SYN_REPORT, 0);
}

void FakeKeyboard::keyPress(int key)
{
    keyDown(key);
    keyUp(key);
}

void FakeKeyboard::keyCombo(const std::vector<int>& keys)
{
    for (int key : keys) {
        keyDown(key);
    }
    for (auto it = keys.rbegin(); it != keys.rend(); ++it) {
        keyUp(*it);
    }
}
