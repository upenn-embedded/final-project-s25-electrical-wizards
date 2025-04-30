#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stddef.h> 
#include <string.h>
#include <stdio.h>
#include "buffers.h"
#include "uart.h"


uint16_t adc_val;
int num_ms = 0;

char print_str[64];
void config_timer(){
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);
    
    // 2) Set compare value for 1?ms
    OCR1A = 249;  // 249 ? triggers when TCNT1 matches
    //Set prescaler to 64
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TIMSK1 &= ~(1<<OCIE1A);  // disable compare?match A interrupt
}
ISR(TIMER1_COMPA_vect) {
    num_ms++;
}

int computeMAX(int ms_lim){//conputes the max value over num_ms cycles
    
    num_ms = 0;
    TCNT1 = 0;     
    TIMSK1 |=  (1<<OCIE1A);//RE ENABLE INTERRUPTS
    
    int max = 0;
    while(num_ms < ms_lim){
        ADCSRA |= (1 << ADSC); // Start conversion
        while (ADCSRA & (1 << ADSC)); // Wait for completion
        adc_val = ADC;
        
        printf("ADC_VAL: %d\n", adc_val);
        if(adc_val > max){
            max = adc_val;
        }
    }
    TIMSK1 &= ~(1<<OCIE1A);//re disable interrupts
    return max;
}

