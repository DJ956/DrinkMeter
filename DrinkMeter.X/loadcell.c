#include "loadcell.h"
#include "./mcc_generated_files/mcc.h"

/**
 * LoadCell clock pin set low
 */
static void set_CELL_CLK_LOW(LoadCell *p){ LATA &= ~p->cell_clk; }

/**
 * LoadCell clock pin set high
 */
static void set_CELL_CLK_HIGH(LoadCell *p){ LATA |= p->cell_clk; }


static int get_CELL_DAT_VAL(LoadCell *p){
    if(p->cell_dat == 0x01){ return PORTAbits.RA0; }
    if(p->cell_dat == 0x02){ return PORTAbits.RA1; }
    if(p->cell_dat == 0x04){ return PORTAbits.RA2; }
    if(p->cell_dat == 0x08){ return PORTAbits.RA3; }
    if(p->cell_dat == 0x10){ return PORTAbits.RA4; }
    if(p->cell_dat == 0x20){ return PORTAbits.RA5; }
    if(p->cell_dat == 0x40){ return PORTAbits.RA6; }
    if(p->cell_dat == 0x80){ return PORTAbits.RA7; }
    
    return 0;
}

/**
 * ロードセルからの値を計測して返す。
 * @param p
 * @param scale_avg_count 計測する回数 回数が多ければ多いほど制度がよくなる。
 * @return ロードセルからの計測値
 */
unsigned long get_scale_val(LoadCell *p, uint8_t scale_avg_count){
    set_CELL_CLK_LOW(p);
    unsigned long weight_count = 0;
    unsigned  long weight_add = 0;
    for (uint8_t j = 0; j < scale_avg_count; j++)
    {
        while (get_CELL_DAT_VAL(p) == 0);
        while (get_CELL_DAT_VAL(p) == 1);
        __delay_us(10);
        
        uint8_t i;
        for (i = 0; i < 24; i++){ 
            weight_count <<= 1;
            set_CELL_CLK_HIGH(p);
            __delay_us(10);
            set_CELL_CLK_LOW(p);
            weight_count += get_CELL_DAT_VAL(p);
            __delay_us(10);
        }
        
        for (i = 0; i < 1; i++){ //i=1:ChAGain128　2:ChBGain32　3:ChAGain64
            set_CELL_CLK_HIGH(p);
            __delay_us(10);
            set_CELL_CLK_LOW(p);
            __delay_us(10);
        };
        weight_add += weight_count;
        weight_count = 0;
    }
    weight_count = weight_add / scale_avg_count;
    /** 最上位ビットをXORさせる */
    /** 24th bit flip*/
    weight_count = weight_count ^ 0x800000;
    return weight_count;
}

/**
 * ロードセルから計測した値をグラムに変換する。
 * 0グラム値のほうが大きい場合、0をグラムに設定する。
 * @param p
 */
void scale_convert_gram(LoadCell *p){
    /**0グラム値のほうが大きい場合、0をグラムに設定する。*/
    if(p->weight_dat < p->weight_zero){
        p->gram = 0;
        return;
    }
    
    float temp = p->weight_dat - p->weight_zero;
    temp = temp / DIVIDE_VAL;
    p->gram = (uint16_t)temp;
}