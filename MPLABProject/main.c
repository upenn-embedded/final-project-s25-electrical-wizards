/*
 * File:   main.c
 * Author: samgr
 *
 * Created on April 11, 2025, 10:24 AM
 */

#define F_CPU 16000000UL
#define USART_BAUD_RATE      9600
#define USART_BAUD_PRESCALER (((F_CPU / (USART_BAUD_RATE * 16UL))) - 1)

#define CHOMP_THRESH 60
#define BIG_CHOMP_THRESH 160

#include <xc.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "uart.h"
#include "DS1307.h"
#include "LCD_GFX.h"
#include "ST7735.h"
#include "buffers.h"
#include "esp32.h"

#define BLACK       0x0000  
#define WHITE       0xFFFF  
#define RED         0xF800  
#define GREEN       0x07E0  
#define BLUE        0x001F  
#define YELLOW      0xFFE0  

#define GRAPH_WIDTH 128
#define GRAPH_HEIGHT 72
#define X_OFFSET 10
#define Y_BASE 100

#define SAMPLE_MS 10
#define THRESHOLD 100

int graphX = 0;
int prevAvg = Y_BASE;


uint8_t curr_time[3] = {0,0,0};
uint8_t bedtime_start[3] = {0,59,20};  //30 seconds past 12:00 AM
uint8_t bedtime_end[3] = {0,4,21};    // 30 seconds past 12:05 AM

char c;
char line_buf[MAX_STRING_LENGTH];
uint8_t buf_idx = 0;


typedef struct {
    uint8_t bed_time[3];
    uint32_t data[GRAPH_WIDTH];
    uint16_t sleep_duration;       //In minutes
} Graph;

volatile uint32_t bucket_ms = 0;           
volatile uint16_t bucket_event_count = 0;
volatile uint32_t bucket_elapsed_ms = 0;
volatile uint8_t  bucket_index = 0;
volatile uint8_t  last_above = 0;
//initialize read buffer and calculation buffer

void initADC(){
       // Use internal 2.56V reference (check datasheet!)
    ADMUX |= (1 << REFS0);
    ADMUX |= (1 << REFS1);
    
    // Select ADC0 (PC0)
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0)); // Clear MUX bits
    //ADMUX |= (1 << MUX1); // 0b0010 for ADC2
    

    // ADC Auto Trigger Source: Off (manual)
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
    
    // ADC Clock Prescaler = 128 ? 125kHz ADC clock
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

    ADCSRA |= (1 << ADEN);  // Enable ADC
}

void config_timer0_10ms(void) {
    // CTC: OCR0A ticks every SAMPLE_MS
    TCCR0A = (1<<WGM01);
    OCR0A  = (uint8_t)((F_CPU/64/1000) * SAMPLE_MS - 1);
    TIMSK0 = (1<<OCIE0A);
    TCCR0B = (1<<CS01)|(1<<CS00);  // prescaler 64
}

void enable_streaming(){
    ADCSRA |= (1 << 5);//enable free running mode
    ADCSRA |= (1 << ADSC); //turn on conversion
}
void disable_streaming(){
    ADCSRA &= ~(1 << 5);//disable free running mode
    ADCSRA &= ~(1 << ADSC); //initally make sure conversion off 
}

void init(){
    // Configure PD4 as output for LED
    DDRD |= (1 << PD4);
    PORTD &= ~(1 << PD4);  // LED off initially
    uart_init();
    initADC();
    i2c_init();
    lcd_init();
    LCD_setScreen(BLACK);
}

