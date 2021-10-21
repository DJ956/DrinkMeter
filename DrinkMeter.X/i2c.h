#include <xc.h> // include processor files - each processor file is guarded.  

void i2c_wait();
void i2c_start();
void i2c_stop();
void i2c_repeated_start();
void i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);


