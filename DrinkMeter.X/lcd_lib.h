#include <xc.h>
#include <stdint.h>
    
#define En 0b00000100
#define Rs 0b00000001

/**
 * LCDを制御する構造体
 */
typedef struct{
    /**
     * LCD_I2Cアドレス
     */
    uint8_t lcd_address;
    /**
     * 列数
     */
    uint8_t COL;
    /**
     * 行数
     */
    uint8_t ROW;
    /**
     * 内部で使用する値
     */
    uint8_t _backlightval;
} Lcd;

void lcd_init(Lcd *p);
static void lcd_backlight(Lcd *p);    
static void lcd_cmd(Lcd *p, uint8_t cmd);
void lcd_clear(Lcd *p);
void lcd_set_cursor(Lcd *p, uint8_t col, uint8_t row);
void lcd_print(Lcd *p, char *str);
void lcd_print_with(Lcd *p, char *str, uint8_t col, uint8_t row);

static void write4bits(Lcd *p, uint8_t val);

static void command(Lcd *p, uint8_t val, uint8_t mode);