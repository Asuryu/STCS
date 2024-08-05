#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "config.h"


static void test_relationship(void **state) {
    ThermalPair *pair_array = (ThermalPair *) *state;
    modify_temperatures(&pair_array[0], ECLIPSE);
    assert_float_not_equal(pair_array[0].thermistor, 6.0, 1e-9);
    for (int i = 1; i < 4; i++) {
        assert_float_equal(pair_array[i].thermistor, 6.0, 1e-4);
    }
}


int main(void) {
    struct ThermalPair pair_array[4] = {
        {1, 6.0},    
        {1, 6.0},
        {1, 6.0},
        {1, 6.0}
    };
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_prestate(test_relationship, pair_array),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}