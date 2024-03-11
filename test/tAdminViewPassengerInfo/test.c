#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int
main(){
    struct DateDMY date;
    struct Bus16 Trips[22];

    date.day = 2;
    date.month = 4;
    date.year = 2024;
    
    initializeBusTrip(Trips, 22, &date);
    adminViewPassengerInfo(&date, Trips);
    return 0;
}