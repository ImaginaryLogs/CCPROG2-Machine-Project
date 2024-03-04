#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    Bus16 BusTrip;
    int keys[16] = {0};
    int size = 16;
    String15 name = "Vegapunk";
    printf("%d", tripFileGetDetail(&exampleDate, BusTrip, name, keys, size));
    return 0;
}
