#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    Bus16 BusTrip;
    printf("%d", tripFile_SearchSameTrip(&exampleDate, "AE101", BusTrip));
    return 0;
}