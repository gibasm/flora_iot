#include <string.h>
#include <lwip/apps/httpd.h>
#include <pico/cyw43_arch.h>
#include "lwipopts.h"
#include "cgi.h"
#include "waterctl.h"

static const tCGI cgi_handlers[] = {
    {
        "/set_hydration.cgi", cgi_set_hydration_handler 
    },
    {
        "/water_on.cgi", cgi_water_on_handler 
    },
};

const char* cgi_set_hydration_handler(int index, int num_params, char* param[], char* value[])
{
    //TODO: set optimal hydration level to reach and drive
    //the water source valve accordingly
    return "/index.shtml";
}

const char* cgi_water_on_handler(int index, int num_params, char* param[], char* value[])
{
    int seconds = 0;
    if(num_params < 1) {
        printf("/water_on.cgi: Invalid number of parameters\n\r");
        return "/index.shtml";
    }

    if(strncmp(param[0], "time", 5) != 0) {
        printf("/water_on.cgi: Invalid parameter name, valid: time\n\r");
        return "/index.shtml";
    }

    seconds = atoi(value[0]);
    if((seconds <= 0) || (seconds > 255)) {
        printf("/water_on.cgi: Number of seconds must be between 1 and 255\n\r");
        return "/index.shtml";
    }
   
    water_on_for_async(seconds); 

    return "/index.shtml";
}

void cgi_init(void)
{
    http_set_cgi_handlers(cgi_handlers, 2);
    waterctl_init();
}
