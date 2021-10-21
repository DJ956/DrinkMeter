
#include "tm1637.h"
#include <xc.h>

#define _XTAL_FREQ 16000000

#define COMM1 0x40 
#define COMM2 0xC0
#define COMM3 0x80

/** DATA PIN NUMBER */
#define DIO_PIN 0x01
/** CLOCK PIN NUNBER */
#define CLK_PIN 0x02

uint8_t m_brightness;

uint8_t segdata[] = { 
   0x3F, // 0
   0x06, // 1
   0x5B, // 2
   0x4F, // 3
   0x66, // 4
   0x6D, // 5
   0x7D, // 6
   0x07, // 7
   0x7F, // 8
   0x6F  // 9 
};

void bit_delay(void){
    //__delay_ms(10);
}

void start_segment(void){    
    //DIO_PIN, CLK_PIN set output
    TRISA &= ~DIO_PIN;
    TRISA &= ~CLK_PIN; 
    
    //DIO_PIN, CLK_PIN set high
    LATA |= DIO_PIN;
    LATA |= CLK_PIN;       
    
    //DIO_PIN, CLK_PIN set low
    LATA &= ~DIO_PIN;
    LATA &= ~CLK_PIN;
}

void stop_segment(void){
    //DIO_PIN, CLK_PIN set output
    TRISA &= ~DIO_PIN;
    TRISA &= ~CLK_PIN;
    
    //DIO_PIN, CLK_PIN set low
    LATA &= ~DIO_PIN;
    LATA &= ~CLK_PIN;    

    //DIO_PIN, CLK_PIN set high
    LATA |= DIO_PIN;
    LATA |= CLK_PIN;      
}

uint8_t get_DIO_PIN_value(){
    if(DIO_PIN == 0x00){ return PORTAbits.RA0; }
    if(DIO_PIN == 0x01){ return PORTAbits.RA1; }
    if(DIO_PIN == 0x02){ return PORTAbits.RA2; }
    if(DIO_PIN == 0x03){ return PORTAbits.RA3; }
    if(DIO_PIN == 0x04){ return PORTAbits.RA4; }
    if(DIO_PIN == 0x05){ return PORTAbits.RA5; }
    if(DIO_PIN == 0x06){ return PORTAbits.RA6; }
    if(DIO_PIN == 0x07){ return PORTAbits.RA7; }
}

uint8_t write_byte(uint8_t b){
    uint8_t data = b;
    
    //CLK_PIN set output
    TRISA &= ~CLK_PIN;
    for(uint8_t i = 0; i < 8; i++){      
        //CLK_PIN set low        
        LATA &= ~CLK_PIN;
        
        if(data & 0x01){
            //DIO_PIN set high
            LATA |= DIO_PIN;
        }else{
            //DIO_PIN set low
            LATA &= ~DIO_PIN;            
        }
        
        //CLK_PIN set high
        LATA |= CLK_PIN;
        
        data = data >> 1;
    }
    
    //wait ACK
    //CLK_PIN set low
    //DIO_PIN set high
    LATA &= CLK_PIN;
    LATA |= DIO_PIN;
    
    //CLK_PIN set high    
    LATA |= CLK_PIN;    
    
    //DIO_PIN set input
    TRISA |= DIO_PIN;
    bit_delay();
    
    uint8_t ack = get_DIO_PIN_value();
    if(ack == 0){
        //DIO_PIN set output
        TRISA &= ~DIO_PIN;
        
        //DIO_PIN set low
        LATA &= ~DIO_PIN;
    }
    
    bit_delay();
    //DIO_PIN set output
    TRISA &= ~DIO_PIN; 
    bit_delay();
    
    return ack;
}

/**
 * 
 * @param brightness
 * @param on 0:light off, 1:light on
 */
void set_brigthness(uint8_t brightness, uint8_t on){
    if(on == 1){
        m_brightness = (brightness & 0x7 | 0x08);
    }else{
        m_brightness = (brightness & 0x7 | 0x00);
    }
}

/**
 * set number to TM1637
 * @param segments
 * @param length segments array length
 * @param pos 
 */
void set_segments(const uint8_t segments[], uint8_t length, uint8_t pos){
    
    //write COMM1
    start_segment();
    write_byte(COMM1);
    stop_segment();
    
    //write COMM2 + first digit address
    start_segment();
    write_byte(COMM2 + (pos & 0x03));
    
    //write the data bytes
    for(uint8_t k = 0; k < length; k++){
        write_byte(segments[k]);        
    }
    
    stop_segment();
    
    //write COMM3 + brigthness
    start_segment();
    write_byte(COMM3 + (m_brightness & 0x0f));    
    stop_segment();
}

/**
 * TM1637 segment set to 00:00
 */
void clear_segment(){
    uint8_t data[] = { 0, 0, 0, 0 };
	set_segments(data, 4, 0);
}

/**
 * digit to TM1637 number
 * @param digit
 * @return 
 */
uint8_t encode_digit(uint8_t digit){
    return segdata[digit & 0x0f];
}