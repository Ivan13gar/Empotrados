/*
 * leon3_bprint.c
 *
 *  Created on: 02/03/2022
 *      Author: ivan
 *  Revised on: 23/03/2022
 *      Author: Edel Diaz (UAH)
 */
#include "leon3_bprint.h"
#include "leon3_uart.h" //EDEL: esto es para usar leon3_uart_tx_fifo_is_empty

int8_t leon3_print_string(char* str){

	uint32_t write_timeout=0;
	int cont = 0;
	char car = (char) str[cont];
  int error = 0;

	while(car != '\0'){
		error = leon3_putchar(car);
		cont = cont + 1;
		car = (char) str[cont];
	}
  
  //EDEL: Error: Debes añadir la espera a que la fifo de transmisión se vacie
	while(!leon3_uart_tx_fifo_is_empty());
  
  //EDEL: Error: lo que se devuelve debe ser el error
	return (error);
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


//uint8 [255-0]
uint32_t leon3_print_uint32(uint32_t i){

	int8_t error=0;
	uint8_t aux = 1000000000;//2 elevado a 32
	int8_t first_digit = 0;

	if(i==0)
		error=leon3_putchar('0');
	else{
		while(aux&&(!error)){
			uint8_t digit;

			digit=i/aux;
			i-=aux*digit;
			aux=aux/10;
			if((first_digit)||digit){
				error=leon3_putchar('0'+digit);
				first_digit=1;
			}
		}
	}

	while(!leon3_uart_tx_fifo_is_empty());

  return error;
}
