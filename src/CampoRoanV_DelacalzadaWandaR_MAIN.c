
#include "CampoRoanV_DelacalzadaWandaR_Functions.h"

/**
 *
 * TODO Fill it functions accessible to a general user based on the diagram.
 * 
 */
int 
main()
{
    struct dropOffPointList exits[4];
    char mainInput;
    struct Bus16 Triplist[TOTAL_TRIPS];
    struct TimeHM timeNow;
    struct DateDMY dateNow;
    printGraphics("BusExpress");
    repeatGetDateDMY(&dateNow);
    initializeDropOffPointList(exits);
    initializeBusTrip(Triplist, TOTAL_TRIPS, &dateNow, 1, exits);
    do {
        printGraphics("BusExpress");
        printf("Date Today YYYY/MM/DD: ");
        printDate(&dateNow);
        printf("\n");
        repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
        printf("\n");
        switch (mainInput) {
            case 'a':
                menuAdmin(Triplist, TOTAL_TRIPS, &dateNow, &exits);
                break;
            case 'b':
                menuPassenger(Triplist, &exits);
                break;
            case 'c':
                printSingleColorText(FG_RED, "[x] Terminating...\n\tPress any button again.");
                getchar();
                break;
            default: // loop back to main menu
                printErrorMessage("Invalid command, please select again.");
                break;
        }
    } while (mainInput != 'c');
    return 0;
}
