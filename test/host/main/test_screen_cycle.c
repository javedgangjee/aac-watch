#include <stdlib.h>
#include "unity.h"
#include "screen_cycle.h"

void run_settings_tests(void);

static screen_cycle_t sc;

void setUp(void)
{
    screen_cycle_init(&sc, SCREEN_YES, SCREEN_CYCLE_DEFAULT_PERIOD_MS);
}

void tearDown(void)
{
}

/* Tick 50 ms 2400 times. Check the screen is on after the 2399th tick. */
static void tick_until_off(void)
{
    for (int i = 1; i < 2400; i++) {
        screen_cycle_tick(&sc, 50);
    }
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 50));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_01_starts_on_yes(void)
{
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
}

static void test_case_02_starts_on_no(void)
{
    screen_cycle_init(&sc, SCREEN_NO, SCREEN_CYCLE_DEFAULT_PERIOD_MS);
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_03_no_switch_before_period(void)
{
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_04_switch_at_period(void)
{
    screen_cycle_tick(&sc, 4999);
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_05_two_full_periods(void)
{
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 5000));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 5000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_06_leftover_kept_short(void)
{
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4000));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 3000));
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 2999));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_07_leftover_kept_full(void)
{
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4000));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 3000));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 3000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_08_small_ticks(void)
{
    for (int i = 1; i < 100; i++) {
        TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 50));
    }
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 50));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_09_zero_tick(void)
{
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 0));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_10_off_after_two_minutes(void)
{
    tick_until_off();
}

static void test_case_11_off_screen_ignores_ticks(void)
{
    tick_until_off();
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 10000));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_12_tap_on_dark_screen(void)
{
    tick_until_off();
    TEST_ASSERT_EQUAL(SCREEN_TAP_WAKE, screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_FALSE(screen_cycle_is_speaking(&sc));
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_13_tap_restarts_off_count(void)
{
    screen_cycle_tick(&sc, 119000);
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    screen_cycle_tick(&sc, 119999);
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    TEST_ASSERT_FALSE(screen_cycle_is_speaking(&sc));
}

static void test_case_14_full_period_after_word(void)
{
    screen_cycle_tick(&sc, 3000);
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_15_tap_starts_speaking(void)
{
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_speaking(&sc));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_16_no_switch_while_speaking(void)
{
    screen_cycle_tick(&sc, 3000);
    screen_cycle_tap(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 10000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_speaking(&sc));
}

static void test_case_17_hold_then_full_period(void)
{
    screen_cycle_tick(&sc, 4000);
    screen_cycle_tap(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 2000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_18_tap_while_speaking(void)
{
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    screen_cycle_tick(&sc, 200);
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_speaking(&sc));
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_is_speaking(&sc));
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_19_speak_done_without_tap(void)
{
    screen_cycle_tick(&sc, 3000);
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 2000));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_20_off_while_speaking(void)
{
    screen_cycle_tap(&sc);
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 120000));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    TEST_ASSERT_FALSE(screen_cycle_is_speaking(&sc));
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 10000));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
}

static void test_case_21_wake_tap_then_speak_tap(void)
{
    tick_until_off();
    TEST_ASSERT_EQUAL(SCREEN_TAP_WAKE, screen_cycle_tap(&sc));
    TEST_ASSERT_FALSE(screen_cycle_is_speaking(&sc));
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_speaking(&sc));
}

