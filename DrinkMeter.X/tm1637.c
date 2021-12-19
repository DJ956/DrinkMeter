
#include "tm1637.h"
#include <xc.h>
#include "mcc_generated_files/mcc.h"

static uint8_t m_brightness;

static uint8_t segdata[] = { 
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

/** CLK_PIN set output mode*/
static void CLK_SET_OUTPUT(TM1637 *p){ TRISB &= ~p->clk_pin; }

/** CLK_PIN set high */
static void CLK_SET_HIGH(TM1637 *p){ LATB |=  p->clk_pin; }
/** CLK_PIN set low */
static void CLK_SET_LOW(TM1637 *p) { LATB &= ~p->clk_pin; }

/** DIO_PIN set output mode */
static void DIO_SET_OUTPUT(TM1637 *p){ TRISB &= ~p->dat_pin; }
/** DIO_PIN set input mode */
static void DIO_SET_INPUT(TM1637 *p){ TRISB |= p->dat_pin; }
/** DIO_PIN set high */
static void DIO_SET_HIGH(TM1637 *p){ LATB |= p->dat_pin; }
/** DIO_PIN set low*/
static void DIO_SET_LOW(TM1637 *p){ LATB &= ~p->dat_pin; }


static void start_segment(TM1637 *p){    
    //DIO_PIN, CLK_PIN set output
    DIO_SET_OUTPUT(p);
    CLK_SET_OUTPUT(p);
    
    //DIO_PIN, CLK_PIN set high
    DIO_SET_HIGH(p);
    CLK_SET_HIGH(p);
    
    //DIO_PIN, CLK_PIN set low
    DIO_SET_LOW(p);
    CLK_SET_LOW(p);
}

static void stop_segment(TM1637 *p){
    //DIO_PIN, CLK_PIN set output
    DIO_SET_OUTPUT(p);
    CLK_SET_OUTPUT(p);
    
    //DIO_PIN, CLK_PIN set low
    CLK_SET_LOW(p);
    DIO_SET_LOW(p);
    
    //DIO_PIN, CLK_PIN set high
    CLK_SET_HIGH(p);
    DIO_SET_HIGH(p);
}

static uint8_t get_DIO_PIN_value(TM1637 *p){
    if(p->dat_pin == 0x01){ return PORTBbits.RB0; }
    if(p->dat_pin == 0x02){ return PORTBbits.RB1; }
    if(p->dat_pin == 0x04){ return PORTBbits.RB2; }
    if(p->dat_pin == 0x08){ return PORTBbits.RB3; }
    if(p->dat_pin == 0x10){ return PORTBbits.RB4; }
    if(p->dat_pin == 0x20){ return PORTBbits.RB5; }
    if(p->dat_pin == 0x40){ return PORTBbits.RB6; }
    if(p->dat_pin == 0x80){ return PORTBbits.RB7; }
}

static uint8_t write_byte(TM1637 *p, uint8_t b){
    uint8_t data = b;
    
    //CLK_PIN set output
    CLK_SET_OUTPUT(p);
    for(uint8_t i = 0; i < 8; i++){      
        //CLK_PIN set low
        CLK_SET_LOW(p);        
        
        if(data & 0x01){
            //DIO_PIN set high
            DIO_SET_HIGH(p);
        }else{
            //DIO_PIN set low     
            DIO_SET_LOW(p);
        }
        
        //CLK_PIN set high
        CLK_SET_HIGH(p);
        
        data = data >> 1;
    }
    
    //wait ACK
    //CLK_PIN set low
    //DIO_PIN set high
    CLK_SET_LOW(p);
    DIO_SET_HIGH(p);
    
    //CLK_PIN set high     
    CLK_SET_HIGH(p);
    
    //DIO_PIN set input
    DIO_SET_INPUT(p);
    
    uint8_t ack = get_DIO_PIN_value(p);
    if(ack == 0){
        //DIO_PIN set output
        DIO_SET_OUTPUT(p);
        
        //DIO_PIN set low
        DIO_SET_LOW(p);
    }
    
    //DIO_PIN set output
    DIO_SET_OUTPUT(p);
    
    return ack;
}

/**
 * 
 * @param brightness
 * @param on 0:light off, 1:light on
 */
static void set_brigthness(TM1637 *p, uint8_t brightness, uint8_t on){
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
static void set_segments(TM1637 *p, const uint8_t segments[], uint8_t length, uint8_t pos){
    //write COMM1
    start_segment(p);
    write_byte(p, COMM1);
    stop_segment(p);
    
    //write COMM2 + first digit address
    start_segment(p);
    write_byte(p, COMM2 + (pos & 0x03));
    
    //write the data bytes
    for(uint8_t k = 0; k < length; k++){
        write_byte(p, segments[k]);        
    }
    
    stop_segment(p);
    
    //write COMM3 + brigthness
    start_segment(p);
    write_byte(p, COMM3 + (m_brightness & 0x0f));    
    stop_segment(p);
}

/**
 * TM1637 segment set to 00:00
 */
static void clear_segment(TM1637 *p){
    uint8_t data[] = { 0, 0, 0, 0 };
	set_segments(p, data, 4, 0);
}

/**
 * digit to TM1637 number
 * @param digit
 * @return 
 */
static uint8_t encode_digit(TM1637 *p, uint8_t digit){
    return segdata[digit & 0x0f];
}

void initialize_digit(TM1637 *p){
    set_brigthness(p, 0x0f, 1);
    print_digit(p, 0);
}

void print_digit(TM1637 *p, uint8_t number){    
    uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
    
    //convert
    uint8_t tmp = number;
    data[0] = encode_digit(p, tmp / 1000);
    tmp %= 1000;
    data[1] = encode_digit(p, tmp / 100);
    tmp %= 100;
    data[2] = encode_digit(p, tmp / 10);
    tmp %= 10;
    data[3] = encode_digit(p, tmp);
    
    set_segments(p, data, 4, 0);
}