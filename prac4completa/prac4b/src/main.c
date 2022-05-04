/*
 * main.c
 *
 *  Created on: 08/04/2022
 *      Author: ivan
 */
#include "leon3_ev_handling.h"
#include "leon3_hw_irqs.h"
#include "leon3_uart.h"
#include "leon3_bprint.h"
#include "leon3_monotonic_clk.h"
#include "leon3_timer_unit_drv.h"
#include "leon3_timing_service.h"
// Código de la tarea T1
void Task1(void){
 leon3_print_string(" Start T1\n");
 emu_execution_time(1000); //execution time 1000 ms
 leon3_putchar('\n');
 leon3_print_string(" End T1\n");
}
// Código de la tarea T2
void Task2(void){
 leon3_print_string(" Start T2\n");
 emu_execution_time(300); //execution time 300 ms
 leon3_putchar('\n');
 leon3_print_string(" End T2\n");
}
// Código de la tarea T3
void Task3(void){
 leon3_print_string(" Start T3\n");
 emu_execution_time(500); //execution time 500 ms
  leon3_putchar('\n');
  leon3_print_string(" End T3\n");
 }
 // Definición de las macros de configuración
 #define CYClIC_EXECUTIVE_PERIOD_IN_TICKS 20
 #define CYClIC_EXECUTIVE_HYPER_PERIOD 6
 #define CYClIC_EXECUTIVE_TASKS_NUMBER 3
 // Array bidimensional para definir la secuencia de tareas del ejecutivo cíclico
 void (*cyclic_executive [CYClIC_EXECUTIVE_HYPER_PERIOD]
 [CYClIC_EXECUTIVE_TASKS_NUMBER+1])(void)={
 {Task1,Task2,Task3,NULL},

 {Task1,NULL,NULL,NULL},

 {Task2,Task1,Task3,NULL},

 {Task1,Task2,NULL,NULL},

 {Task1,Task3,NULL,NULL},

 {Task2,Task1,NULL,NULL}
 };
 // Función principal
 int main()
 {
  //Install handlers for enable and disable irqs
  leon3_set_trap_handler(0x83, leon3_trap_handler_enable_irqs);
  leon3_set_trap_handler(0x84 , leon3_trap_handler_disable_irqs);
  //Declaración de variables de control del ejecutivo cíclico
  uint8_t current_period=0;
  int task_index=0;
  uint64_t next_period_in_ticks_from_reset;
  //Inicialización del servicio de temporización y toma de referencia absoluta
  //del número de ticks desde el reset del sistema
  //Init Timing Service
  init_timing_service( date_time_to_Y2K(18, 3, 22, 0, 0, 0 ));
  //Get Absolute reference
  next_period_in_ticks_from_reset=get_tick_counter_from_reset();
  while(1){
  task_index=0; //poner a 0 al inicio de cada periodo básico
  leon3_print_string("\nStart period\n");
  print_date_time_from_Y2K(get_universal_time_Y2K());
  // Control de la ejecución de las tareas de cada período básico
   while(cyclic_executive[current_period][task_index]){
   cyclic_executive[current_period][task_index]();
   task_index++;
   }
   //Sincronización con el inicio del siguiente período básico
   //Update Absolute reference with next period
   next_period_in_ticks_from_reset+=CYClIC_EXECUTIVE_PERIOD_IN_TICKS;
   wait_until(next_period_in_ticks_from_reset);
   //Next basic period
   current_period++;
   if(current_period==CYClIC_EXECUTIVE_HYPER_PERIOD){
   current_period=0;
   leon3_print_string("\n****************\n");
   leon3_print_string("\nNext hyperperiod\n");
   leon3_print_string("\n****************\n");
   }
   }
   return 0;
  }
