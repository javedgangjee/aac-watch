# Changelog

This file records the changes to aac-watch that a person who reads or runs the project would notice.

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
- Feature 3 adds a settings screen, which a person opens with a swipe down that starts in the top 60 pixels of the lit yes or no screen.
- Feature 3 lets a person choose the Samantha voice or the Reed voice, set the cycle time from 3 to 15 seconds, and set the volume from 10 to 100 in steps of 10.
- Feature 3 dims the minus or plus button when a value is at its limit.
- Feature 3 closes the settings screen on a tap on Done, on a swipe up that starts in the bottom 60 pixels, or after 30 seconds with no touch, and then shows the yes screen for one full cycle time.
- Feature 3 stops the screen switch and the 2 minute count while the settings screen shows.
- Feature 3 saves the settings in the NVS flash partition, so they stay after a restart, and a missing or out of range value falls back to its default.
- Feature 3 adds the `settings` component, and the host tests now number 39.
- Feature 3 adds the Atkinson Hyperlegible font for the settings screen, with the source TTF files and the SIL Open Font License in `tools/fonts/`.
- Feature 4 adds a `LICENSE` file with the GNU General Public License version 3.0 text, and the project is under GPL 3.0 or any later version.
- Feature 4 adds a README that says what the watch does and names the hardware. It also says what the build machine needs, how to build, flash, and use the watch, and gives the license.
- Feature 5 adds a split mode, which shows "Yes" on green in the top half of the screen and "No" on red in the bottom half, with a black band between them, and the screen does not switch.
- Feature 5 makes a tap on a half speak the word of that half, and a tap on the other half while a word plays stops the clip and plays the new word.
- Feature 5 makes a tap on the band of the lit screen do nothing, and it does not restart the 2 minute count.
- Feature 5 turns the split screen off after 2 minutes with no tap, and a tap on the dark screen turns it back on at the split screen and does not speak.
- Feature 5 adds a Mode row at the top of the settings screen with a Cycle button and a Split button, and the default is Cycle.
- Feature 5 hides the cycle time row on the settings screen while Split is lit.
- Feature 5 saves the mode in NVS with the key `mode`, so a watch with saved settings from feature 3 starts in cycle mode and keeps its other settings.
- Feature 6 adds a white check mark icon to the yes screen and a white X icon to the no screen, made from the Font Awesome 5 font that ships with LVGL.
- Feature 6 puts the icon 32 pixels above the word in cycle mode, and to the left of the word on the same line in split mode.
- Feature 6 makes a tap on an icon act as a tap on its screen or its half.
- Feature 6 adds an Icons row with a Show button and a Hide button as the second row of the settings screen, and the default is Show.
- Feature 6 saves the icons setting in NVS with the key `icons`, so a watch with saved settings from feature 5 shows the icons and keeps its other settings.
- Feature 6 adds the Font Awesome source font and its license in `tools/fonts/`, and the README credits Font Awesome.
- Feature 7 makes a tap that speaks turn the tapped screen white, with the word and the icon in green on the yes screen or red on the no screen, until the word ends.
- Feature 7 swaps only the tapped half in split mode, and a tap on the other half while a word plays puts the first half back and swaps the new half.
- Feature 7 puts the normal colors back when the word ends, when the screen turns off, or when the settings screen closes, and a tap on the dark screen does not swap the colors.
- Feature 7 adds the functions `screen_cycle_tap_word` and `screen_cycle_spoken` to the `screen_cycle` component, and the host tests now number 69.

### Changed

- Feature 2 makes the build need Python 3.13 and the `say` command, and the build fails if either one is missing.
- Feature 2 changes `screen_cycle_tap` to return `SCREEN_TAP_WAKE` or `SCREEN_TAP_SPEAK` in place of a bool, and the host tests now number 21.
- Feature 3 makes `tools/make_clips.py` write four clips, which are `yes_samantha.pcm`, `no_samantha.pcm`, `yes_reed.pcm`, and `no_reed.pcm`.
- Feature 3 makes the cycle time a setting, and `screen_cycle_init` now takes the period, where a period of 0 means the default of 5 seconds.
- Feature 3 makes `audio_init` take the start volume and makes `audio_play` take the voice.
- Feature 4 renames the project from yes-no to aac-watch, so the firmware file is now `build/aac_watch.bin`.
- Feature 4 changes the settings namespace in NVS from `yes_no` to `aac_watch`, so a watch that already has saved settings goes back to the defaults one time.
- Feature 5 makes every button on the settings screen 48 pixels high in place of 56, so the four rows and Done fit on the screen.
- Feature 5 makes the README describe the two modes and the four settings, and the host tests now number 56.
- Feature 6 makes the settings screen scroll up and down, with each row at a fixed height of 81 pixels and a thin grey line between rows, and the list starts at the top each time the screen opens.
- Feature 6 makes a press that starts in the bottom 60 pixels of the settings screen leave the list still, so the swipe up closes the screen at any scroll position.
- Feature 6 makes the rows below the cycle time row move up in split mode and keep their height, in place of the even spread from feature 5.
- Feature 6 makes the README describe the icons setting and the scrolling settings screen, and the host tests now number 60.
- Feature 7 makes the README say that the tapped screen or half shows white with the word in green or red while the word plays.
- Split mode makes the word 25% smaller. The word font goes from size 160 to size 120 in the new file `main/font_yes_no_120.c`, and the icon keeps size 80.

### Removed

- Feature 4 removes `.vscode/settings.json` from git and adds `.vscode/` and `.ruff_cache/` to `.gitignore`.
