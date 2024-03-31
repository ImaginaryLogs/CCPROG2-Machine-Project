#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int
main(){
    struct TimeHM Time = GetTimeHmToday();
    printf("Time: %02d %02d\n", Time.hour, Time.minute);
    return 0;
}