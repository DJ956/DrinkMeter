#pragma once

#include "loadcell.h"
#include "lcd_lib.h"

typedef struct{
    /**
     * ロードセルの構造体
     */
    LoadCell *loadcell;
    /**
     * LCDの構造体
     */
    Lcd *lcd;
    /**
     * 設定してい最大グラム
     */
    uint16_t max_gram;
    /**
     * 最大グラム値と、現在計測しているグラム値のパーセント
     */
    uint8_t percentage;
} DrinkMeter;

/**
 * LCDの初期化、ロードセルの0グラム時の計測を行う。
 * @param p
 */
void initialize(DrinkMeter *p); 

/**
 * 最大グラム値と、現在計測しているグラム値のパーセントを取得する。
 * @param p
 */
void calc_percentage(DrinkMeter *p);

/**
 * グラムをLCDと7セグに表示する。
 * @param p
 */
void print_gram(DrinkMeter *p);

/**
 * 最大グラム数を設定する。
 * @param p
 * @param max_gram 最大グラム数
 */
void set_max_gram(DrinkMeter *p, uint16_t max_gram);