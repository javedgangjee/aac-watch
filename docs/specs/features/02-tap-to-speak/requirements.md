# Feature 2 requirements: tap to speak

## From the specs

1. When the user taps the lit screen, the watch speaks the word on that screen. The source is mission.md, "What it does", and roadmap.md, feature 2.
2. The screen stays put while the word plays. The source is mission.md, "What it does".
3. After the word ends, the screen stays for a full 5 seconds before the next switch. The source is mission.md, "What it does".
4. A tap on the dark screen turns it back on at the yes screen and does not speak. The source is mission.md, "What it does", and roadmap.md, feature 2.
5. A tap on the lit screen restarts the 2 minute count. The source is mission.md, "What it does".
6. The clips for yes and no are made on the Mac at build time with the macOS `say` command. A Python 3.13 script runs `say`, converts the output to raw 16-bit PCM, and writes the files that the build embeds in the firmware flash. The source is tech-stack.md, "Audio clips".
7. The watch plays the clips through the ES8311 codec and the speaker with `espressif/esp_codec_dev` at version 1.5. The source is tech-stack.md, "Firmware", and roadmap.md, feature 2.
8. The new timer rule is part of the screen and timer logic, and Unity tests on the Mac cover it. The source is tech-stack.md, "Tests".
9. The project manifest `main/idf_component.yml` lists `espressif/esp_codec_dev`. The source is feature 1 requirements.md, item 7 under "Not covered by the specs".

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** A tap while the word plays stops the clip and plays the word again from the start. The tap also restarts the 2 minute count. The full 5 seconds start when the last play ends.
2. **Confirmed.** The voice is Samantha, the US English voice that ships with macOS.
3. **Confirmed.** The speaker volume is 90 on the codec scale of 0 to 100.
4. **Confirmed.** The clips are mono, 16-bit, and 22050 Hz. This is the default I2S setting in the board support package. The `say` command writes this format directly, so the script needs no other tool. Each clip is about half a second long and uses about 22 KB of flash.
5. **Confirmed.** The build runs the script on every clean build through a CMake custom command. The clips go into the build folder and are not checked in. The build fails if `say` or Python 3.13 is missing.
6. **Confirmed.** The function `screen_cycle_tap` returns a new type with the values `SCREEN_TAP_WAKE` and `SCREEN_TAP_SPEAK`, in place of a bool. The firmware plays the clip when the result is `SCREEN_TAP_SPEAK`. This changes the expected result of the tap in feature 1 test cases 12, 13, and 14.
7. **Confirmed.** Feature 1 test case 14 changes, because the new rule holds the screen after a tap. The old case expected a switch 2 seconds after a tap. The new case expects the switch 5 seconds after the word ends.
8. **Confirmed.** The 2 minute count keeps running while the word plays. If the count ends during a play, the screen turns off and the hold ends. A tap always restarts the count, so this can happen only if the end of a play is never reported.
9. **Confirmed.** The firmware opens the codec once at start and keeps it open. This avoids a delay and a click at each tap.
10. **Confirmed.** The project uses these file names.
    - The clip script is `tools/make_clips.py`.
    - The playback code is `main/audio.c` and `main/audio.h`.
