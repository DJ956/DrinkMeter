#include "i2c.h"
#include "loadcell.h"
#include "lcd_lib.h"
#include "tm1637.h"
#include "drink_meter.h"
#include "mcc_generated_files/mcc.h"

void main(void)
{
    SYSTEM_Initialize();
    
   //set digital mode
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    //RB1 cell clock is output
    //RB2 cell data is input
    TRISB = 0x04;
    
    //RC7, RC6, RC5, RC4, RC3 is input
    TRISC = 0xF8;
    
    //init LATA
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;
    
    //init i2c
    SSPADD = 0x13;    
    SSPCON1 = 0x28;
    SSPCON2 = 0x0;
    SSPSTAT = 0;
    
    //clk, data, -, -, max_gram
    LoadCell loadcell = {0x02, 0x04, 0, 0, 530};
    //add, col, row
    Lcd lcd = {0x27, 16, 2, 0};
    //clk, dat
    TM1637 tm1637 = {0x08, 0x10};
    
    DrinkMeter drink = {&loadcell, &lcd, &tm1637, 500, 30, 0};
    
    initialize(&drink);
    
    while(1){
        calc_percentage(&drink);
        print_gram(&drink);
        
        if(RC7 == 0){
            set_max_gram(&drink, 500, 30);
            __delay_ms(1000);
        }
        
        if(RC6 == 0){
            set_max_gram(&drink, 350, 15);
            __delay_ms(1000);
        }
        
        if(RC5 == 0){
            set_max_gram(&drink, 355, 35);
            __delay_ms(1000);
        }
    }
}