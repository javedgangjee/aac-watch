# Changelog

This file records the changes to yes-no that a person who reads or runs the project would notice.

## Unreleased

### Added

- Feature 1 sets up the ESP-IDF v6.1 project from the Waveshare example `02_lvgl_demo_v9`, with the board support package at version 2 and LVGL 9.5.0.
- Feature 1 adds a green yes screen and a red no screen, and each one shows its word in large white letters in the center.
- Feature 1 makes the watch show the yes screen first after power on and switch between the two screens every 5 seconds.
- Feature 1 turns the screen off after 2 minutes, and a tap on the dark screen turns it back on at the yes screen.
- Feature 1 makes a tap on the lit screen restart the 2 minute count.
- Feature 1 keeps the screen and timer logic in the `screen_cycle` component, and 14 Unity tests run it on the Mac with the ESP-IDF linux target.
