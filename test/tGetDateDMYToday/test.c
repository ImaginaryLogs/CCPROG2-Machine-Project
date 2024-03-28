#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int
main(){
    
    struct DateDMY Date = GetDateToday();
    printf("Date: %04d %02d %02d\n", Date.year, Date.month, Date.day);
    return 0;
}