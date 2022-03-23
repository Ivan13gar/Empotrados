/*
 * main.c
 *
 *  Created on: 11/03/2022
 *      Author: ivan
 */
#include "leon3_uart.h"
#include "leon3_bprint.h"
#include "leon3_hw_irqs.h"
#include "leon3_ev_handling.h"

void device_hw_irq_level_1_handler(void);


void device_hw_irq_level_1_handler(void){
leon3_print_string("Device HW IRQ user handler \n");
}
int main()
{
 //Instalar como manejador del trap 0x83 la rutina
 // que habilita las interrupciones
 leon3_set_trap_handler(0x83,leon3_trap_handler_enable_irqs);
 //Instalar el manejador del trap que 0x83 la rutina
 // que deshabilita las interrupciones
 leon3_set_trap_handler(0x84,leon3_trap_handler_disable_irqs);
 //Llamada al sistema para deshabilitar las interrupciones
 leon3_sys_call_disable_irqs();
//COMPLETAR
//
//
//Enmascarar todas las interrupciones
 leon3_mask_all_irqs();
 //Instalar la función device_hw_irq_level_1_handler como
 // manejador de usuario de la interrupción de nivel 1
 device_hw_irq_level_1_handler();
 //Desenmascarar la interrupción de nivel 1
 leon3_unmask_irq (0x11);
 //Llamada al sistema para habilitar las interrupciones
 leon3_trap_handler_enable_irqs();
//Fuerza la interrupción
 leon3_force_irq (0x11);
//FIN COMPLETAR
return 0;
}

