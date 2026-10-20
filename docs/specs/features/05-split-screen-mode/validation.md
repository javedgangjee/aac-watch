# Feature 5 validation: split screen mode

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic and the settings logic pass the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity reports 0 failures.

Cases 1 to 27 and S1 to S12 stay as features 1 to 3 list them. Each new screen cycle case from 28 to 34 starts from `screen_cycle_init` with `SCREEN_YES` and a period of 5000 ms.

| Case | Input | Expected result |
|---|---|---|
| 28 | The test makes no change. | The function `screen_cycle_is_split` returns false. |
| 29 | The test sets split on and ticks 10000 ms. | The tick returns false. The current screen is `SCREEN_YES` and the screen is on. |
| 30 | The test sets split on, ticks 119999 ms, and ticks 1 ms. | The screen is on after the 119999 ms tick. The 1 ms tick returns true and the screen is off. |
| 31 | The test sets split on, ticks 120000 ms, and taps. | The tap returns `SCREEN_TAP_WAKE`. The screen is on and `screen_cycle_is_split` returns true. |
| 32 | The test sets split on, ticks 60000 ms, taps, ticks 119999 ms, and ticks 1 ms. | The tap returns `SCREEN_TAP_SPEAK`. The screen is on after the 119999 ms tick, and the 1 ms tick turns it off. |
| 33 | The test sets split on, taps, ends the word, and ticks 10000 ms. | The tick returns false and the current screen is `SCREEN_YES`. |
| 34 | The test sets split on, ticks 3000 ms, sets split off, ticks 4999 ms, and ticks 1 ms. | The 4999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |

Each split word case calls `screen_cycle_split_word` with a height of 502.

| Case | Input | Expected result |
|---|---|---|
| 35 | The y position is 0. | The function returns true and the word is `SCREEN_YES`. |
| 36 | The y position is 245. | The function returns true and the word is `SCREEN_YES`. |
| 37 | The y position is 246. | The function returns false. |
| 38 | The y position is 255. | The function returns false. |
| 39 | The y position is 256. | The function returns true and the word is `SCREEN_NO`. |
| 40 | The y position is 501. | The function returns true and the word is `SCREEN_NO`. |

Each new settings case starts from `settings_defaults`, unless the case says otherwise.

| Case | Input | Expected result |
|---|---|---|
| S13 | The test makes no change. | The mode is `SETTINGS_MODE_CYCLE`. |
| S14 | The test sets the mode to 2 and sanitizes. | The function returns true and the mode is `SETTINGS_MODE_CYCLE`. |
| S15 | The test sets the mode to `SETTINGS_MODE_SPLIT` and sanitizes. | The function returns false and the mode is `SETTINGS_MODE_SPLIT`. |
| S16 | The test compares the defaults with a copy that has `SETTINGS_MODE_SPLIT`. | The function `settings_equal` returns false. |

### Check 2. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors.

## Manual checks

Before check 3, run `idf.py erase-flash` and then `idf.py flash`, so the watch starts with no saved settings.

### Check 3. The settings screen shows the mode row and fits.

1. Wait for the yes screen.
2. Swipe down from the top edge to open the settings screen.

The expected result is that the rows are Mode, Voice, Cycle time, and Volume, with Done at the bottom. "Cycle" is lit. No item is cut off by the round corners, and the text is easy to read.

### Check 4. The cycle time row hides in split mode.

1. On the settings screen, tap "Split".
2. Tap "Cycle".

The expected result is that after step 1 "Split" is lit, the cycle time row is gone, and the other rows spread out. After step 2 "Cycle" is lit and the cycle time row shows again. The watch makes no sound.

### Check 5. The split screen shows and does not switch.

1. On the settings screen, tap "Split" and tap Done.
2. Watch the screen for 20 seconds with no touch.

The expected result is that the top half is green with "Yes" and the bottom half is red with "No". A black band separates the halves. Each word is white and in the center of its half. The screen does not change for the 20 seconds.

### Check 6. A tap on each half speaks its word.

1. Tap the middle of the yes half.
2. Tap the middle of the no half.
3. Tap near the top left corner and then near the bottom right corner.

The expected result is that step 1 speaks "Yes", step 2 speaks "No", and step 3 speaks "Yes" and then "No". The screen does not change.

### Check 7. A tap on the band does nothing.

1. Tap the center of the black band.

The expected result is that the watch makes no sound and the screen does not change.

### Check 8. A tap on the other half stops the word.

1. Tap the yes half and, at once, tap the no half.

The expected result is that "Yes" stops and "No" plays to the end.

### Check 9. The swipes work in split mode.

1. Swipe down from the top edge.
2. Tap Done. Put a finger in the middle of the no half and swipe down.

The expected result is that step 1 opens the settings screen with no sound. In step 2 the settings screen does not open, and the watch speaks "No".

### Check 10. The dark screen wakes at the split screen.

1. Wait 2 minutes with no touch until the screen goes dark.
2. Tap the no half.

The expected result is that the split screen shows at full brightness and the watch makes no sound.

### Check 11. The mode stays after a restart.

1. Unplug the USB cable. If the watch has a battery, hold the power button until the watch turns off. Turn the watch on again.

The expected result is that the split screen shows. The settings screen shows "Split" lit.

### Check 12. Cycle mode still works.

1. On the settings screen, tap "Cycle", set the cycle time to 5 s, and tap Done.
2. Do feature 3 checks 5 to 10 again.

The expected result is the same as in feature 3. The 48 pixel buttons are easy to tap.

## Results on 2026-10-20

Check 1 passes. Unity ran 56 tests with 0 failures, which include cases 28 to 40 and S13 to S16.

Check 2 passes. The firmware build for esp32s3 ended with no errors, and the project code gave no warnings.

Checks 3 to 12 pass. The user ran each one on the watch after `idf.py erase-flash` and `idf.py flash`.

Each item in requirements.md has at least one passing check, with three gaps.

- No check proves that a tap on the band of the lit screen leaves the 2 minute count as it is, or that a tap on the band of the dark screen wakes it. Check 7 proves only that the band tap makes no sound.
- No check starts the watch with saved settings from feature 3. Case S14 and check 11 cover a mode that is out of range and a saved split mode.
- No check covers the README text about the split mode and the mode setting.
