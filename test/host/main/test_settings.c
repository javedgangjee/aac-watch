#include "unity.h"
#include "settings.h"

static void assert_settings(settings_voice_t voice, uint8_t cycle_s, uint8_t volume, const settings_t *s)
{
    TEST_ASSERT_EQUAL(voice, s->voice);
    TEST_ASSERT_EQUAL_UINT8(cycle_s, s->cycle_s);
    TEST_ASSERT_EQUAL_UINT8(volume, s->volume);
}

static void test_case_s01_defaults(void)
{
    settings_t s;
    settings_defaults(&s);
    assert_settings(SETTINGS_VOICE_SAMANTHA, 5, 90, &s);
    TEST_ASSERT_EQUAL_UINT32(5000, settings_cycle_ms(&s));
}

static void test_case_s02_cycle_up_once(void)
{
    settings_t s;
    settings_defaults(&s);
    TEST_ASSERT_TRUE(settings_cycle_step(&s, +1));
    TEST_ASSERT_EQUAL_UINT8(6, s.cycle_s);
}

static void test_case_s03_cycle_up_to_max(void)
{
    settings_t s;
    settings_defaults(&s);
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_TRUE(settings_cycle_step(&s, +1));
    }
    TEST_ASSERT_FALSE(settings_cycle_step(&s, +1));
    TEST_ASSERT_EQUAL_UINT8(15, s.cycle_s);
    TEST_ASSERT_EQUAL_UINT32(15000, settings_cycle_ms(&s));
}

static void test_case_s04_cycle_down_to_min(void)
{
    settings_t s;
    settings_defaults(&s);
    TEST_ASSERT_TRUE(settings_cycle_step(&s, -1));
    TEST_ASSERT_TRUE(settings_cycle_step(&s, -1));
    TEST_ASSERT_FALSE(settings_cycle_step(&s, -1));
    TEST_ASSERT_EQUAL_UINT8(3, s.cycle_s);
}

static void test_case_s05_volume_down_to_min(void)
{
    settings_t s;
    settings_defaults(&s);
    for (int i = 0; i < 8; i++) {
        TEST_ASSERT_TRUE(settings_volume_step(&s, -1));
    }
    TEST_ASSERT_FALSE(settings_volume_step(&s, -1));
    TEST_ASSERT_EQUAL_UINT8(10, s.volume);
}

static void test_case_s06_volume_up_to_max(void)
{
    settings_t s;
    settings_defaults(&s);
    TEST_ASSERT_TRUE(settings_volume_step(&s, +1));
    TEST_ASSERT_FALSE(settings_volume_step(&s, +1));
    TEST_ASSERT_EQUAL_UINT8(100, s.volume);
}

static void test_case_s07_sanitize_defaults(void)
{
    settings_t s;
    settings_defaults(&s);
    TEST_ASSERT_FALSE(settings_sanitize(&s));
    assert_settings(SETTINGS_VOICE_SAMANTHA, 5, 90, &s);
}

static void test_case_s08_sanitize_all_out_of_range(void)
{
    settings_t s = { .voice = (settings_voice_t)7, .cycle_s = 2, .volume = 95 };
    TEST_ASSERT_TRUE(settings_sanitize(&s));
    assert_settings(SETTINGS_VOICE_SAMANTHA, 5, 90, &s);
}

static void test_case_s09_sanitize_keeps_valid_voice(void)
{
    settings_t s = { .voice = SETTINGS_VOICE_REED, .cycle_s = 16, .volume = 0 };
    TEST_ASSERT_TRUE(settings_sanitize(&s));
    assert_settings(SETTINGS_VOICE_REED, 5, 90, &s);
}

static void test_case_s10_sanitize_limits(void)
{
    settings_t s = { .voice = SETTINGS_VOICE_REED, .cycle_s = 15, .volume = 10 };
    TEST_ASSERT_FALSE(settings_sanitize(&s));
    assert_settings(SETTINGS_VOICE_REED, 15, 10, &s);
}

static void test_case_s11_equal_defaults(void)
{
    settings_t a;
    settings_t b;
    settings_defaults(&a);
    settings_defaults(&b);
    TEST_ASSERT_TRUE(settings_equal(&a, &b));
}

static void test_case_s12_not_equal_one_field(void)
{
    settings_t a;
    settings_t b;
    settings_defaults(&a);

    b = a;
    b.voice = SETTINGS_VOICE_REED;
    TEST_ASSERT_FALSE(settings_equal(&a, &b));

    b = a;
    b.cycle_s = 6;
    TEST_ASSERT_FALSE(settings_equal(&a, &b));

    b = a;
    b.volume = 80;
    TEST_ASSERT_FALSE(settings_equal(&a, &b));
}

void run_settings_tests(void)
{
    RUN_TEST(test_case_s01_defaults);
    RUN_TEST(test_case_s02_cycle_up_once);
    RUN_TEST(test_case_s03_cycle_up_to_max);
    RUN_TEST(test_case_s04_cycle_down_to_min);
    RUN_TEST(test_case_s05_volume_down_to_min);
    RUN_TEST(test_case_s06_volume_up_to_max);
    RUN_TEST(test_case_s07_sanitize_defaults);
    RUN_TEST(test_case_s08_sanitize_all_out_of_range);
    RUN_TEST(test_case_s09_sanitize_keeps_valid_voice);
    RUN_TEST(test_case_s10_sanitize_limits);
    RUN_TEST(test_case_s11_equal_defaults);
    RUN_TEST(test_case_s12_not_equal_one_field);
}
