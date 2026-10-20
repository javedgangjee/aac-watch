# Feature 2 validation: tap to speak

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic passes the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity reports 0 failures.

Each test case starts from `screen_cycle_init` with `SCREEN_YES`, unless the case says otherwise. Cases 1 to 11 stay as feature 1 lists them. Cases 12, 13, and 14 change, and cases 15 to 21 are new.

| Case | Input | Expected result |
|---|---|---|
| 12 | The test does case 10, taps, ticks 4999 ms, and ticks 1 ms. | The tap returns `SCREEN_TAP_WAKE`. The screen is on, the current screen is `SCREEN_YES`, and the screen is not speaking. The 4999 ms tick returns false and the screen stays `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 13 | The test ticks 119000 ms, taps, ticks 119999 ms, and ticks 1 ms. | The tap returns `SCREEN_TAP_SPEAK`. The screen is on after the 119999 ms tick. The 1 ms tick returns true, the screen is off, and the screen is not speaking. |
| 14 | The test ticks 3000 ms, taps, ends the word, ticks 4999 ms, and ticks 1 ms. | The tap returns `SCREEN_TAP_SPEAK`. The 4999 ms tick returns false and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 15 | The test taps once. | The tap returns `SCREEN_TAP_SPEAK`. The screen is speaking, and the current screen is `SCREEN_YES`. |
| 16 | The test ticks 3000 ms, taps, and ticks 10000 ms. | The 10000 ms tick returns false. The current screen is `SCREEN_YES` and the screen is speaking. |
| 17 | The test ticks 4000 ms, taps, ticks 2000 ms, ends the word, ticks 4999 ms, and ticks 1 ms. | The 2000 ms tick and the 4999 ms tick return false, and the current screen is `SCREEN_YES`. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 18 | The test taps, ticks 200 ms, taps, ends the word, ticks 4999 ms, and ticks 1 ms. | Both taps return `SCREEN_TAP_SPEAK`. The screen is speaking after the second tap and is not speaking after the word ends. The 4999 ms tick returns false. The 1 ms tick returns true and the current screen is `SCREEN_NO`. |
| 19 | The test ticks 3000 ms, ends the word with no tap, and ticks 2000 ms. | The 2000 ms tick returns true and the current screen is `SCREEN_NO`. |
| 20 | The test taps, ticks 120000 ms, ends the word, and ticks 10000 ms. | The 120000 ms tick returns true. The screen is off and is not speaking. The 10000 ms tick returns false and the screen stays off. |
| 21 | The test does case 10, taps, and taps again. | The first tap returns `SCREEN_TAP_WAKE` and the screen is not speaking. The second tap returns `SCREEN_TAP_SPEAK` and the screen is speaking. |

### Check 2. The clip script makes both clips.

The command is `mkdir -p build/clip-check && python3.13 tools/make_clips.py build/clip-check && wc -c build/clip-check/yes.pcm build/clip-check/no.pcm`.

The check passes when the script exits with no error and each file has an even byte count between 8820 and 88200. These limits are 0.2 seconds and 2 seconds of mono 16-bit audio at 22050 Hz.

### Check 3. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors and the build log shows the clip script ran.

## Manual checks

### Check 4. A tap on the yes screen speaks "Yes".

1. Connect the watch over USB and run `idf.py flash`.
2. Hold the watch at arm's length and tap the yes screen once.

The expected result is that the watch says "Yes" right after the tap in the Samantha voice. The word is clear, has no distortion, and is easy to hear at arm's length.

### Check 5. A tap on the no screen speaks "No".

1. Wait for the no screen and tap it once.

The expected result is that the watch says "No" right after the tap, and the word is clear.

### Check 6. The screen holds while the word plays and for 5 seconds after.

1. Wait for the yes screen to show and count 4 seconds.
2. Tap the screen once and start a stopwatch.

The expected result is that the screen does not switch at the 5 second mark of the cycle. The screen switches to no at about 5.5 seconds on the stopwatch, plus or minus 1 second. The extra half second is the length of the word.

### Check 7. A tap during the word restarts the word.

1. Tap the yes screen 3 times fast, with about a quarter second between taps.

The expected result is that the word starts again at each tap and plays once to the end after the last tap. The screen stays on yes until about 5 seconds after the word ends.

### Check 8. A tap on the dark screen does not speak.

1. Wait 2 minutes with no tap until the screen goes dark.
2. Tap the screen once.

The expected result is that the yes screen shows at full brightness and the watch makes no sound. The screen switches to no at 5 seconds, plus or minus 1 second.

### Check 9. The feature 1 timing still holds.

1. Do feature 1 checks 5 and 8 again.

The expected result is the same as in feature 1. In check 8, the tap at 1 minute 30 seconds also speaks the word, and the next switch comes about 5 seconds after the word ends.

## Results

The checks ran on 2026-10-19 on branch `feature/02-tap-to-speak`.

| Check | Result | Summary |
|---|---|---|
| 1 | Pass | Unity ran 21 tests with 0 failures and 0 ignored. |
| 2 | Pass | The script exited with no error. The file `yes.pcm` has 21194 bytes and `no.pcm` has 18240 bytes. |
| 3 | Pass | The build ended with no errors, and the log shows the step "Making the yes and no clips with say". The 15 warnings come from ESP-IDF and LVGL code, and none come from project code. |
| 4 | Pass | The user reported a pass. |
| 5 | Pass | The user reported a pass. |
| 6 | Pass | The user reported a pass. |
| 7 | Pass | The user reported a pass. |
| 8 | Pass | The user reported a pass. |
| 9 | Pass | The user reported a pass. |

### Requirement coverage

Each item in requirements.md has a passing check, except for the items below. I checked these items by reading the code, and each one is met.

- Item 9 under "From the specs" has no check that names it. The file `main/idf_component.yml` lists `espressif/esp_codec_dev` at `~1.5`, and `dependencies.lock` locks version 1.5.11. Check 3 builds with it.
- Item 3 under "Not covered by the specs" has no check that measures the volume. The file `main/audio.c` sets the volume to 90. Check 4 confirms only that the word is easy to hear.
- Item 5 under "Not covered by the specs" has no check for a build without `say` or Python 3.13. The file `main/CMakeLists.txt` stops the build when Python 3.13 is missing, and `tools/make_clips.py` exits with an error when `say` is missing.
- Item 9 under "Not covered by the specs" has no check that the codec opens only once. The function `audio_init` in `main/audio.c` opens the codec, and `main/main.c` calls it once at start.
- Item 10 under "Not covered by the specs" is checked for `tools/make_clips.py` by check 2. No check names `main/audio.c` or `main/audio.h`, but both files exist and check 3 builds them.
