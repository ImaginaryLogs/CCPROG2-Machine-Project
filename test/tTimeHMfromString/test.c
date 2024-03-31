#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
#include <stdio.h>

int main() {
    char timeStr[] = "6:26";

    struct TimeHM time = TimeHMfromString(timeStr);

    printf("%02d:%02d", time.hour, time.minute);
}