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
    
    //RB1 cell clock is output
    //RB0 cell data is input
    TRISB = 0x01;
    //init LATA
    LATA = 0x00;
    LATB = 0x00;
    
    //init i2c
    SSPADD = 0x13;    
    SSPCON1 = 0x28;
    SSPCON2 = 0x0;
    SSPSTAT = 0;
    
    LoadCell loadcell = {0x01, 0x00,0, 0, 500};
    Lcd lcd = {0x27, 16, 2, 0};
    
    DrinkMeter drink = {&loadcell, &lcd, 500, 0};
    
    initialize(&drink);
    
    while(1){
        calc_percentage(&drink);
        print_gram(&drink);
        
        __delay_ms(500);
    }
}