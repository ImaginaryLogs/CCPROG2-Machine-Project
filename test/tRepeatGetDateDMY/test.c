#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct DateDMY inputDate;
    inputDate.year = 0;
    inputDate.month = 0;
    inputDate.day = 0;
    repeatGetDateDMY(&inputDate);
    printf("%d %d %d\n", inputDate.year, inputDate.month, inputDate.day);
    return 0;
}