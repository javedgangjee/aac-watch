# Feature 7 plan: tap feedback

1. Record the spoken word in the screen and timer logic.
   - This step changes `components/screen_cycle/include/screen_cycle.h` and `components/screen_cycle/screen_cycle.c`.
   - The struct `screen_cycle_t` gets the field `spoken`, of type `screen_t`.
   - The new function `screen_cycle_tap_word` does what `screen_cycle_tap` does now. On a lit screen it also sets `spoken` to the word it is given. On a dark screen it does not set `spoken`.
   - The function `screen_cycle_tap` calls `screen_cycle_tap_word` with the current screen, so the existing tests and callers keep working.
   - The new function `screen_cycle_spoken` returns true while the screen is speaking and writes the spoken word to its second argument. It returns false when the screen is not speaking. The speaking state already ends when the word ends, when the screen turns off, and at `screen_cycle_init`.

2. Update the host tests.
   - This step changes `test/host/main/test_screen_cycle.c`.
   - The tests add cases 41 to 49 from validation.md.

3. Draw the swap.
   - This step changes `main/ui.c` and `main/ui.h`.
   - The function `make_half` also returns the word label of the half, so the code can change its color.
   - The new function `ui_set_swap` takes a flag and a word. When the flag is true, it makes the background white and the word and icon labels green or red, on the cycle screen in cycle mode or on the half of that word in split mode. When the flag is false, it puts every color back.
   - The functions `ui_show` and `ui_show_split` put every color back before they draw, so a new view always starts with its normal colors.

4. Connect the swap in the main loop.
   - This step changes `main/main.c`.
   - The function `on_tap` calls `screen_cycle_tap_word` with the word it finds, in place of `screen_cycle_tap`. After a tap that speaks, it calls `apply_state`.
   - The function `apply_state` keeps the swap that the panel shows in two new static values. After it draws the view, it asks `screen_cycle_spoken` for the swap and calls `ui_set_swap` when the swap differs from the one shown. A change of view clears the swap that is shown, because `ui_show` and `ui_show_split` put the colors back.
   - When the screen turns off, `apply_state` puts the colors back before the panel goes dark, so the screen wakes with its normal colors.
   - The function `on_speak_done` calls `apply_state` after it ends the word, unless the settings screen is open. The function `on_settings_close` already calls `apply_state`.

5. Update the README.
   - This step changes `README.md`.
   - The use section says that the tapped screen or half shows white with the word in green or red while the word plays. The `deslopificate` skill checks the changed text for AI tells and tropes.

6. Set the status of feature 7 to in progress in `docs/specs/roadmap.md`. This step is done during planning.
