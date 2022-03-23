
/*
 *  Revised on: 23/03/2022
 *      Author: Edel Diaz (UAH)
 */

#include "leon3_uart.h"
#include "leon3_bprint.h"
#include "leon3_hw_irqs.h"
#include "leon3_ev_handling.h"
uint8_t irq_counter=0;
char RxChars[8];
/*Esta función se utilizará como manejador de la interrupción
que genera la recepción de un dato por el puerto serie.
La función almacena en RxChars los primeros 8 carateres que se
reciben, reenviando, además el sucesor por el mismo canal Tx*/
void uart_rx_irq_handler(void);
void uart_rx_irq_handler(void){
  char aux;
  aux=leon3_getchar();
  if(irq_counter<7){
  RxChars[irq_counter]=aux;
  aux++;
  leon3_putchar(aux);
  }
  irq_counter++;
}

int main()
{
uint8_t i;
  //Instalar como manejador del trap 0x83 la rutina
  // que habilita las interrupciones
  leon3_set_trap_handler(0x83,leon3_trap_handler_enable_irqs);
  //Instalar el manejador del trap que 0x83 la rutina
  // que deshabilita las interrupciones
  leon3_set_trap_handler(0x84,leon3_trap_handler_disable_irqs);
  //Llamada al sistema para deshabilitar las interrupciones
  leon3_sys_call_disable_irqs();
  //COMPLETAR instalando como manejador de la interrupción de

  //EDEL: Enmascarar todas las interrupciones, sólo desenmascaremos aquellas que tenemos manejadas.
  leon3_mask_all_irqs();

  //nivel 2 la rutina uart_rx_irq_handler siguiendo el mismo
  //EDEL: leon3_install_user_hw_irq_handler ya suma 0x10 internamente.
  //leon3_install_user_hw_irq_handler(0x12,uart_rx_irq_handler);
  	leon3_install_user_hw_irq_handler(2, uart_rx_irq_handler);

  //Habilito loop-back
  leon3_uart_ctrl_config_rxtx_loop(1);
  //Habilito interrupción de recepción por la UART
  leon3_uart_ctrl_rx_irq_enable();
  //Habilito la recepción por la UART
  leon3_uart_ctrl_rx_enable ();
  //COMPLETAR habilitando las interrupciones y
  //desenmascarando la interrupción de nivel 2
  
  
	//EDEL: Mejora si hacen el unmask después del enable_irqs:
	//Mejor llamar primero al unmask y luego al enable_irqs. Mira la parte 3a
	//Desenmascarar la interrupción de nivel 2
	leon3_unmask_irq(2);

  leon3_sys_call_enable_irqs();
  //leon3_unmask_irq(2);


  //FIN COMPLETAR
  leon3_putchar('A');
  //Sondeo si los 8 caracteres se han recibido
  while(irq_counter < 7)
  ;
  //Tras recibir los 8 caracteres, configuro la UART
  //sin loop-back
  leon3_uart_ctrl_config_rxtx_loop(0);
  //Envío de nuevo los 8 caracteres que se recibieron,
  //pero sin loop-back, por lo que aparecerán por pantalla
  for(i=0;i<8;i++)
  leon3_putchar(RxChars[i]);
  leon3_putchar('\n');
  //Espero a que todos los caracteres se hayan enviado.
  while(!leon3_uart_tx_fifo_is_empty())
  ;
  return 0;
 }
