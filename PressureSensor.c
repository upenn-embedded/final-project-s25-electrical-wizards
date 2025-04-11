/* 
 * File:   main.c
 * Author: michaelaizenberg
 *
 * Created on April 4, 2025, 12:29 PM
 * They said we can't use the pressure sensor but I wanted to save the code anyway
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <avr/io.h>
 #define F_CPU 16000000UL
 #include <util/delay.h>
 #include "uart.h"
 
 /*
  * 
  */
 void ADC_init() {
         ADMUX = (1 << REFS1) | (1 << REFS0);  
     ADCSRA = (1 << ADEN)  
            | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  
 }
 
 
 
 uint16_t ADC_read() {
     ADCSRA |= (1 << ADSC);  
     while (ADCSRA & (1 << ADSC));  
     return ADC; 
 }
 
 int main() {
     ADC_init();
     uart_init();
     while (1) {
         uint16_t adc_value = ADC_read();
         printf("%d\n", adc_value);
         _delay_ms(100);  
     }
 }
 