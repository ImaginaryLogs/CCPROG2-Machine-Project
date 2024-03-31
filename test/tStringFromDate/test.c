#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
#include <stdio.h>

int main(){
    struct DateDMY Date = {31, 03, 2024};
    char dateStr[] = "";

    StringfromDateDMY(dateStr, &Date, 1);
    printf("String: %s", dateStr);
}