static void test_case_22_period_3000(void)
{
    screen_cycle_set_period(&sc, 3000);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 2999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_23_period_15000(void)
{
    screen_cycle_set_period(&sc, 15000);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 14999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_24_set_period_clears_switch_count(void)
{
    screen_cycle_tick(&sc, 3000);
    screen_cycle_set_period(&sc, 5000);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_25_wake_keeps_period(void)
{
    screen_cycle_set_period(&sc, 8000);
    screen_cycle_tick(&sc, 120000);
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    TEST_ASSERT_EQUAL(SCREEN_TAP_WAKE, screen_cycle_tap(&sc));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 7999));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_26_full_period_after_word(void)
{
    screen_cycle_set_period(&sc, 3000);
    screen_cycle_tap(&sc);
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 2999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_27_off_time_with_long_period(void)
{
    screen_cycle_set_period(&sc, 15000);
    screen_cycle_tick(&sc, 119999);
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
}

static void test_case_28_split_off_at_start(void)
{
    TEST_ASSERT_FALSE(screen_cycle_is_split(&sc));
}

static void test_case_29_split_does_not_switch(void)
{
    screen_cycle_set_split(&sc, true);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 10000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
}

static void test_case_30_split_off_after_two_minutes(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tick(&sc, 119999);
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
}

static void test_case_31_split_wake_keeps_split(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tick(&sc, 120000);
    TEST_ASSERT_EQUAL(SCREEN_TAP_WAKE, screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_split(&sc));
}

static void test_case_32_split_tap_restarts_off_count(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tick(&sc, 60000);
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap(&sc));
    screen_cycle_tick(&sc, 119999);
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
}

static void test_case_33_split_no_switch_after_word(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tap(&sc);
    screen_cycle_speak_done(&sc);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 10000));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
}

