#include "waterctl.h"
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "gpio_conf.h"

//TODO: add mutex-like atomic guards to prevent user
//from setting the alarm multiple times

static int64_t water_off_callback(
        [[maybe_unused]] alarm_id_t id, 
        [[maybe_unused]] void* user_data)
{
    printf("Water OFF\n\r");     
    gpio_put(WATER_SOURCE0_PIN, 1);

    return 0;
}

void water_on_for_async(int seconds)
{
    printf("Water ON\n\r");    
    gpio_put(WATER_SOURCE0_PIN, 0);
    add_alarm_in_ms(seconds * 1000, water_off_callback, NULL, false);
}

void waterctl_init()
{
    gpio_init(WATER_SOURCE0_PIN);
    gpio_set_dir(WATER_SOURCE0_PIN, GPIO_OUT);
    gpio_put(WATER_SOURCE0_PIN, 1);
}
