# Feature 4 plan: cleanup for distribution

1. Rename the project to aac-watch.
   - This step changes `CMakeLists.txt`, `main/settings_store.c`, `docs/specs/mission.md`, `CLAUDE.md`, and `CHANGELOG.md`.
   - The CMake call `project(yes_no)` becomes `project(aac_watch)`.
   - The constant `STORE_NAMESPACE` in `main/settings_store.c` becomes `"aac_watch"`.
   - The first sentence of mission.md starts with "aac-watch is". The heading of CLAUDE.md becomes `# aac-watch`. The intro line of the changelog names aac-watch.

2. Clean up the files that git tracks.
   - This step changes `.gitignore` and removes `.vscode/settings.json` from the git index.
   - The command `git rm --cached .vscode/settings.json` removes the file from git and keeps the local copy.
   - The `.gitignore` file adds the lines `.vscode/` and `.ruff_cache/`.

3. Add the license.
   - This step creates `LICENSE`.
   - The file holds the text of https://www.gnu.org/licenses/gpl-3.0.txt with no changes.

4. Write the README.
   - This step creates `README.md`.
   - The title is `# aac-watch`. The first section says in a few sentences what the watch does, from mission.md.
   - The hardware section names the Waveshare ESP32-S3-Touch-AMOLED-2.06.
   - The build machine section lists macOS, ESP-IDF v6.1, Python 3.13 as `python3.13`, and the `say` command with the Samantha and Reed voices. It says that `say -v '?'` lists the installed voices, and that the build fails if Python 3.13 or `say` is missing.
   - The build and flash section gives the commands in order. The person activates the ESP-IDF v6.1 environment, runs `idf.py set-target esp32s3`, runs `idf.py build`, connects the watch over USB, and runs `idf.py flash`. It says the first build downloads the components from the ESP-IDF component registry.
   - The use section covers the tap that speaks, the cycle, the screen off after 2 minutes and the tap that wakes it, the swipe down from the top that opens the settings screen, the three settings and their ranges, and the three ways to go back.
   - The license section gives the copyright line and the standard GNU notice for GPL 3.0 or later, and points at `LICENSE`. It says the Yes and No words use Montserrat Bold under the SIL Open Font License, with a link to https://github.com/JulietaUla/Montserrat/blob/master/OFL.txt. It says the Atkinson Hyperlegible font and its license are in `tools/fonts/`.

5. Set the status of feature 4 to in progress in `docs/specs/roadmap.md`. This step is done during planning.
