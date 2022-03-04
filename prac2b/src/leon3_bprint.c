/*
 * leon3_bprint.c
 *
 *  Created on: 02/03/2022
 *      Author: ivan
 */
#include "leon3_bprint.h"

int8_t leon3_print_string(char* str){

	uint32_t write_timeout=0;
	int cont = 0;
	char car = (char) str[cont];

	while(car != '\0'){
		leon3_putchar(car);
		cont = cont + 1;
		car = (char) str[cont];
	}
	return (write_timeout == 0xAAAAA);
}

int8_t leon3_print_uint8(uint8_t i){

	uint32_t write_timeout=0;

	if(i>99){
		leon3_putchar((i/100) + 48);
		leon3_putchar(((i%100)/10)+48);
		leon3_putchar((i%10)+48);
	} else if (i>9){
		leon3_putchar((i/10)+48);
		leon3_putchar((i/10)+48);
	} else {
		leon3_putchar(i+48);
	}
	return (write_timeout == 0xAAAAA);
}

