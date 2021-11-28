#include "./mcc_generated_files/mcc.h"

#include "i2c.h"
#include "lcd_lib.h"

static void lcd_cmd(Lcd *p, uint8_t cmd){
    i2c_start();
    i2c_write(p->lcd_address << 1);  
    i2c_write(cmd);
    i2c_stop();    
    __delay_us(10);
}

/**
 * LCDを初期化する。
 * @param p
 */
void lcd_init(Lcd *p){
    __delay_ms(400);
    
    write4bits(p, 0x30);
    __delay_ms(5);
    
    write4bits(p, 0x30);
    __delay_ms(5);
    
    write4bits(p, 0x30);
    __delay_us(300);
         
    write4bits(p, 0x20);
    __delay_us(10);
    
    //command(0x28)
    write4bits(p, 0x20);
    
    __delay_us(100);
    
    write4bits(p, 0x80);
    
    
    //display
    //command(0x0C)
    write4bits(p, 0x00);
    
    __delay_us(100);
    
    write4bits(p, 0xC0);
    
    //clear
    //command(0x01);
    write4bits(p, 0x00);
    
    __delay_us(100);
    
    write4bits(p, 0x10);
    
    __delay_ms(5); //takes a long time
    
    //command(0x06);
    write4bits(p, 0x00);
    
    __delay_us(100);
    
    write4bits(p, 0x60);
    
    //home()
    //command(0x02)
    write4bits(p, 0x00);
    
    __delay_us(100);
    
    write4bits(p, 0x20);
    
    __delay_ms(3);
}

/**
 * バックライトをONにする。
 * @param p
 */
void lcd_backlight(Lcd *p){
    lcd_cmd(p, 0x08);
    p->_backlightval = 0x08;
    __delay_us(10);
}

/**
 * LCDの文字を削除して、カーソルポジションを0,0にセットする。
 * @param p
 */
void lcd_clear(Lcd *p){
    for(uint8_t row = 0; row < (p->ROW); row++){
        for(uint8_t col = 0; col < (p->COL); col++){
            lcd_set_cursor(p, col, row);
            lcd_print(p, " ");
        }
    }
    
    lcd_set_cursor(p, 0, 0);
}

/**
 * カーソルポジションをセットする。
 * @param col 列数
 * @param row 行数
 */
void lcd_set_cursor(Lcd *p, uint8_t col, uint8_t row){
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if(row > p->ROW){
        row = p->ROW - 1;
    }    
    uint8_t cmd = 0x80 | (col + row_offsets[row]);    
    command(p, cmd, 0);
}

/**
 * LCDに文字を出力する
 * @param p
 * @param str
 */
void lcd_print(Lcd *p, char *str) {
    while (*str) {
        uint8_t cmd = *str++;
        command(p, cmd, Rs);     
    }
}

/**
 * カーソルポジションを設定してLCDに文字を出力する。
 * @param p
 * @param str 出力する文字列
 * @param col 表示させる列数の開始位置
 * @param row 表示させる行数の開始位置
 */
void lcd_print_with(Lcd *p, char *str, uint8_t col, uint8_t row){
    lcd_set_cursor(p, col, row);
    lcd_print(p, str);
}

static void command(Lcd *p, uint8_t val, uint8_t mode){    
    uint8_t highnib = (val & 0xf0) | p->_backlightval | mode;
	uint8_t lownib = ((val << 4) & 0xf0) | p->_backlightval | mode;
    
	lcd_cmd(p, highnib);
    lcd_cmd(p, highnib | En);
    lcd_cmd(p, highnib & ~En);
    
    __delay_us(100);
        
	lcd_cmd(p, lownib);
    lcd_cmd(p, lownib | En);
    lcd_cmd(p, lownib & ~En);  
}

static void write4bits(Lcd *p, uint8_t val){
    lcd_cmd(p, val);        
    lcd_cmd(p, val | En);        
    lcd_cmd(p, val);
}