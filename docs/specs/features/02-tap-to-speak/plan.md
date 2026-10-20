# Feature 2 plan: tap to speak

1. Write the clip script.
   - This step creates `tools/make_clips.py`.
   - The script takes the output folder as its only argument. For each word, it runs `say -v Samantha -o <word>.wav --file-format=WAVE --data-format=LEI16@22050 <Word>` in a temporary folder.
   - The script opens each WAV file with the Python `wave` module. It fails with a clear message if the file is not mono, 16-bit, and 22050 Hz.
   - The script writes the audio frames with no header to `yes.pcm` and `no.pcm` in the output folder.
   - The script uses only the Python standard library.

2. Add the speaking state to the screen and timer logic.
   - This step changes `components/screen_cycle/include/screen_cycle.h` and `components/screen_cycle/screen_cycle.c`.
   - The header adds the type `screen_tap_t` with the values `SCREEN_TAP_WAKE` and `SCREEN_TAP_SPEAK`. The struct adds the field `speaking`.
   - The function `screen_cycle_init` sets `speaking` to false.
   - The function `screen_cycle_tap` returns `screen_tap_t`. On a dark screen it does what it did in feature 1 and returns `SCREEN_TAP_WAKE`. On a lit screen it sets the 2 minute count to zero, sets `speaking` to true, and returns `SCREEN_TAP_SPEAK`.
   - The function `screen_cycle_tick` adds the time to the 2 minute count as before. While `speaking` is true, it does not add the time to the switch count and does not switch. When the screen turns off, `speaking` becomes false.
   - The new function `screen_cycle_speak_done` sets `speaking` to false and sets the switch count to zero. When `speaking` is already false, it changes nothing.
   - The new function `screen_cycle_is_speaking` returns `speaking`.

3. Update the host tests.
   - This step changes `test/host/main/test_screen_cycle.c`.
   - Cases 12, 13, and 14 change as validation.md lists. Cases 15 to 21 are new.

4. Write the playback code.
   - This step creates `main/audio.h` and `main/audio.c`.
   - The function `audio_init` takes a callback that runs when a play ends. It calls `bsp_audio_codec_speaker_init`, sets the volume to 90 with `esp_codec_dev_set_out_vol`, and opens the codec at 22050 Hz, 16 bits, and 1 channel. It makes a queue of length 1 and a playback task.
   - The function `audio_play` takes a screen value and puts it in the queue with `xQueueOverwrite`. It does not block, so the tap callback can call it.
   - The playback task waits on the queue. It writes the clip for the screen to the codec in chunks of 1024 bytes. Before each chunk, it checks the queue without a wait. If a new request is there, it starts that clip from the start. When the last chunk of a clip is written and no new request is there, it calls the end callback.
   - The clips come from the symbols `_binary_yes_pcm_start`, `_binary_yes_pcm_end`, `_binary_no_pcm_start`, and `_binary_no_pcm_end`.

5. Embed the clips and add the codec component.
   - This step changes `main/CMakeLists.txt` and `main/idf_component.yml`.
   - The CMake file finds `python3.13` with `find_program` and stops the build if it is missing. It adds a custom command that runs `tools/make_clips.py` with the component build folder as the output. The command depends on the script.
   - The CMake file embeds `yes.pcm` and `no.pcm` with `target_add_binary_data` as `BINARY`, and adds `audio.c` to the sources.
   - The manifest adds `espressif/esp_codec_dev` at version `~1.5`.

6. Connect the tap and the playback in the entry point.
   - This step changes `main/main.c`.
   - The function `app_main` calls `audio_init` after `bsp_display_start`.
   - The tap callback calls `audio_play` with the current screen when `screen_cycle_tap` returns `SCREEN_TAP_SPEAK`. When it returns `SCREEN_TAP_WAKE`, the callback does what it did in feature 1.
   - The end callback takes the display lock, calls `screen_cycle_speak_done`, and gives the lock back.

7. Build the firmware for the `esp32s3` target. If the board audio driver fails to build on ESP-IDF v6.1, stop and tell the user before any change to the driver.

8. Run the host tests, the clip check, and the firmware build, and fix any failures.
