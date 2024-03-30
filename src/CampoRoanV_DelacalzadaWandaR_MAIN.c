
#include "CampoRoanV_DelacalzadaWandaR_Functions.h"



int 
main() {
    struct dropOffPointList exits[4];
    struct Bus16            Triplist[TOTAL_TRIPS];
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

/********************************************************************************************************* 
 * This is to certify that this project is our own work, based on our personal efforts in studying and 
 * applying the concepts learned. We have constructed the functions and their respective algorithms and 
 * corresponding code by ourselves. The program was run, tested, and debugged by our own efforts. 
 * 
 * We further certify that we have not copied in part or whole or otherwise plagiarized the work of 
 * other students and/or persons. 
 *                                                      ROAN CEDRIC V. CAMPO,           DLSU ID# 12305936
 *                                                      WANDA JUDE R. DE LA CALZADA,    DLSU ID# 12305669
*********************************************************************************************************/
