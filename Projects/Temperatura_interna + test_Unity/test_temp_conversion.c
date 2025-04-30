#include "unity/unity.h"
#include "temp_utils.h"

void setUp(void) {}    // obrigatório pelo Unity
void tearDown(void) {} // obrigatório pelo Unity

void test_adc_706mV_returns_27C(void) {
    uint16_t adc_val = (uint16_t)((0.706 / 3.3) * 4095); // Valor ADC correspondente a 0.706 V
    float temp = adc_to_celsius(adc_val);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 27.0, temp);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_706mV_returns_27C);
    return UNITY_END();
}