static void test_case_34_split_off_clears_switch_count(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tick(&sc, 3000);
    screen_cycle_set_split(&sc, false);
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

/* Check the word under a press at row y on a screen 502 pixels high. */
static void assert_split_word(int32_t y, bool on_half, screen_t expected)
{
    screen_t word;
    if (on_half) {
        TEST_ASSERT_TRUE(screen_cycle_split_word(y, 502, &word));
        TEST_ASSERT_EQUAL(expected, word);
    } else {
        TEST_ASSERT_FALSE(screen_cycle_split_word(y, 502, &word));
    }
}

static void test_case_35_split_word_top_row(void)
{
    assert_split_word(0, true, SCREEN_YES);
}

static void test_case_36_split_word_last_yes_row(void)
{
    assert_split_word(245, true, SCREEN_YES);
}

static void test_case_37_split_word_first_band_row(void)
{
    assert_split_word(246, false, SCREEN_YES);
}

static void test_case_38_split_word_last_band_row(void)
{
    assert_split_word(255, false, SCREEN_YES);
}

static void test_case_39_split_word_first_no_row(void)
{
    assert_split_word(256, true, SCREEN_NO);
}

static void test_case_40_split_word_bottom_row(void)
{
    assert_split_word(501, true, SCREEN_NO);
}

/* Check that screen_cycle_spoken returns true and gives the expected word. */
static void assert_spoken(screen_t expected)
{
    screen_t word = (expected == SCREEN_YES) ? SCREEN_NO : SCREEN_YES;
    TEST_ASSERT_TRUE(screen_cycle_spoken(&sc, &word));
    TEST_ASSERT_EQUAL(expected, word);
}

static void assert_not_spoken(void)
{
    screen_t word;
    TEST_ASSERT_FALSE(screen_cycle_spoken(&sc, &word));
}

static void test_case_41_not_spoken_at_start(void)
{
    assert_not_spoken();
}

static void test_case_42_tap_speaks_yes(void)
{
    screen_cycle_tap(&sc);
    assert_spoken(SCREEN_YES);
}

static void test_case_43_tap_speaks_no(void)
{
    screen_cycle_tick(&sc, 5000);
    screen_cycle_tap(&sc);
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
    assert_spoken(SCREEN_NO);
}

static void test_case_44_word_end_clears_spoken(void)
{
    screen_cycle_tap(&sc);
    screen_cycle_speak_done(&sc);
    assert_not_spoken();
}

static void test_case_45_split_tap_no(void)
{
    screen_cycle_set_split(&sc, true);
    TEST_ASSERT_EQUAL(SCREEN_TAP_SPEAK, screen_cycle_tap_word(&sc, SCREEN_NO));
    assert_spoken(SCREEN_NO);
}

static void test_case_46_split_tap_other_half(void)
{
    screen_cycle_set_split(&sc, true);
    screen_cycle_tap_word(&sc, SCREEN_YES);
    assert_spoken(SCREEN_YES);
    screen_cycle_tick(&sc, 200);
    screen_cycle_tap_word(&sc, SCREEN_NO);
    assert_spoken(SCREEN_NO);
}

static void test_case_47_wake_tap_not_spoken(void)
{
    tick_until_off();
    TEST_ASSERT_EQUAL(SCREEN_TAP_WAKE, screen_cycle_tap_word(&sc, SCREEN_NO));
    assert_not_spoken();
}

static void test_case_48_off_clears_spoken(void)
{
    screen_cycle_tap(&sc);
    screen_cycle_tick(&sc, 120000);
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
    assert_not_spoken();
}

static void test_case_49_init_clears_spoken(void)
{
    screen_cycle_tap(&sc);
    screen_cycle_init(&sc, SCREEN_YES, SCREEN_CYCLE_DEFAULT_PERIOD_MS);
    assert_not_spoken();
}

void app_main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_case_01_starts_on_yes);
    RUN_TEST(test_case_02_starts_on_no);
    RUN_TEST(test_case_03_no_switch_before_period);
    RUN_TEST(test_case_04_switch_at_period);
    RUN_TEST(test_case_05_two_full_periods);
    RUN_TEST(test_case_06_leftover_kept_short);
    RUN_TEST(test_case_07_leftover_kept_full);
    RUN_TEST(test_case_08_small_ticks);
    RUN_TEST(test_case_09_zero_tick);
    RUN_TEST(test_case_10_off_after_two_minutes);
    RUN_TEST(test_case_11_off_screen_ignores_ticks);
    RUN_TEST(test_case_12_tap_on_dark_screen);
    RUN_TEST(test_case_13_tap_restarts_off_count);
    RUN_TEST(test_case_14_full_period_after_word);
    RUN_TEST(test_case_15_tap_starts_speaking);
    RUN_TEST(test_case_16_no_switch_while_speaking);
    RUN_TEST(test_case_17_hold_then_full_period);
    RUN_TEST(test_case_18_tap_while_speaking);
    RUN_TEST(test_case_19_speak_done_without_tap);
    RUN_TEST(test_case_20_off_while_speaking);
    RUN_TEST(test_case_21_wake_tap_then_speak_tap);
    RUN_TEST(test_case_22_period_3000);
    RUN_TEST(test_case_23_period_15000);
    RUN_TEST(test_case_24_set_period_clears_switch_count);
    RUN_TEST(test_case_25_wake_keeps_period);
    RUN_TEST(test_case_26_full_period_after_word);
    RUN_TEST(test_case_27_off_time_with_long_period);
    RUN_TEST(test_case_28_split_off_at_start);
    RUN_TEST(test_case_29_split_does_not_switch);
    RUN_TEST(test_case_30_split_off_after_two_minutes);
    RUN_TEST(test_case_31_split_wake_keeps_split);
    RUN_TEST(test_case_32_split_tap_restarts_off_count);
    RUN_TEST(test_case_33_split_no_switch_after_word);
    RUN_TEST(test_case_34_split_off_clears_switch_count);
    RUN_TEST(test_case_35_split_word_top_row);
    RUN_TEST(test_case_36_split_word_last_yes_row);
    RUN_TEST(test_case_37_split_word_first_band_row);
    RUN_TEST(test_case_38_split_word_last_band_row);
    RUN_TEST(test_case_39_split_word_first_no_row);
    RUN_TEST(test_case_40_split_word_bottom_row);
    RUN_TEST(test_case_41_not_spoken_at_start);
    RUN_TEST(test_case_42_tap_speaks_yes);
    RUN_TEST(test_case_43_tap_speaks_no);
    RUN_TEST(test_case_44_word_end_clears_spoken);
    RUN_TEST(test_case_45_split_tap_no);
    RUN_TEST(test_case_46_split_tap_other_half);
    RUN_TEST(test_case_47_wake_tap_not_spoken);
    RUN_TEST(test_case_48_off_clears_spoken);
    RUN_TEST(test_case_49_init_clears_spoken);
    run_settings_tests();
    exit(UNITY_END());
}
