#pragma once

u16_t __time_critical_func(ssi_handler)(int idx, char* insert, int insert_len);

void ssi_init(void);
