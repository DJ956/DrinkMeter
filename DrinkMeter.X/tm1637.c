
#include "tm1637.h"
#include <xc.h>
#include "mcc_generated_files/mcc.h"

//#define _XTAL_FREQ 16000000

#define COMM1 0x40 
#define COMM2 0xC0
#define COMM3 0x80

/** DATA PIN NUMBER */
#define DIO_PIN 0x04 //RA2
/** CLOCK PIN NUNBER */
#define CLK_PIN 0x02 //RA1

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

/** CLK_PIN set output mode*/
void CLK_SET_OUTPUT(){ 
    bit_delay();
    TRISA &= ~CLK_PIN;
    bit_delay();
}

/** CLK_PIN set input mode*/
void CLK_SET_INPUT(){
    bit_delay();
    TRISA |= CLK_PIN;
    bit_delay();
}

/** CLK_PIN set high */
void CLK_SET_HIGH(){ LATA |= CLK_PIN; }
/** CLK_PIN set low */
void CLK_SET_LOW() { LATA &= ~CLK_PIN; }

/** DIO_PIN set output mode */
void DIO_SET_OUTPUT(){
    bit_delay();
    TRISA &= ~DIO_PIN;
    bit_delay();
}
/** DIO_PIN set input mode */
void DIO_SET_INPUT(){
    bit_delay();
    TRISA |= DIO_PIN;
    bit_delay();
}
/** DIO_PIN set high */
void DIO_SET_HIGH(){ LATA |= DIO_PIN; }
/** DIO_PIN set low*/
void DIO_SET_LOW(){ LATA &= ~DIO_PIN; }

void start_segment(void){    
    //DIO_PIN, CLK_PIN set output
    DIO_SET_OUTPUT();
    CLK_SET_OUTPUT();
    
    //DIO_PIN, CLK_PIN set high
    DIO_SET_HIGH();
    CLK_SET_HIGH();
    
    //DIO_PIN, CLK_PIN set low
    DIO_SET_LOW();
    CLK_SET_LOW();
}

void stop_segment(void){
    //DIO_PIN, CLK_PIN set output
    DIO_SET_OUTPUT();
    CLK_SET_OUTPUT();
    
    //DIO_PIN, CLK_PIN set low
    CLK_SET_LOW();
    DIO_SET_LOW();
    
    //DIO_PIN, CLK_PIN set high
    CLK_SET_HIGH();
    DIO_SET_HIGH();
}

uint8_t get_DIO_PIN_value(){
    if(DIO_PIN == 0x01){ return PORTAbits.RA0; }
    if(DIO_PIN == 0x02){ return PORTAbits.RA1; }
    if(DIO_PIN == 0x04){ return PORTAbits.RA2; }
    if(DIO_PIN == 0x08){ return PORTAbits.RA3; }
    if(DIO_PIN == 0x10){ return PORTAbits.RA4; }
    if(DIO_PIN == 0x20){ return PORTAbits.RA5; }
    if(DIO_PIN == 0x40){ return PORTAbits.RA6; }
    if(DIO_PIN == 0x80){ return PORTAbits.RA7; }
}

uint8_t write_byte(uint8_t b){
    uint8_t data = b;
    
    //CLK_PIN set output
    CLK_SET_OUTPUT();
    for(uint8_t i = 0; i < 8; i++){      
        //CLK_PIN set low
        CLK_SET_LOW();        
        
        if(data & 0x01){
            //DIO_PIN set high
            DIO_SET_HIGH();
        }else{
            //DIO_PIN set low     
            DIO_SET_LOW();
        }
        
        //CLK_PIN set high
        CLK_SET_HIGH();
        
        data = data >> 1;
    }
    
    //wait ACK
    //CLK_PIN set low
    //DIO_PIN set high
    CLK_SET_LOW();
    DIO_SET_HIGH();
    
    //CLK_PIN set high     
    CLK_SET_HIGH();
    
    //DIO_PIN set input
    DIO_SET_INPUT();
    
    uint8_t ack = get_DIO_PIN_value();
    if(ack == 0){
        //DIO_PIN set output
        DIO_SET_OUTPUT();
        
        //DIO_PIN set low
        DIO_SET_LOW();
    }
    
    //DIO_PIN set output
    DIO_SET_OUTPUT();
    
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