cmake_minimum_required(VERSION 3.25)

project(fake_keyboard)

add_executable(fake_keyboard
    src/fake_keyboard.cpp
    src/main.cpp
)

target_sources(fake_keyboard PRIVATE src/fake_keyboard.h)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
target_link_libraries(fake_keyboard PRIVATE PkgConfig::LIBEVDEV)
