
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "DS1307.h"

void i2c_init() {
    PRR0 &= ~(1 << PRTWI0);   //Datasheet says power reduction bit must be 0
    TWSR0 = 0x00;    // Prescaler = 1
    TWBR0 = 72;      // SCL = 16MHz/(16 + 2*72) = 100kHz
    TWCR0 |= (1 << TWEN); // Enable TWI
}



void i2c_start() {
    TWCR0 = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR0 & (1 << TWINT)));
}

void i2c_stop() {
    TWCR0 = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(uint8_t data) {
    TWDR0 = data;
    TWCR0 = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR0 & (1 << TWINT)));
}

void ds1307_set_time(uint8_t sec, uint8_t min, uint8_t hour) {
    i2c_start();
    i2c_write(0xD0);         // DS1307 address + write
    i2c_write(0x00);         // Start at register 00h
    
    i2c_write((sec % 10) | ((sec / 10) << 4));  // Seconds
    i2c_write((min % 10) | ((min / 10) << 4));  // Minutes
    i2c_write((hour % 10) | ((hour / 10) << 4)); // Hours (24h mode)
    i2c_stop();
}

void ds1307_get_time(uint8_t *time_array) {
    i2c_start();
    i2c_write(0xD0);         // DS1307 address + write
    i2c_write(0x00);         // Start at register 00h
    i2c_stop();
    
    i2c_start();
    i2c_write(0xD1);         // DS1307 address + read
    
    for(uint8_t i=0; i<3; i++) {
        TWCR0 = (1 << TWINT) | (1 << TWEN) | (i<2 ? (1 << TWEA) : 0);
        while (!(TWCR0 & (1 << TWINT)));
        time_array[i] = TWDR0;
    }
    i2c_stop();
}

// Square wave control
void ds1307_set_square_wave(SquareWaveFrequency freq) {
    i2c_start();
    i2c_write(DS1307_WRITE_ADDR);
    i2c_write(REG_CONTROL);
    i2c_write(SQWE_BIT | freq);
    i2c_stop();
}

// Oscillator control
void ds1307_enable_oscillator(uint8_t enable) {
    i2c_start();
    i2c_write(DS1307_WRITE_ADDR);
    i2c_write(REG_SECONDS);
    
    uint8_t seconds = enable ? 0x00 : 0x80;
    i2c_write(seconds);
    i2c_stop();
}

// BCD conversion
uint8_t ds1307_bcd_to_dec(uint8_t bcd) {
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

uint8_t ds1307_dec_to_bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

