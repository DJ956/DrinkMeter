#pragma once

#include <xc.h>
#include <stdint.h>

#define COMM1 0x40 
#define COMM2 0xC0
#define COMM3 0x80

typedef struct{
    uint8_t clk_pin;
    uint8_t dat_pin;
} TM1637;


static void CLK_SET_OUTPUT(TM1637 *p);
static void CLK_SET_HIGH(TM1637 *p);
static void CLK_SET_LOW(TM1637 *p);

static void DIO_SET_OUTPUT(TM1637 *p);
static void DIO_SET_INPUT(TM1637 *p);
static void DIO_SET_HIGH(TM1637 *p);
static void DIO_SET_LOW(TM1637 *p);

static uint8_t get_DIO_PIN_value(TM1637 *p);
static void start_segment(TM1637 *p);
static void stop_segment(TM1637 *p);
static void set_brigthness(TM1637 *p, uint8_t brightness, uint8_t on);
static uint8_t write_byte(TM1637 *p, uint8_t b);
static void set_segments(TM1637 *p, const uint8_t segments[], uint8_t length, uint8_t pos);
static void clear_segment(TM1637 *p);
static uint8_t encode_digit(TM1637 *p, uint8_t digit);

/**
 * Initialize TM1637
 * @param p
 */
void initialize_digit(TM1637 *p);

/**
 * print to num to TM1637
 * @param p
 * @param number
 */
void print_digit(TM1637 *p, uint8_t number);