# Feature 7 validation: tap feedback

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The screen and timer logic passes the host tests.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity reports 0 failures.

Cases 1 to 40 and S1 to S20 stay as features 1 to 6 list them. Each new case starts from `screen_cycle_init` with `SCREEN_YES`, unless the case says otherwise.

| Case | Input | Expected result |
|---|---|---|
| 41 | The test makes no change. | The function `screen_cycle_spoken` returns false. |
| 42 | The test taps with `screen_cycle_tap`. | The function `screen_cycle_spoken` returns true and gives `SCREEN_YES`. |
| 43 | The test ticks 5000 ms and taps with `screen_cycle_tap`. | The current screen is `SCREEN_NO`. The function `screen_cycle_spoken` returns true and gives `SCREEN_NO`. |
| 44 | The test taps with `screen_cycle_tap` and ends the word. | The function `screen_cycle_spoken` returns false. |
| 45 | The test turns split mode on and taps `SCREEN_NO` with `screen_cycle_tap_word`. | The tap returns `SCREEN_TAP_SPEAK`. The function `screen_cycle_spoken` returns true and gives `SCREEN_NO`. |
| 46 | The test turns split mode on, taps `SCREEN_YES`, ticks 200 ms, and taps `SCREEN_NO`, each with `screen_cycle_tap_word`. | After the first tap, the function `screen_cycle_spoken` gives `SCREEN_YES`. After the second tap, it returns true and gives `SCREEN_NO`. |
| 47 | The test does case 10 and taps `SCREEN_NO` with `screen_cycle_tap_word`. | The tap returns `SCREEN_TAP_WAKE`. The function `screen_cycle_spoken` returns false. |
| 48 | The test taps with `screen_cycle_tap` and ticks 120000 ms. | The screen is off. The function `screen_cycle_spoken` returns false. |
| 49 | The test taps with `screen_cycle_tap` and calls `screen_cycle_init` with `SCREEN_YES`. | The function `screen_cycle_spoken` returns false. |

### Check 2. The firmware builds for the watch.

The command is `idf.py set-target esp32s3 && idf.py build`. The check passes when the build ends with no errors.

## Manual checks

Before check 3, run `idf.py flash`. The watch keeps its saved settings. On the settings screen, set the mode to Cycle, the icons to Show, and the cycle time to 5 s.

### Check 3. A tap on the yes screen swaps its colors.

1. On the yes screen, tap once and watch the screen until the word ends.

The expected result is that the screen turns white with a green check mark and a green "Yes" at the tap. The watch says "Yes". When the word ends, the screen goes back to green with white text. The screen switches to no about 5 seconds after the word ends.

### Check 4. A tap on the no screen swaps its colors.

1. On the no screen, tap once and watch the screen until the word ends.

The expected result is that the screen turns white with a red X and a red "No" at the tap. The watch says "No". When the word ends, the screen goes back to red with white text.

### Check 5. Fast taps keep the colors swapped.

1. Tap the yes screen 3 times fast, with about a quarter second between taps.

The expected result is that the screen stays white with green text through all 3 taps and does not flicker. It goes back to green once, after the last word ends.

### Check 6. A tap on the dark screen does not swap the colors.

1. Wait 2 minutes with no tap until the screen goes dark.
2. Tap the screen once.

The expected result is that the yes screen shows green with white text, and the watch makes no sound.

### Check 7. A tap on a split half swaps only that half.

1. On the settings screen, tap "Split" and tap Done.
2. Tap the yes half once.
3. Tap the no half once.

The expected result is that in step 2 the yes half turns white with a green check mark and a green "Yes", and the no half stays red. In step 3 the no half turns white with a red X and a red "No", and the yes half stays green. The black band does not change. Each half goes back when its word ends.

### Check 8. A tap on the other half moves the swap.

1. Tap the yes half and, while "Yes" plays, tap the no half.

The expected result is that the yes half goes back to green at the second tap and the no half turns white at the same time. The watch stops "Yes" and says "No". The no half goes back to red when "No" ends.

### Check 9. The settings screen clears the swap.

1. Tap the yes half and, while "Yes" plays, swipe down from the top edge.
2. Tap Done.

The expected result is that the settings screen opens in step 1. In step 2 the split screen shows with a green yes half and a red no half, and no half is white.

### Check 10. The swap works with the icons hidden.

1. On the settings screen, tap "Hide", tap "Cycle", and tap Done.
2. Tap the yes screen once.

The expected result is that the screen turns white with a green "Yes" in the center and no icon. It goes back to green when the word ends.

### Check 11. The other features still work.

1. On the settings screen, tap "Show" and tap Done.
2. Do feature 6 checks 4, 8, and 11 again.

The expected result is the same as in feature 6.
