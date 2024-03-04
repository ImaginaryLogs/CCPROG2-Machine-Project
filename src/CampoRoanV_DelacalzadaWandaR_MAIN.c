#include "CampoRoanV_DelacalzadaWandaR_Functions.h"

/**
 *
 * * TODO Fill it functions accessible to a general user based on the diagram.
 * 
 */
int 
main()
{
    char mainInput;
    printGraphics("BusExpress");
    do {
        repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
        printf("\n");
        system("cls");
        switch (mainInput) {
            case 'a':
                menuAdmin();
                break;
            case 'b':
                menuPassenger();
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
