# Feature 4 validation: cleanup for distribution

Each command runs from the repo root after `source ~/.espressif/tools/activate_idf_v6.1.sh`.

## Automated checks

### Check 1. The host tests still pass.

The command is `cd test/host && idf.py --preview set-target linux && idf.py build && ./build/screen_cycle_test.elf`. The check passes when Unity runs 39 tests with 0 failures. Cases 1 to 27 and S1 to S12 stay as features 1 to 3 list them.

### Check 2. A clean copy of the repo builds for the watch.

The command is `rm -rf "$TMPDIR/aac-watch-check" && mkdir -p "$TMPDIR/aac-watch-check" && git ls-files -co --exclude-standard | rsync -a --files-from=- . "$TMPDIR/aac-watch-check/" && cd "$TMPDIR/aac-watch-check" && idf.py set-target esp32s3 && idf.py build && ls -l build/aac_watch.bin`.

The copy holds only the files that git tracks or would track, which is what a person gets from a clone. The check passes when the build ends with no errors, the log shows the clip script ran, and `build/aac_watch.bin` exists.

### Check 3. The old project name is gone outside the finished feature folders.

The command is `git grep -n -i -E 'yes[-_]no' -- . ':!docs/specs/features/01-*' ':!docs/specs/features/02-*' ':!docs/specs/features/03-*' | grep -v font_yes_no_160`.

The check passes when the command prints nothing.

### Check 4. The license file is the GPL 3.0 text with no changes.

The command is `curl -s https://www.gnu.org/licenses/gpl-3.0.txt | diff - LICENSE && echo same`. The check passes when the command prints only `same`.

### Check 5. The VS Code settings leave git and stay on disk.

The command is `git ls-files .vscode; git check-ignore .vscode/settings.json .ruff_cache; test -f .vscode/settings.json && echo kept`.

The check passes when the output is `.vscode/settings.json`, then `.ruff_cache`, then `kept`. The first command prints nothing.

## Manual checks

### Check 6. The README has the required content.

1. Open `README.md`.

The expected result is that the title is aac-watch, and the README has a section on what the project does, the hardware, the build machine, how to build and flash, how to use the watch, and the license. The license section has the line "Copyright (C) 2026 Javed Gangjee", the GNU notice for version 3 or any later version, and the Montserrat license link. The README does not use the name yes-no.

### Check 7. The README build and flash steps work as written.

1. Open a new terminal and go to the clean copy from check 2.
2. Run `idf.py fullclean`.
3. Follow the build and flash section of the README word for word, with the watch connected over USB.

The expected result is that each command works as written, and the watch shows the yes screen after the flash.

### Check 8. The watch uses the new settings namespace.

1. Before you flash the feature 4 firmware, make sure the watch has Reed saved from feature 3. If it does not, flash the main branch, set Reed, and tap Done.
2. Flash the feature 4 firmware with `idf.py flash`.
3. Open the settings screen.
4. Set Reed, 4 s, and volume 70, and tap Done.
5. Unplug the USB cable, turn the watch off and on, and open the settings screen.

The expected result is that step 3 shows the defaults, which are Samantha lit, 5 s, and 90. Step 5 shows Reed lit, 4 s, and 70.

### Check 9. The README use section matches the watch.

1. Do each action that the use section of the README lists, in the order it lists them.

The expected result is that the watch does what the README says for each action.
