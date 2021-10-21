#include <xc.h>
#include "i2c.h"

void i2c_wait(){
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void i2c_start(){
    i2c_wait();
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
}

void i2c_stop(){
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
}

void i2c_repeated_start(){
    i2c_wait();
    SSPCON2bits.RSEN = 1;
    while(SSPCON2bits.RSEN);
}

void i2c_write(uint8_t data){
    SSPBUF = data;
    while(SSPSTATbits.BF);
    while(SSPCON2bits.ACKSTAT);
    i2c_wait();
}

uint8_t i2c_read(uint8_t ack){
    uint8_t data;
    
    SSPCON2bits.ACKDT = ack;
    SSPCON2bits.RCEN = 1;
    while(!SSPSTATbits.BF);
    SSPCON2bits.ACKEN = 1;
    data = SSPBUF;
    SSPCON1bits.SSPOV = 0;
    i2c_wait();
    
    return data;
}

