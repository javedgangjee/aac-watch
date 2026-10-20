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
- Feature 2 makes the watch speak the word on the screen when the user taps the lit screen.
- Feature 2 holds the screen while the word plays, and the next switch comes a full 5 seconds after the word ends.
- Feature 2 makes a tap during a word stop the clip and play the word again from the start.
- Feature 2 adds the script `tools/make_clips.py`, which makes the yes and no clips with the macOS `say` command and the Samantha voice at each clean build.
- Feature 2 plays the clips through the ES8311 codec and the speaker with `espressif/esp_codec_dev` version 1.5 at volume 90.

### Changed

- Feature 2 makes the build need Python 3.13 and the `say` command, and the build fails if either one is missing.
- Feature 2 changes `screen_cycle_tap` to return `SCREEN_TAP_WAKE` or `SCREEN_TAP_SPEAK` in place of a bool, and the host tests now number 21.
