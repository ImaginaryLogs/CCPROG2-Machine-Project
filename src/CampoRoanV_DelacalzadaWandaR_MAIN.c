#include "CampoRoanV_DelacalzadaWandaR_Functions.h"

/**
 *
 * TODO Fill it functions accessible to a general user based on the diagram.
 * 
 */
int 
main()
{
    char mainInput;
    struct Bus16 Triplist[TOTAL_TRIPS];
    struct TimeHM timeNow;
    struct DateDMY dateNow;
    printGraphics("BusExpress");

    repeatGetDateDMY(&dateNow);
    
    do {
        repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
        printf("\n");
        system("cls");
        switch (mainInput) {
            case 'a':
                menuAdmin(Triplist, TOTAL_TRIPS, &dateNow);
                break;
            case 'b':
                menuPassenger(Triplist, TOTAL_TRIPS, &dateNow);
                break;
            case 'c':
                
                printSingleColorText(FG_RED, "[x] Terminating...\n\tPress any button again.");
                getchar();
                break;
            default: // loop back to main menu
                printErrorMessage("Invalid command, please select again.\n");
                break;
        }
    } while (mainInput != 'c');
    return 0;
}
