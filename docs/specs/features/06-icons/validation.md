# Feature 6 validation: icons

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic and the settings logic pass the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity reports 0 failures.

Cases 1 to 40 and S1 to S16 stay as features 1 to 5 list them. Each new settings case starts from `settings_defaults`, unless the case says otherwise.

| Case | Input | Expected result |
|---|---|---|
| S17 | The test makes no change. | The icons value is `SETTINGS_ICONS_SHOW`. |
| S18 | The test sets the icons value to 2 and sanitizes. | The function returns true and the icons value is `SETTINGS_ICONS_SHOW`. |
| S19 | The test sets the icons value to `SETTINGS_ICONS_HIDE` and sanitizes. | The function returns false and the icons value is `SETTINGS_ICONS_HIDE`. |
| S20 | The test compares the defaults with a copy that has `SETTINGS_ICONS_HIDE`. | The function `settings_equal` returns false. |

### Check 2. The icon font files are not compressed.

The command is `grep -c -- "--no-compress" main/font_icons_150.c main/font_icons_80.c`. The check passes when the count is 1 for each file.

### Check 3. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors.

## Manual checks

Before check 4, run `idf.py erase-flash` and then `idf.py flash`, so the watch starts with no saved settings.

### Check 4. The cycle mode screens show the icons.

1. Wait for the yes screen.
2. Wait 5 seconds for the no screen.

The expected result is that the yes screen shows a white check mark above "Yes", and the no screen shows a white X above "No". The icon and the word sit in the center of the screen as one group. The screens switch every 5 seconds.

### Check 5. A tap on the icon speaks the word.

1. On the yes screen, tap the check mark.
2. On the no screen, tap the X.

The expected result is that step 1 speaks "Yes" and step 2 speaks "No".

### Check 6. The settings screen shows the Icons row and scrolls.

1. Swipe down from the top edge to open the settings screen.
2. Drag up from the middle of the screen until the list stops.
3. Drag down from the middle of the screen until the list stops.

The expected result is that after step 1 the rows start with Mode and Icons, and "Show" is lit. A thin grey line sits between each two rows, and no line sits between Volume and Done. After step 2 Volume and Done show at the bottom. After step 3 Mode shows at the top. The list does not bounce past its ends, no item is cut off by the round corners, and the watch makes no sound.

### Check 7. The rows keep their height when the cycle time row hides.

1. On the settings screen, tap "Split".
2. Drag the list to its end.
3. Tap "Cycle".

The expected result is that after step 1 the cycle time row and its line are gone. Volume moves up below Voice, with one line between them, and each row keeps the same height as before. No blank space and no double line shows. After step 3 the cycle time row and its line show again in their place.

### Check 8. A swipe up from the bottom edge closes the settings screen at any scroll position.

1. With the list at the top, swipe up from the bottom edge.
2. Open the settings screen, drag the list to its end, and swipe up from the bottom edge.

The expected result is that each swipe closes the settings screen and shows the yes screen. The list does not scroll during the swipe.

### Check 9. The split mode screen shows the icons.

1. On the settings screen, tap "Split" and tap Done.

The expected result is that the yes half shows a white check mark to the left of "Yes", and the no half shows a white X to the left of "No". Each icon is centered on the capitals of its word. Each line sits in the center of its half, and no part of it is cut off at the edges.

### Check 10. A tap on a split icon speaks its word.

1. Tap the check mark.
2. Tap the X.

The expected result is that step 1 speaks "Yes" and step 2 speaks "No".

### Check 11. Hide removes the icons in both modes.

1. On the settings screen, tap "Hide" and tap Done.
2. Look at the split screen.
3. On the settings screen, tap "Cycle" and tap Done. Watch one yes screen and one no screen.

The expected result is that no icon shows in steps 2 and 3. Each word sits in the center of its half or its screen, as in feature 5.

### Check 12. The icons setting stays after a restart.

1. Unplug the USB cable. If the watch has a battery, hold the power button until the watch turns off. Turn the watch on again.

The expected result is that the yes screen shows with no icon. The settings screen shows "Hide" lit.

### Check 13. The other settings still work.

1. On the settings screen, tap "Show", set the cycle time to 5 s, and tap Done.
2. Do feature 5 checks 4, 6, 7, 9, and 10 again.

The expected result is the same as in feature 5, with the icons in place.

## Results

The checks ran on 2026-10-20.

| Check | Result | Summary |
|---|---|---|
| 1 | Pass | Unity ran 60 tests with 0 failures, and cases S17 to S20 pass. |
| 2 | Pass | The count is 1 for `main/font_icons_150.c` and 1 for `main/font_icons_80.c`. |
| 3 | Pass | The build ends with no errors. The 15 warnings all come from ESP-IDF and LVGL code. |
| 4 to 13 | Pass | The user ran each manual check on the watch after an erase and a flash, and each one passed. |

Some items in requirements.md have no check in this file.

- Item 3 of "Not covered by the specs" says the list starts at the top each time the settings screen opens. Check 6 shows the top only on the first open.
- Item 8 of "Not covered by the specs" says a watch with saved settings from feature 5 keeps its other settings. Every manual check started from an erased flash, so no check loads settings from feature 5.
- Items 10 and 17 of "Not covered by the specs" cover the files in `tools/fonts/` and the README text. No check covers them, but a read of the repo shows the woff file, its license, the icons setting, the scroll, and the Font Awesome credit in place.
- Item 18 of "Not covered by the specs" moves to the replan step and needs no check here.
