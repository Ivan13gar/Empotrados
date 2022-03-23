/*
 * leon3_bprint.h
 *
 *  Created on: 02/03/2022
 *      Author: ivan
 *  Revised on: 23/03/2022
 *      Author: Edel Diaz (UAH)
 */
#ifndef LEON3_BPRINT_H_
#define LEON3_BPRINT_H_

#include "leon3_types.h"
int8_t leon3_print_string(char* str);
int8_t leon3_print_uint8(uint8_t i);
int8_t leon3_print_uint32(uint32_t i); //EDL: esto debería estar definido aquí
#endif

