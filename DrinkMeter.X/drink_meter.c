#include "drink_meter.h"
#include "lcd_lib.h"
#include <stdio.h>

void initialize(DrinkMeter *p){
    lcd_init(p->lcd);
    lcd_backlight(p->lcd);
    
    lcd_print_with(p->lcd, "Initialize", 0, 0);     
    lcd_print_with(p->lcd, "Load cell Unit", 0, 1);
    
    initialize_digit(p->tm1637);
    
    /** 0グラムを計測 */                                           
    p->loadcell->weight_zero = get_scale_val(p->loadcell, 50);
    
    char valstr[16];
    sprintf(valstr, "%lu val", p->loadcell->weight_dat);
    
    lcd_clear(p->lcd);
    lcd_print(p->lcd, valstr);
}

void calc_percentage(DrinkMeter *p){
    //今のグラムを計測する。
    p->loadcell->weight_dat = get_scale_val(p->loadcell, 16);
    scale_convert_gram(p->loadcell);
    
    //現在のグラムが0
    if(p->loadcell->gram == 0){
        p->percentage = 0;
    }else{
        //0以外の場合は計測
        p->percentage = ((float)p->loadcell->gram / (float)p->max_gram) * 100;
    }
}

void print_gram(DrinkMeter *p){
    char row1[16];
    char row2[16];
    
    sprintf(row1, "%dg / %dg", p->loadcell->gram, p->max_gram);
    sprintf(row2, "%d %%", p->percentage);
    
    lcd_clear(p->lcd);
    
    lcd_print_with(p->lcd, row1, 0, 0);
    lcd_print_with(p->lcd, row2, 0, 1);
    
    print_digit(p->tm1637, p->percentage);
}

void set_max_gram(DrinkMeter *p, uint16_t max_gram){
    p->max_gram = max_gram;
    calc_percentage(p);
}