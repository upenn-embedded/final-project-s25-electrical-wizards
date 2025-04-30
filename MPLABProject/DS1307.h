#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>

// I2C Addresses
#define DS1307_ADDRESS 0x68          // 7-bit address
#define DS1307_WRITE_ADDR 0xD0       // 8-bit write address
#define DS1307_READ_ADDR 0xD1        // 8-bit read address

// Register addresses
#define REG_SECONDS   0x00
#define REG_MINUTES   0x01
#define REG_HOURS     0x02
#define REG_DAY       0x03
#define REG_DATE      0x04
#define REG_MONTH     0x05
#define REG_YEAR      0x06
#define REG_CONTROL   0x07

// Control register bits
#define SQWE_BIT  (1 << 4)
#define RS0_BIT   (1 << 0)
#define RS1_BIT   (1 << 1)

typedef enum {
    SQW_1HZ    = 0x00,
    SQW_4KHZ   = RS0_BIT,
    SQW_8KHZ   = RS1_BIT,
    SQW_32KHZ  = RS0_BIT | RS1_BIT
} SquareWaveFrequency;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} DS1307_Time;

// Core I2C functions
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);

// DS1307 functions
void ds1307_set_time(uint8_t sec, uint8_t min, uint8_t hour);
void ds1307_get_time(uint8_t *time_array);
void ds1307_set_square_wave(SquareWaveFrequency freq);
void ds1307_enable_oscillator(uint8_t enable);

// BCD conversion helpers
uint8_t ds1307_bcd_to_dec(uint8_t bcd);
uint8_t ds1307_dec_to_bcd(uint8_t dec);

#endif // DS1307_H