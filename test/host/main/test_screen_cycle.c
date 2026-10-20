#include <stdlib.h>
#include "unity.h"
#include "screen_cycle.h"

static screen_cycle_t sc;

void setUp(void)
{
    screen_cycle_init(&sc, SCREEN_YES);
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
    screen_cycle_init(&sc, SCREEN_NO);
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
    TEST_ASSERT_TRUE(screen_cycle_tap(&sc));
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_FALSE(screen_cycle_tick(&sc, 4999));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
}

static void test_case_13_tap_restarts_off_count(void)
{
    screen_cycle_tick(&sc, 119000);
    TEST_ASSERT_FALSE(screen_cycle_tap(&sc));
    screen_cycle_tick(&sc, 119999);
    TEST_ASSERT_TRUE(screen_cycle_is_on(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 1));
    TEST_ASSERT_FALSE(screen_cycle_is_on(&sc));
}

static void test_case_14_tap_keeps_switch_timing(void)
{
    screen_cycle_tick(&sc, 3000);
    TEST_ASSERT_FALSE(screen_cycle_tap(&sc));
    TEST_ASSERT_EQUAL(SCREEN_YES, screen_cycle_current(&sc));
    TEST_ASSERT_TRUE(screen_cycle_tick(&sc, 2000));
    TEST_ASSERT_EQUAL(SCREEN_NO, screen_cycle_current(&sc));
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
    RUN_TEST(test_case_14_tap_keeps_switch_timing);
    exit(UNITY_END());
}
