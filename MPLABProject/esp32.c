#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stddef.h> 
#include <string.h>
#include <stdio.h>

#include "esp32.h"

void SPI_SlaveInit(void)
{
/* Set MISO output, all others input */
    DDRC = (1<<DDC1);
/* Enable SPI */
    SPCR1 = (1<<SPE);
}
char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR1 & (1<<SPIF)));
    /* Return Data Register */
    return SPDR1;
}