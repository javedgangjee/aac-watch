# Feature 5 plan: split screen mode

1. Add the mode to the settings logic.
   - This step changes `components/settings/include/settings.h` and `components/settings/settings.c`.
   - The header defines `settings_mode_t` with the values `SETTINGS_MODE_CYCLE` and `SETTINGS_MODE_SPLIT`, and the constant `SETTINGS_MODE_DEFAULT`, which is `SETTINGS_MODE_CYCLE`. The struct `settings_t` gets the field `mode`.
   - The function `settings_defaults` sets the mode to cycle. The function `settings_sanitize` sets a mode that is neither value to cycle. The function `settings_equal` also compares the mode.

2. Add the split mode to the screen and timer logic.
   - This step changes `components/screen_cycle/include/screen_cycle.h` and `components/screen_cycle/screen_cycle.c`.
   - The struct `screen_cycle_t` gets the field `split`. The function `screen_cycle_init` sets it to false.
   - The new function `screen_cycle_set_split` sets `split` and sets the switch count to zero. The new function `screen_cycle_is_split` returns it.
   - In split mode, `screen_cycle_tick` adds to the off count and turns the screen off at 2 minutes, but it never switches the screen.
   - The function `screen_cycle_tap` keeps `split` when it wakes the dark screen, as it keeps the period.
   - The constant `SCREEN_CYCLE_SPLIT_GAP` is 10. The new function `screen_cycle_split_word` takes the y position of the press and the screen height. It returns false when the press is on the band. Otherwise it returns true and writes `SCREEN_YES` for the top half or `SCREEN_NO` for the bottom half. The band starts at row `(height - gap) / 2`.

3. Update the host tests.
   - This step changes `test/host/main/test_screen_cycle.c` and `test/host/main/test_settings.c`.
   - The screen cycle tests add cases 28 to 40 from validation.md. The settings tests add cases S13 to S16.

4. Save and load the mode.
   - This step changes `main/settings_store.c`.
   - The store reads and writes the key `mode` as a `u8`, in the same way as the other keys.

5. Draw the split screen.
   - This step changes `main/ui.c` and `main/ui.h`.
   - The function `ui_init` also makes two hidden half objects. The top half is green with a "Yes" label, and the bottom half is red with a "No" label. Each half is the screen width wide and `(height - SCREEN_CYCLE_SPLIT_GAP) / 2` high. The labels use `font_yes_no_160` and sit in the center of their half. The halves are not clickable and not scrollable, so presses reach the screen object.
   - The new function `ui_show_split` sets the screen background to black, hides the cycle label, and shows the two halves.
   - The function `ui_show` hides the two halves and shows the cycle label, and then draws as it does now.

6. Add the mode row to the settings screen.
   - This step changes `main/ui_settings.c`.
   - The constant `UI_BUTTON_H` goes from 56 to 48.
   - A group named "Mode" is the first row. It holds a "Cycle" button and a "Split" button, laid out as the voice row is, and the chosen one is lit.
   - A tap on a mode button sets the mode and redraws the values. The function `draw_values` hides the cycle time group when the mode is split and shows it when the mode is cycle.

7. Connect the mode in the main loop.
   - This step changes `main/main.c`.
   - After each call to `screen_cycle_init`, at start and when the settings screen closes, the code calls `screen_cycle_set_split` with the saved mode.
   - The code keeps the view on the panel, which is yes, no, or split. The function `apply_state` calls `ui_show_split` in split mode and `ui_show` in cycle mode, and only when the view changes.
   - In split mode, the function `on_tap` calls `screen_cycle_split_word` with `ui_press_start_y` and the screen height. On the lit screen, a press on the band returns before `screen_cycle_tap`. A press on a half calls `screen_cycle_tap` and plays the word of that half. A tap on the dark screen wakes it as it does now.

8. Update the README.
   - This step changes `README.md`.
   - The use section says the settings screen has four settings, and it describes the mode setting and the split screen. The `deslopificate` skill checks the changed text for AI tells and tropes.

9. Set the status of feature 5 to in progress in `docs/specs/roadmap.md`. This step is done during planning.
