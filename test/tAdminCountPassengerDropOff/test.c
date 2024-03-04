#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int
main(){
    struct DateDMY date = GetDateToday();
    adminCountPassengerDropOff(&date);
    return 0;
}