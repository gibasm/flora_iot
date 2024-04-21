#pragma once

void cgi_init(void);

const char* cgi_set_hydration_handler(int index, int num_params, char* param[], char* value[]);

const char* cgi_water_on_handler(int index, int num_params, char* param[], char* value[]);
