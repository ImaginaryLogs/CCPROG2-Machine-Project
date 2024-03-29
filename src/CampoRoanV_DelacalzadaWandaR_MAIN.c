
#include "CampoRoanV_DelacalzadaWandaR_Functions.h"


int 
main() {
    struct dropOffPointList exits[4];
    struct Bus16            Triplist[TOTAL_TRIPS];
    struct TimeHM           timeNow;
    struct DateDMY          dateNow;
    char mainInput;

    printGraphics("BusExpress");
    repeatGetDateDMY(&dateNow);
    initializeDropOffPointList(exits);
    initializeBusTrip(Triplist, &dateNow, TRUE, exits);

    do {
        printGraphics("BusExpress");
        printf("Date Today DD/MM/YYY: ");
        printDate(&dateNow);
        printf("\n");
        repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
        printf("\n");

        switch (mainInput) {
            case 'A':
            case 'a':
                menuAdmin(Triplist, &dateNow, exits);
                break;
            case 'B':
            case 'b':
                menuPassenger(Triplist, exits);
                break;
            case 'C':
            case 'c':
                tripFile_WriteBusTrip(&dateNow, Triplist);
                printSingleColorText(FG_RED, "[x] Saving State, Terminating...\n\tPress any button again.");
                getchar();
                break;
            default: // loop back to main menu
                printErrorMessage("Invalid command, please select again.");
                break;
        }

    } while (mainInput != 'c');
    return 0;
}
