/*
 * main.c
 *
 *  Created on: 11/03/2022
 *      Author: ivan
 *  Revised on: 23/03/2022
 *      Author: Edel Diaz (UAH)
 */
#include "leon3_uart.h"
#include "leon3_bprint.h"
#include "leon3_hw_irqs.h"
#include "leon3_ev_handling.h"

//EDL: esto debería ir en un .h
void device_hw_irq_level_1_handler(void);
void sw_trap_vector_0x82_handler(void);


void device_hw_irq_level_1_handler(void){
leon3_print_string("Device HW IRQ user handler \n");
}

//EDL
/*Apartado 7, handler de trap 0x82 (división por cero),
* produce excepción de window overflow si se llama a leon3_print_string
* desde el trap, por lo que implemento la función aquí directamente. */
void sw_trap_vector_0x82_handler(void){
	int8_t i;
	char* str = "error, division por cero\n";

	for (i = 0; str[i] != '\0'; i++){
		leon3_putchar(str[i]);
	}

}

int main()
{
  //EDL
  unsigned int i=0;
	unsigned int j=10;
  
  //Instalar como manejador del trap 0x83 la rutina
  //que habilita las interrupciones
  leon3_set_trap_handler(0x83,leon3_trap_handler_enable_irqs);
 
  //Instalar el manejador del trap que 0x84 la rutina
  //que deshabilita las interrupciones
  leon3_set_trap_handler(0x84,leon3_trap_handler_disable_irqs);
 
 	//Apartado7 Instalar el manejador del trap que 0x82 la rutina
	//que deshabilita las interrupciones
	leon3_set_trap_handler(0x82,sw_trap_vector_0x82_handler);
 
  //Llamada al sistema para deshabilitar las interrupciones
  leon3_sys_call_disable_irqs();

  //Enmascarar todas las interrupciones
  leon3_mask_all_irqs();
 
  //Instalar la función device_hw_irq_level_1_handler como
  //manejador de usuario de la interrupción de nivel 1
  //device_hw_irq_level_1_handler();
  leon3_install_user_hw_irq_handler(1, device_hw_irq_level_1_handler);//EDEL: esta 
  // es la funcion que enlaza la interrupcion 1 a la funcion device_hw_irq_level_1_handler
 
  //Desenmascarar la interrupción de nivel 1
  //leon3_unmask_irq (0x11);
  leon3_unmask_irq (1);// EDEL: No es necesario sumarle 0x10
 
  //Llamada al sistema para habilitar las interrupciones
  //leon3_trap_handler_enable_irqs();
  leon3_sys_call_enable_irqs(); // EDEL: Esta es la funcion que deberías utilizar
 
  //Fuerza la interrupción
  //leon3_force_irq (0x11);
  leon3_force_irq (1);// EDEL: No es necesario sumarle 0x10

	//Apartado 5, enmascaro la interrupcion
	leon3_mask_irq(1);

	//Force IRQ
	leon3_force_irq(1);
	
	//Apartado 6, deshabilito irqs (comentar el sys_enable)
	leon3_sys_call_disable_irqs();

	//Force IRQ
	leon3_force_irq(1);
	
	
	//Apartado 7, fuerza división por 0, lo que lanza trap 0x82
	for (i=10;i>0;i--){
		j=j/(i-9u);

	}

  return 0;
}

