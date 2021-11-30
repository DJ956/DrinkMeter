#include "i2c.h"
#include "loadcell.h"
#include "lcd_lib.h"
#include "drink_meter.h"
#include "mcc_generated_files/mcc.h"


void main(void)
{
    
    SYSTEM_Initialize();
    
    
   //set digital mode
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    //RA0 cell clock is output
    //RA1 cell data is input
    TRISA = 0x02;
    //init LATA
    LATA = 0x00;
    LATB = 0x00;
    
    //init i2c
    SSPADD = 0x13;    
    SSPCON1 = 0x28;
    SSPCON2 = 0x0;
    SSPSTAT = 0;
    
    /*
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    
    clear_segment();
    
    data[0] = encode_digit(1);
    data[1] = encode_digit(9);
    data[2] = encode_digit(8);
    data[3] = encode_digit(4);
    
    
    set_brigthness(0x0f, 1);
    set_segments(data, 4, 0);
     */
}