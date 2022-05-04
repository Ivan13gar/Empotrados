/*
 * leon3_uart.c
 *
 *  Created on: 18/02/2022
 *      Author: ivan
 */
#include "leon3_uart.h"
#include "leon3_types.h"
//Estructura de datos que permite acceder a los registros de la
//UART de LEON3
struct UART_regs
{

 /** \brief UART Data Register */
 volatile uint32_t Data; /* 0x80000100 */
 /** \brief UART Status Register */
 volatile uint32_t Status; /* 0x80000104 */
 /** \brief UART Control Register */
 volatile uint32_t Ctrl; /* 0x80000108 */
 /** \brief UART Scaler Register */
 volatile uint32_t Scaler; /* 0x8000010C */
 };


struct UART_regs * const pLEON3_UART_REGS= (struct UART_regs *)0x80000100;   //puntero a pLEON3_UART_REGS

//! LEON3 UART A Transmit FIFO is FULL
#define LEON3_UART_TFF (0x200)
//! LEON3 UART A Transmit FIFO is EMPTY
#define LEON3_UART_TFE (0x4)
#define LEON3_UART_RE (0x1);         //posición en hexadecimal del campo Receiver enable de Control
#define LEON3_UART_RIE (0x4);         //posición en hexadecimal del campo Receiver interrupt enable de Control
#define LEON3_UART_LB (0x80); //posición en hexadecimal del campo Loop back de Control

#define leon3_UART_TF_IS_FULL() ((pLEON3_UART_REGS -> Status ) & LEON3_UART_TFF) //devuelve 1 si la condición se cumple, y si no, devuelve 0

int8_t leon3_putchar(char c)
{

 uint32_t write_timeout=0;

 while(leon3_UART_TF_IS_FULL() && (write_timeout < 0xAAAAA))
 {
	 write_timeout++;

 } //Espera mientras la cola de transmisión esté llena
 if(write_timeout < 0xAAAAA){
 //COMPLETAR. Escribir el carácter en el registro Data
	 pLEON3_UART_REGS->Data = (volatile uint32_t) c;
 }
 return (write_timeout == 0xAAAAA);
}
int8_t leon3_uart_tx_fifo_is_empty(){
	//LEON3_UART_TFE()

	uint8_t uart_tf_is_empty;
	uart_tf_is_empty = pLEON3_UART_REGS->Status & LEON3_UART_TFE;
	return uart_tf_is_empty;

}

/*struct UART_regs * const Data= (struct UART_regs *)0x80000100;               //puntero a Data
struct UART_regs * const Status= (struct UART_regs *)0x80000104;			 //puntero a Status
struct UART_regs * const Ctrl= (struct UART_regs *)0x80000108;				 //puntero a Ctrl
struct UART_regs * const Scaler= (struct UART_regs *)0x8000010C;			 //puntero a Scaler*/


char leon3_getchar(){

	uint8_t valor;
	return valor = pLEON3_UART_REGS->Data;
}

void leon3_uart_ctrl_rx_enable(){

	pLEON3_UART_REGS->Ctrl |= LEON3_UART_RE;

}
void leon3_uart_ctrl_rx_irq_enable(){

	pLEON3_UART_REGS->Ctrl |= LEON3_UART_RIE;

}
void leon3_uart_ctrl_config_rxtx_loop(uint8_t set_rxtxloop){

		if(set_rxtxloop == 1){
			pLEON3_UART_REGS->Ctrl |= LEON3_UART_LB;
		} else{
			pLEON3_UART_REGS->Ctrl &= ~LEON3_UART_LB;
		}
}












