# Feature 3 plan: settings screen

1. Make the clips for both voices.
   - This step changes `tools/make_clips.py`.
   - The script keeps a map from the voice key to the `say` voice name. The key `samantha` maps to `Samantha`, and the key `reed` maps to `Reed (English (US))`.
   - For each voice and each word, the script writes `<word>_<voice>.pcm` to the output folder. The format check stays as it is.

2. Make the cycle time a setting in the screen and timer logic.
   - This step changes `components/screen_cycle/include/screen_cycle.h` and `components/screen_cycle/screen_cycle.c`.
   - The constant `SCREEN_CYCLE_PERIOD_MS` becomes `SCREEN_CYCLE_DEFAULT_PERIOD_MS`, with the same value of 5000. The struct adds the field `period_ms`.
   - The function `screen_cycle_init` sets `period_ms` to the default. The function `screen_cycle_tick` uses `period_ms` in place of the constant.
   - The function `screen_cycle_tap` keeps `period_ms` when it wakes the dark screen.
   - The new function `screen_cycle_set_period` sets `period_ms` and sets the switch count to zero.

3. Write the settings logic.
   - This step creates `components/settings/CMakeLists.txt`, `components/settings/include/settings.h`, and `components/settings/settings.c`.
   - The header defines the type `settings_voice_t` with the values `SETTINGS_VOICE_SAMANTHA` and `SETTINGS_VOICE_REED`. It defines the struct `settings_t` with the fields `voice`, `cycle_s`, and `volume`. It defines the limits, the step sizes, and the defaults as constants.
   - The function `settings_defaults` sets Samantha, 5 seconds, and volume 90.
   - The functions `settings_cycle_step` and `settings_volume_step` take a direction of +1 or -1. Each one moves the value one step and stays inside the limits. Each one returns true if the value changed.
   - The function `settings_sanitize` sets each field that is out of range to its default. A volume that is not a multiple of 10 is out of range. The function returns true if a field changed.
   - The function `settings_cycle_ms` returns the cycle time in milliseconds.
   - The component has no dependency on ESP-IDF drivers, so it builds on the host.

4. Update the host tests.
   - This step changes `test/host/CMakeLists.txt`, `test/host/main/CMakeLists.txt`, and `test/host/main/test_screen_cycle.c`, and creates `test/host/main/test_settings.c`.
   - The test project adds `components/settings` to its component folders, and the test component requires `settings`.
   - The screen cycle tests add cases 22 to 26 from validation.md. The settings tests add cases S1 to S10. The file `test_settings.c` has a function `run_settings_tests`, and `app_main` calls it after the screen cycle cases.

5. Save and load the settings in flash.
   - This step creates `main/settings_store.c` and `main/settings_store.h`.
   - The function `settings_store_load` starts NVS. If NVS reports no free pages or a new version, the function erases NVS and starts it again. It reads the three keys from the namespace `yes_no`, uses the default for each missing key, and calls `settings_sanitize`.
   - The function `settings_store_save` writes the three keys as `u8` values and commits them.

6. Play the chosen voice at the chosen volume.
   - This step changes `main/audio.c` and `main/audio.h`.
   - The function `audio_init` takes the start volume in addition to the end callback.
   - The function `audio_play` takes the voice in addition to the screen. The queue holds both values, and the playback task picks one of the four clips.
   - The new function `audio_set_volume` calls `esp_codec_dev_set_out_vol`.

7. Embed the four clips.
   - This step changes `main/CMakeLists.txt`.
   - The custom command lists the four clips as outputs, and `target_add_binary_data` embeds each one. The sources add `ui_settings.c`, `settings_store.c`, `font_settings_28.c`, and `font_settings_48.c`.

8. Make the font files for the settings screen and check them in.
   - This step creates `tools/fonts/AtkinsonHyperlegible-Regular.ttf`, `tools/fonts/AtkinsonHyperlegible-Bold.ttf`, `tools/fonts/OFL.txt`, `main/font_settings_28.c`, and `main/font_settings_48.c`.
   - The three source files come from `https://raw.githubusercontent.com/google/fonts/main/ofl/atkinsonhyperlegible/`.
   - The first command is `npx lv_font_conv --font tools/fonts/AtkinsonHyperlegible-Regular.ttf --range 0x20-0x7E --size 28 --bpp 4 --no-compress --format lvgl --lv-include lvgl.h --lv-font-name font_settings_28 -o main/font_settings_28.c`.
   - The second command is the same with `AtkinsonHyperlegible-Bold.ttf`, size 48, and the name `font_settings_48`.
   - The fonts must not be compressed, for the reason tech-stack.md gives.

9. Build the settings screen.
   - This step creates `main/ui_settings.c` and `main/ui_settings.h`.
   - The settings screen is its own LVGL screen object with a black background. It has the voice row, the cycle time row, the volume row, and the Done button, laid out as requirements.md item 8 says, with a 44 pixel margin from the edges, so that nothing sits in the round corners of the screen.
   - The function `ui_settings_open` takes the current settings and a close callback, keeps a copy of the settings, draws the values, and loads the screen.
   - Each button changes the copy with the settings logic and redraws the values. A button at its limit shows dimmed.
   - The screen records where each press starts. A gesture to the top that starts in the bottom 60 pixels calls the close callback. The Done button also calls it. The callback gets the copy.

10. Open the settings screen from the yes and no screens.
    - This step changes `main/ui.c` and `main/ui.h`.
    - The function `ui_init` takes a second callback for the swipe down. The screen records where each press starts. A gesture to the bottom that starts in the top 60 pixels calls this callback and calls `lv_indev_wait_release`, so the swipe does not also count as a tap.
    - The new function `ui_load` loads the yes and no screen object again after the settings screen.

11. Connect the settings in the entry point.
    - This step changes `main/main.c`.
    - The function `app_main` calls `settings_store_load` first. It passes the volume to `audio_init` and the cycle time to `screen_cycle_set_period`.
    - The tap callback passes the voice to `audio_play`.
    - The swipe callback opens the settings screen only when the screen is lit. It sets a flag that the settings screen shows.
    - While the flag is set, the tick callback does not call `screen_cycle_tick`. It calls `lv_display_get_inactive_time` and closes the settings screen after 30 seconds with no touch.
    - The close function clears the flag. If the settings changed, it saves them, sets the volume, and keeps the new copy. It calls `screen_cycle_init` with the yes screen and `screen_cycle_set_period` with the cycle time, loads the yes and no screen, sets the last tick time to now, and calls `apply_state`.

12. Update mission.md and roadmap.md for the swipe, which the user asked for during planning. This step is done in plan mode.

13. Run the host tests, the clip check, and the firmware build, and fix any failures.
