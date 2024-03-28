#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct Passenger input;
    struct dropOffPointList exits[4];
    initializeDropOffPointList(exits);
    struct Bus16 Triplist[TOTAL_TRIPS]	;
    repeatGetPassenger(&input, Triplist, exits);
    printPassenger(&input);	
    return 0;
}
