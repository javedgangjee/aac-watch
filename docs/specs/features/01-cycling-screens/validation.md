# Feature 1 validation: cycling screens

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic passes the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The linux target is in preview on ESP-IDF v6.1, so `set-target` needs the `--preview` flag. The check passes when Unity reports 0 failures.

Each test case starts from `screen_cycle_init` with `SCREEN_YES`, unless the case says otherwise.

| Case | Input | Expected result |
|---|---|---|
| 1 | The test makes no tick call. | The current screen is `SCREEN_YES` and the screen is on. |
| 2 | The test starts from `SCREEN_NO` and makes no tick call. | The current screen is `SCREEN_NO`. |
| 3 | The test ticks 4999 ms. | The tick returns false and the current screen is `SCREEN_YES`. |
| 4 | The test ticks 4999 ms and then 1 ms. | The second tick returns true and the current screen is `SCREEN_NO`. |
| 5 | The test ticks 5000 ms twice. | Each tick returns true and the current screen is `SCREEN_YES`. |
| 6 | The test ticks 4000 ms, then 3000 ms, then 2999 ms. | The ticks return false, true, and false. The current screen is `SCREEN_NO`. |
| 7 | The test ticks 4000 ms, then 3000 ms, then 3000 ms. | The ticks return false, true, and true. The current screen is `SCREEN_YES`. |
| 8 | The test ticks 50 ms 100 times. | The current screen is `SCREEN_NO` after the 100th tick, and only the 100th tick returns true. |
| 9 | The test ticks 0 ms. | The tick returns false and the current screen is `SCREEN_YES`. |
| 10 | The test ticks 50 ms 2400 times. | The screen is on after the 2399th tick. The 2400th tick returns true, the screen is off, and the current screen is `SCREEN_NO`. |
| 11 | The test does case 10 and then ticks 10000 ms. | The last tick returns false. The screen is off and the current screen is `SCREEN_NO`. |
| 12 | The test does case 10, taps, ticks 4999 ms, and ticks 1 ms. | The tap returns true, the screen is on, and the current screen is `SCREEN_YES`. The 4999 ms tick returns false and the screen stays `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 13 | The test ticks 119000 ms, taps, ticks 119999 ms, and ticks 1 ms. | The tap returns false. The screen is on after the 119999 ms tick. The 1 ms tick returns true and the screen is off. |
| 14 | The test ticks 3000 ms, taps, and ticks 2000 ms. | The tap returns false and the current screen stays `SCREEN_YES`. The 2000 ms tick returns true and the current screen is `SCREEN_NO`. |

### Check 2. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors.

## Manual checks

### Check 3. The yes screen looks right.

1. Connect the watch over USB and run `idf.py flash`.
2. Look at the screen within 5 seconds of the restart.

The expected result is a green screen with the word "Yes" in large white letters. The word is in title case and sits in the center of the screen, both horizontally and vertically.

### Check 4. The no screen looks right.

1. Wait for the screen to switch after check 3.

The expected result is a red screen with the word "No" in large white letters. The word is in title case and sits in the center of the screen, both horizontally and vertically.

### Check 5. The screens switch every 5 seconds.

1. Start a stopwatch when the screen switches to yes.
2. Count 12 switches and stop the stopwatch on the 12th switch.

The expected result is that the screens alternate between yes and no at each switch, and the stopwatch shows 60 seconds, plus or minus 1 second.

### Check 6. The screen turns off after 2 minutes.

1. Run `idf.py monitor`, press Ctrl+T and then Ctrl+R to restart the watch, and start a stopwatch.
2. Do not touch the screen.

The expected result is that the screen goes dark at 2 minutes, plus or minus 1 second.

### Check 7. A tap on the dark screen turns it on at the yes screen.

1. Wait until the screen is dark, as in check 6.
2. Tap the screen once and start a stopwatch.

The expected result is that the yes screen shows at full brightness right after the tap. It switches to the no screen at 5 seconds, plus or minus 1 second.

### Check 8. A tap on the lit screen restarts the 2 minute count.

1. Run `idf.py monitor`, press Ctrl+T and then Ctrl+R to restart the watch, and start a stopwatch.
2. Tap the screen once at 1 minute 30 seconds.

The expected result is that the tap does not change the screen or the switch timing. The screen stays on past 2 minutes and goes dark at 3 minutes 30 seconds, plus or minus 1 second.

## Results

The checks ran on 2026-10-19.

| Check | Result | Summary |
|---|---|---|
| 1 | Pass | Unity ran 14 tests and reported 0 failures. |
| 2 | Pass | The firmware built with no errors. The warnings came only from LVGL and FreeRTOS code. |
| 3 | Pass | The first run failed because jumbled lines covered each letter. The font was then made again without compression, and the second run passed. |
| 4 | Pass | The user confirmed the no screen on the watch. |
| 5 | Pass | The user confirmed the 5 second switch on the watch. |
| 6 | Pass | The user confirmed that the screen turns off at 2 minutes. |
| 7 | Pass | The user confirmed that a tap on the dark screen shows the yes screen. |
| 8 | Pass | The user confirmed that a tap on the lit screen restarts the 2 minute count. |

The check 3 failure had one cause. The project draws each screen update on two LVGL threads, and LVGL decodes compressed glyphs with one shared state. The fix is the `--no-compress` option in plan step 4.

Each item in requirements.md has at least one passing check. The capital letters are about 112 pixels high, because `lv_font_conv` sets the font size and not the letter height. Item 1 under "Not covered by the specs" says 160 pixels.
