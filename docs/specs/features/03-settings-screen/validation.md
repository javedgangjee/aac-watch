# Feature 3 validation: settings screen

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic and the settings logic pass the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity reports 0 failures.

Cases 1 to 21 stay as features 1 and 2 list them. Each new screen cycle case starts from `screen_cycle_init` with `SCREEN_YES`.

| Case | Input | Expected result |
|---|---|---|
| 22 | The test sets the period to 3000 ms, ticks 2999 ms, and ticks 1 ms. | The 2999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 23 | The test sets the period to 15000 ms, ticks 14999 ms, and ticks 1 ms. | The 14999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 24 | The test ticks 3000 ms, sets the period to 5000 ms, ticks 4999 ms, and ticks 1 ms. | The 4999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 25 | The test sets the period to 8000 ms, ticks 120000 ms, taps, ticks 7999 ms, and ticks 1 ms. | The 120000 ms tick turns the screen off. The tap returns `SCREEN_TAP_WAKE` and the current screen is `SCREEN_YES`. The 7999 ms tick returns false. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 26 | The test sets the period to 3000 ms, taps, ends the word, ticks 2999 ms, and ticks 1 ms. | The 2999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 27 | The test sets the period to 15000 ms, ticks 119999 ms, and ticks 1 ms. | The screen is on after the 119999 ms tick. The 1 ms tick returns true and the screen is off. |

Each settings case starts from `settings_defaults`, unless the case says otherwise.

| Case | Input | Expected result |
|---|---|---|
| S1 | The test makes no change. | The voice is `SETTINGS_VOICE_SAMANTHA`, the cycle time is 5, and the volume is 90. The function `settings_cycle_ms` returns 5000. |
| S2 | The test steps the cycle time up once. | The step returns true and the cycle time is 6. |
| S3 | The test steps the cycle time up 11 times. | The first 10 steps return true and the 11th returns false. The cycle time is 15, and `settings_cycle_ms` returns 15000. |
| S4 | The test steps the cycle time down 3 times. | The first 2 steps return true and the 3rd returns false. The cycle time is 3. |
| S5 | The test steps the volume down 9 times. | The first 8 steps return true and the 9th returns false. The volume is 10. |
| S6 | The test steps the volume up twice. | The first step returns true and the second returns false. The volume is 100. |
| S7 | The test sanitizes the defaults. | The function returns false and the settings do not change. |
| S8 | The test sanitizes voice 7, cycle time 2, and volume 95. | The function returns true. The voice is `SETTINGS_VOICE_SAMANTHA`, the cycle time is 5, and the volume is 90. |
| S9 | The test sanitizes `SETTINGS_VOICE_REED`, cycle time 16, and volume 0. | The function returns true. The voice is `SETTINGS_VOICE_REED`, the cycle time is 5, and the volume is 90. |
| S10 | The test sanitizes `SETTINGS_VOICE_REED`, cycle time 15, and volume 10. | The function returns false and the settings do not change. |
| S11 | The test compares the defaults with the defaults. | The function `settings_equal` returns true. |
| S12 | The test compares the defaults with three copies. The first copy has `SETTINGS_VOICE_REED`, the second has cycle time 6, and the third has volume 80. | The function `settings_equal` returns false for each copy. |

### Check 2. The clip script makes all four clips.

The command is `mkdir -p build/clip-check && python3.13 tools/make_clips.py build/clip-check && wc -c build/clip-check/*.pcm`.

The check passes when the script exits with no error and makes `yes_samantha.pcm`, `no_samantha.pcm`, `yes_reed.pcm`, and `no_reed.pcm`. Each file has an even byte count between 8820 and 88200, which is 0.2 seconds to 2 seconds of audio.

### Check 3. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors and the build log shows the clip script ran.

## Manual checks

Before check 4, run `idf.py erase-flash` and then `idf.py flash`, so the watch starts with no saved settings.

### Check 4. The settings screen opens with the defaults.

1. Wait for the yes screen.
2. Put a finger on the top edge of the screen and swipe down to the middle.

The expected result is that the settings screen shows and the watch makes no sound. "Samantha" is lit, the cycle time is 5 s, and the volume is 90. The text fits on the screen and is easy to read.

### Check 5. A swipe that does not start at the top does not open the settings screen.

1. Go back to the yes screen with the Done button.
2. Put a finger in the middle of the screen and swipe down.

