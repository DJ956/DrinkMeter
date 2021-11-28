#include <xc.h>
#include <stdint.h>

//DIVIDE value
#define DIVIDE_VAL 493

/**
 * ロードセルを制御する構造体
 */
typedef struct {
    /**
     * CLK信号のピン
     */
    uint8_t cell_clk;
    /**
     * DAT信号のピン
     */
    uint8_t cell_dat;
    /**
     * 0グラム値のロードセルの値
     */
    unsigned long weight_zero;
    /**
     * 現在のロードセルの値
     */
    unsigned long weight_dat;
    /**
     * グラム数
     */
    uint16_t gram;
} LoadCell;


/**
 * LoadCell clock pin set low
 */
static void set_CELL_CLK_LOW(LoadCell *p);

/**
 * LoadCell clock pin set high
 */
static void set_CELL_CLK_HIGH(LoadCell *p);


static int get_CELL_DAT_VAL(LoadCell *p);

/**
 * get scale value
 * @return 
 */
unsigned long get_scale_val(LoadCell *p, uint8_t scale_avg_count);

/**
 * scale value to gram value
 */
void scale_convert_gram(LoadCell *p);