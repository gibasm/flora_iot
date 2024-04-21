#include <stdlib.h>
#include <pico/cyw43_arch.h>
#include <lwip/netif.h>
#include <lwip/apps/httpd.h>
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "config.h"

int main(void)
{
    stdio_init_all();    

    if (cyw43_arch_init()) {
        printf("CYW43 init failed\n"); 
        return -1;
    }
    
    cyw43_state.netif->hostname = "FloraIOT";

    cyw43_arch_enable_sta_mode();
    printf("Enabled WiFi station mode\n");

    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_NO_POWERSAVE_MODE, 20, 1, 1, 1));

    printf("Connecting to network with SSID: %s\n", WIFI_SSID);

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Connection timed out\n");
        return -1;
    }
    
    printf("Connected\n");

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    extern cyw43_t cyw43_state;
    int32_t ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
    printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);

    httpd_init();
    ssi_init();
    cgi_init();

    for (;;) {
        
    }

    return 0;
}
