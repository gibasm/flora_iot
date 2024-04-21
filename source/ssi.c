#include <hardware/adc.h>
#include <lwip/apps/httpd.h>
#include <pico/cyw43_arch.h>
#include "lwipopts.h"
#include "ssi.h"
#include "gpio_conf.h"

const char* __not_in_flash("httpd") tags[] = {
    "hydr", // hydration level from the sensor
    "ctmp", // core temperature
};

u16_t __time_critical_func(ssi_handler)(int idx, char* insert, int insert_len)
{ 
    size_t printed;
    float sensor_value;

    switch(idx)
    {
        case 0: // hydr
        adc_select_input(HYDR_SENS0_ADC_SEL);
        sensor_value = adc_read() * 3.3f / (1 << 12); 
        printed = snprintf(insert, insert_len, "%0.2f", sensor_value);
        break;
        case 1: // ctmp
        default:
        adc_select_input(CTEMP_SENS_ADC_SEL);
        sensor_value = 27.0f - ((adc_read() * 3.3f / (1 << 12)) - 0.706f) / 0.001721f;
        printed = snprintf(insert, insert_len, "%0.2f", sensor_value);
        break;
    }    

    LWIP_ASSERT("sane length", printed <= 0xFFFF);

    return (u16_t)printed;
}

void ssi_init(void)
{
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_gpio_init(HYDR_SENS0_PIN);

    for (size_t i = 0; i < LWIP_ARRAYSIZE(tags); i++) {
        LWIP_ASSERT("tag too long for LWIP_HTTPD_MAX_TAG_NAME_LEN",
                    strlen(tags[i]) <= LWIP_HTTPD_MAX_TAG_NAME_LEN);
    }

    http_set_ssi_handler(ssi_handler, tags, LWIP_ARRAYSIZE(tags));
}
