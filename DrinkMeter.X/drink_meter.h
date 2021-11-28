#include <xc.h>
#include "loadcell.h"

typedef struct{
    struct LoadCell *loadcell;
    struct Lcd *lcd;
    uint16_t max_gram;
}DrinkMeter;

void initialize(DrinkMeter *p); 

void print_gram(DrinkMeter *p);

void set_max_gram(DrinkMeter *p);