void init_graph(Graph* graph, uint16_t sleep_duration) {
    //Initialize graph struct
    graph->sleep_duration = sleep_duration;
    //ds1307_get_time(graph->bed_time);
    memset(graph->data, 0, GRAPH_WIDTH);
    
    
    uint32_t total_ms = (uint32_t)sleep_duration * 60 * 1000;
    bucket_ms = total_ms / GRAPH_WIDTH;


    //Draw x and y axes
    LCD_drawLine(X_OFFSET, Y_BASE, X_OFFSET + GRAPH_WIDTH, Y_BASE, WHITE);
    LCD_drawLine(X_OFFSET, Y_BASE, X_OFFSET, Y_BASE - GRAPH_HEIGHT, WHITE);
    
    //Draw arrows on axes
    LCD_drawLine(X_OFFSET + GRAPH_WIDTH, Y_BASE, X_OFFSET + GRAPH_WIDTH - 4, Y_BASE - 3, WHITE);
    LCD_drawLine(X_OFFSET + GRAPH_WIDTH, Y_BASE, X_OFFSET + GRAPH_WIDTH - 4, Y_BASE + 3, WHITE);
    
    LCD_drawLine(X_OFFSET, Y_BASE - GRAPH_HEIGHT, X_OFFSET - 3, Y_BASE - GRAPH_HEIGHT + 4, WHITE);
    LCD_drawLine(X_OFFSET, Y_BASE - GRAPH_HEIGHT, X_OFFSET + 3, Y_BASE - GRAPH_HEIGHT + 4, WHITE); 

    
    //Draw x tick marks and labels
    char unit = 'h';
    uint16_t divisor = 3600; // default: seconds per hour
    if (sleep_duration < 2) { // Less than 2 minutes ? use seconds
        unit = 's';
        divisor = 1;
    } else if (sleep_duration < 120) { // Less than 2 hours ? use minutes
        unit = 'm';
        divisor = 60;
    }
//
    // Total graph span in seconds
    uint32_t total_seconds = sleep_duration * 60;
    uint8_t num_ticks = 4;

    for (uint8_t i = 0; i <= num_ticks; i++) {
        uint8_t x = X_OFFSET + (GRAPH_WIDTH * i / num_ticks);
        LCD_drawLine(x, Y_BASE - 2, x, Y_BASE + 2, WHITE);

        uint32_t seconds_at_tick = (total_seconds * i) / num_ticks;
        uint16_t value = seconds_at_tick / divisor;

        char label[6];
        snprintf(label, sizeof(label), "%u%c", value, unit);
        LCD_drawString(x - 6, Y_BASE + 6, label, WHITE, BLACK);
    }
}

//Maps the current elapsed time to which column on the graph a new
//data point should be placed
uint8_t get_current_column(Graph* graph) {
    
    //Find current time in seconds
    uint8_t time[3];
    ds1307_get_time(time);
    
    uint8_t sec = (time[0] & 0x0F) + ((time[0] >> 4) * 10);
    uint8_t min = (time[1] & 0x0F) + ((time[1] >> 4) * 10);
    uint8_t hour = (time[2] & 0x0F) + ((time[2] >> 4) * 10);
    
    
    uint32_t current_time_seconds = (hour * 3600) + (min * 60) + sec;
    

    //Find start time in seconds
    sec = (graph->bed_time[0] & 0x0F) + ((graph->bed_time[0] >> 4) * 10);
    min = (graph->bed_time[1] & 0x0F) + ((graph->bed_time[1] >> 4) * 10);
    hour = (graph->bed_time[2] & 0x0F) + ((graph->bed_time[2] >> 4) * 10);
    
    uint32_t start_time_seconds = (hour * 3600) + (min * 60) + sec;
    
    //Get the total seconds on the graph
    int total_seconds = graph->sleep_duration * 60;
    //Get the elapsed seconds
    int elapsed_seconds = 0;
    if (current_time_seconds < start_time_seconds) {
        elapsed_seconds = (86400 - start_time_seconds) + current_time_seconds;
    } else {
        elapsed_seconds = current_time_seconds - start_time_seconds;
    }
    
    //Return the column of the graph for the next data point
    //It's proportional to the elapsed seconds/total_seconds
    return (elapsed_seconds * GRAPH_WIDTH)/total_seconds;
}


void decode_bcd_time(uint8_t* time_arr) {
    for (int i = 0; i < 3; i++) {
        time_arr[i] = (time_arr[i] & 0x0F) + ((time_arr[i] >> 4) * 10);
    }
}

bool is_between_times(uint8_t* curr, uint8_t* start, uint8_t* end) {
    uint32_t curr_sec  = curr[2] * 3600 + curr[1] * 60 + curr[0];
    uint32_t start_sec = start[2] * 3600 + start[1] * 60 + start[0];
    uint32_t end_sec   = end[2] * 3600 + end[1] * 60 + end[0];
    

    if (end_sec >= start_sec) {
        return (curr_sec >= start_sec && curr_sec <= end_sec);
    } else {
        return (curr_sec >= start_sec || curr_sec <= end_sec);
    }
}

uint16_t get_sleep_duration_minutes(uint8_t* start, uint8_t* end) {
    uint32_t start_sec = start[2] * 3600 + start[1] * 60 + start[0];
    uint32_t end_sec   = end[2] * 3600 + end[1] * 60 + end[0];

    if (end_sec < start_sec) {
        end_sec += 86400;
    }

    return (end_sec - start_sec) / 60;
}

void parse_bedtime_string(const char* str, uint8_t* bedtime_start, uint8_t* bedtime_end) {
    if (strlen(str) != 4) return;

    char hh[3] = { str[0], str[1], '\0' };
    char mm[3] = { str[2], str[3], '\0' };

    uint8_t hour = (uint8_t)atoi(hh);
    uint8_t min  = (uint8_t)atoi(mm);
    uint8_t sec  = 0;

    // Set bedtime_start
    bedtime_start[0] = sec;
    bedtime_start[1] = min;
    bedtime_start[2] = hour;

    // Add 5 minutes to get bedtime_end
    min += 5;
    if (min >= 60) {
        min -= 60;
        hour = (hour + 1) % 24;
    }

    bedtime_end[0] = sec;
    bedtime_end[1] = min;
    bedtime_end[2] = hour;
}