The expected result is that the settings screen does not open. The watch speaks the word on the screen, as a tap does.

### Check 6. The voice setting works.

1. Open the settings screen, tap "Reed", and tap Done.
2. Tap the yes screen. Wait for the no screen and tap it.

The expected result is that "Reed" is lit after the tap in step 1. In step 2 the watch says "Yes" and then "No" in the Reed voice.

### Check 7. The cycle time setting works and stops at its limits.

1. Open the settings screen and tap the minus button of the cycle time 3 times.
2. Tap Done and time the switches with a stopwatch.
3. Open the settings screen and tap the plus button of the cycle time 13 times.

The expected result is that after step 1 the value is 3 s and the minus button shows dimmed. In step 2 the first switch comes 3 seconds after Done, and the screens then switch every 3 seconds, plus or minus 1 second. After step 3 the value is 15 s and the plus button shows dimmed.

### Check 8. The volume setting works and stops at its limits.

1. On the settings screen, tap the minus button of the volume 9 times, and tap Done.
2. Tap the yes screen at arm's length.
3. Open the settings screen, tap the plus button of the volume 9 times, and tap Done.
4. Tap the yes screen at arm's length.

The expected result is that after step 1 the value is 10 and the minus button is dimmed. In step 2 the word is quiet. After step 3 the value is 100 and the plus button is dimmed. In step 4 the word is loud and has no distortion.

### Check 9. A swipe up from the bottom goes back.

1. Open the settings screen.
2. Put a finger on the bottom edge of the screen and swipe up to the middle.

The expected result is that the yes screen shows and the watch makes no sound. The first switch comes one full cycle time after the swipe.

### Check 10. The settings screen goes back after 30 seconds with no touch.

1. Open the settings screen, change the cycle time to 5 s, and start a stopwatch.
2. Do not touch the watch.

The expected result is that the yes screen shows at about 30 seconds, plus or minus 2 seconds. The screens then switch every 5 seconds.

### Check 11. The settings stay after a restart.

1. Open the settings screen and set "Reed", 4 s, and volume 70. Tap Done.
2. Unplug the USB cable. If the watch has a battery, hold the power button until the watch turns off. Turn the watch on again.
3. Open the settings screen.

The expected result is that the settings screen shows "Reed" lit, 4 s, and 70. A tap on the yes screen speaks in the Reed voice.

### Check 12. A swipe on the dark screen only wakes it.

1. Wait 2 minutes with no touch until the screen goes dark.
2. Swipe down from the top edge.

The expected result is that the yes screen shows at full brightness, the settings screen does not open, and the watch makes no sound.

### Check 13. Feature 2 still works with the defaults.

1. Open the settings screen and set "Samantha", 5 s, and volume 90. Tap Done.
2. Do feature 2 checks 4, 6, 7, and 8 again.

The expected result is the same as in feature 2.

## Results

These results are from 2026-10-19.

- Check 1 passes. Unity runs 39 tests with 0 failures, which includes cases 22 to 27 and S1 to S12.
- Check 2 passes. The script makes the four clips, and the byte counts are 21194, 18240, 35922, and 34498.
- Check 3 passes. The build ends with no errors, and the log shows the clip script wrote all four clips. The only warnings come from LVGL and ESP-IDF.
- Check 4 fails on the first run and passes on the second run. On the first run, the round corners of the screen cut off the items at the corners. The fix sets the margin of the settings screen to 44 pixels and the button height to 56 pixels, and plan.md step 9 now gives the new margin.
- Checks 5 to 13 pass.

No check in this file covers these requirements, so they have no passing check.

- Item 9 says the settings screen makes no sound when a setting changes. Checks 6, 7, and 8 change settings, but their expected results do not say that the watch stays silent.
- Item 13 says the 2 minute count starts from zero after the settings screen closes. No check measures this.
- Item 14 says the 2 minute count stops while the settings screen shows, and a word that plays when the screen opens plays to the end. No check covers either part.
- Item 10 says a swipe up must start in the bottom 60 pixels. Check 5 tests a swipe down from the middle, but no check tests a swipe up from the middle.
- Items 17, 18, and 19 cover the font source, the font settings, and the file names. No check covers them, but a review of the files on 2026-10-19 found the TTF files and OFL.txt in `tools/fonts/`, fonts that are not compressed with the range 0x20 to 0x7E, and the file names and NVS keys that item 19 lists.
