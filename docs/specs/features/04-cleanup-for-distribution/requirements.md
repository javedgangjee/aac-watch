# Feature 4 requirements: cleanup for distribution

## From the specs

1. The repo has a license file. The source is roadmap.md, feature 4.
2. The repo has a README that says what the project does. The source is roadmap.md, feature 4, and mission.md.
3. The README says how to build and flash the firmware. The source is roadmap.md, feature 4, and tech-stack.md, which gives ESP-IDF v6.1, Python 3.13, the `say` command, and macOS as the build machine.
4. Another person can get, build, and run the project from the repo. The source is roadmap.md, feature 4.
5. The watch behaviour from features 1 to 3 stays the same. The source is mission.md, "What it does".

## Not covered by the specs

The user confirmed each item below during planning.

1. **Confirmed.** The project code uses the GNU General Public License version 3.0 or any later version. The file `LICENSE` holds the full GPL 3.0 text from gnu.org with no changes.
2. **Confirmed.** The copyright line reads "Copyright (C) 2026 Javed Gangjee". It goes in the README with the standard GNU notice for "version 3 or any later version".
3. **Confirmed.** The source files get no license header. The `LICENSE` file and the README notice cover the project.
4. **Confirmed.** The project name changes from yes-no to aac-watch everywhere except the finished feature folders 01 to 03, which stay as a record. The changes are in mission.md, CLAUDE.md, the CHANGELOG intro line, and the CMake project name, which becomes `aac_watch`. The firmware file becomes `build/aac_watch.bin`.
5. **Confirmed.** The font name `font_yes_no_160` and its file `main/font_yes_no_160.c` stay, because the name describes the two words in the font.
6. **Confirmed.** The NVS namespace changes from `yes_no` to `aac_watch`. A watch that is already flashed goes back to the default settings one time.
7. **Confirmed.** The file `.vscode/settings.json` leaves git with `git rm --cached`, and `.vscode/` goes into `.gitignore`. The local copy stays. The plan also adds `.ruff_cache/` to `.gitignore`, because the folder is in the repo root and is not ignored.
8. **Confirmed.** The README names the Montserrat font and its SIL Open Font License and links to the license. No Montserrat license file is added.
9. **Confirmed.** The README has these sections. They are what the project does, the hardware, what the build machine needs, how to build and flash, how to use the watch, and the license.
