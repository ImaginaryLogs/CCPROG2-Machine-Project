#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct DateDMY input;

    repeatGetDateDMY(&input);
    printf("Date: %d-%d-%d", input.day, input.month, input.year);
}