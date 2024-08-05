#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "../../../implementation/TSL/config.h"


static void test_verify_periods(void **state) {
    TSL_data tsl;

    tsl.clock = 0x0010;
    verify_periods(&tsl);
    assert_int_equal(tsl.period, NORMAL);

    tsl.clock = 0x0020;
    verify_periods(&tsl);
    assert_int_equal(tsl.period, ECLIPSE);

    tsl.clock = 0x0040;
    verify_periods(&tsl);
    assert_int_equal(tsl.period, SUN_EXPOSURE);
}


static void test_modify_temperatures(void **state) {
    ThermalPair TP_block[1] = {{1, 0}};

    modify_temperatures(TP_block, NORMAL);
    assert_float_equal(TP_block[0].thermistor, 1, 0.001);

    modify_temperatures(TP_block, ECLIPSE);
    assert_float_equal(TP_block[0].thermistor, 5, 0.001);

    modify_temperatures(TP_block, SUN_EXPOSURE);
    assert_float_equal(TP_block[0].thermistor, 12, 0.001);

    assert_int_equal(modify_temperatures(TP_block, 4), -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_verify_periods),
        cmocka_unit_test(test_modify_temperatures),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}