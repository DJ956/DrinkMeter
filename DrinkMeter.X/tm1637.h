#include <xc.h>
#include <stdint.h>

// I2C functions for MSSP1 module
void bit_delay();
void start_segment();
void stop_segment();
void set_brigthness(uint8_t brightness, uint8_t on);
uint8_t write_byte(uint8_t b);
void set_segments(const uint8_t segments[], uint8_t length, uint8_t pos);
void clear_segment();
uint8_t encode_digit(uint8_t digit);