void plot_point(Graph* graph, int x) {
    int y = Y_BASE - graph->data[x];
    LCD_drawPixel(X_OFFSET + x, y, WHITE);
}

void plot_graph(Graph* graph) {
    // Iterate through the data array
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        plot_point(graph,i);

    }
}



void drawMovingAverageTrendline(int *data, int currentIndex, int windowSize) {
    char str[64];

    if (currentIndex < windowSize) {
        return; // Not enough points yet
    }

    int sum = 0;
    for (int i = currentIndex - windowSize; i <= currentIndex; i++) {
        sum += data[i];
        //printf("sum: %d\n, sum);
    }
    int currentAvg = sum / windowSize;

    LCD_drawLine(
        X_OFFSET + currentIndex - windowSize, prevAvg,
        X_OFFSET + currentIndex,     currentAvg,
            WHITE
    );
    //printf("prevY: %d,    currY: %d\n", prevAvg, currentAvg);
    prevAvg = currentAvg;
}


int main(void) {
    Graph graph;
    init();
    char print_str[64];
    config_timer();
    sei();
    
    int max_val = 0;
    char c_received;
    
   
    //ds1307_set_time(30,26,15);    
    int sleep_duration = get_sleep_duration_minutes(bedtime_start, bedtime_end);
    init_graph(&graph, sleep_duration); 
    
    double collection_interval = (sleep_duration * 60)/GRAPH_WIDTH;
    
    double chomps = 0; // counts the number of bites 
    bool ready_for_plotting = false; // keeps track of the time difference between points
    double big_chomps = 0;
    double plot_timing_counter = 0;
    double plot_timing_thresh = collection_interval * 10; //set number of loops before plotting a point, this will have to be calculated
    
    int last_x = -1;
    
    while (graphX < GRAPH_WIDTH) {
        //compute adc reading
        ds1307_get_time(curr_time);
        decode_bcd_time(curr_time);
        //parse_bedtime_string(line_buf, bedtime_start, bedtime_end);

        if (!is_between_times(curr_time, bedtime_start, bedtime_end)) {
            
            printf("Currtime: H: %d, M: %d, S: %d\n\n", curr_time[2], curr_time[1], curr_time[0]) ;
            //printf("Bedtime: H: %d, M: %d, S: %d\n\n", bedtime_start[2], bedtime_start[1], bedtime_start[0]);
            _delay_ms(1000);
            continue;
        }
       
        
        max_val = computeMAX(10); 
        
        //uint16_t adc_val = ADC;
        
//        sprintf(print_str, "MaxVal:   %d\n\n", max_val);
//        printf("MaxVal: %d\n\n", max_val);
//        
        plot_timing_counter++;
        if(plot_timing_counter >= plot_timing_thresh){
            ready_for_plotting = true;
        }
        
        //update graph
        if(!ready_for_plotting){
            if(max_val > CHOMP_THRESH){
                if(max_val > BIG_CHOMP_THRESH){
                    big_chomps++;
                    chomps++;
                }
                else{
                    chomps++;
                }
            }
        }
        else{
            //draw line from current point to next point
            printf("CHOMPS:   %lf\nBIG Chomps %lf\n\n", chomps, big_chomps);
            int y = Y_BASE - (chomps * 2 + big_chomps * 3);
            //printf("y: %d\n", y);
            if (y < (Y_BASE - GRAPH_HEIGHT)) {
                y = Y_BASE - GRAPH_HEIGHT;
            }
            graph.data[graphX] = y;
            LCD_drawPixel(X_OFFSET + graphX, y, WHITE);
            
//            if (graphX > 0 && graphX % 5 == 0) {
//                drawMovingAverageTrendline(graph.data,graphX,5);
//            }
            graphX++;
            ready_for_plotting = false;
            plot_timing_counter = 0;
            chomps = 0;
            big_chomps = 0;
        }


        
        _delay_ms(100);
    }
    return 1;
}

ISR(USART0_RX_vect) {
    c = UDR0;
    printf("TESTING\n\n\n\n\n\n");
    if (c != '\n' && c != '\r') {
        if (buf_idx < MAX_STRING_LENGTH - 1)
            line_buf[buf_idx++] = c;
    }

    if (c == '\n') {
        line_buf[buf_idx] = '\0';
        printf("Received: %s\n", line_buf);
        buf_idx = 0;
    }
}
