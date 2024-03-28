#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"


int main() {
    struct DateDMY date;
    struct dropOffPointList exits[4];
    struct Bus16 TripDatabase[22];

    date.year = 2024;
    date.month = 4;
    date.day = 1;

    initializeDropOffPointList(exits);
    initializeBusTrip(TripDatabase, TOTAL_TRIPS, &date, FALSE, exits);

    tripFile_GetBusTrip(&date, TripDatabase, exits);

    return 0;
}
