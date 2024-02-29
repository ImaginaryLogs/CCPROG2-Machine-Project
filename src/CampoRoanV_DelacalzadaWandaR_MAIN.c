#include "CampoRoanV_DelacalzadaWandaR_Functions.h"

void initializePassenger(struct Passenger *input){
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    struct TimeHM exampleTime;
    exampleTime.hour = 7;
    exampleTime.minute = 30;

    strcpy(input->tripNumber, "AE101");
    strcpy(input->embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
    strcpy(input->passengerName, "Juan A. Dela Cruz");
    input->idNumber = 12345637;
    input->priorityNumber = 0;
    input->dateOfTrip = exampleDate;
    input->timeOfTrip = exampleTime;
    strcpy(input->dropOffPoint, "2nd drop-off point – Phase 5, San Jose Village");
}

void test(){
    struct Passenger Juan;
    initializePassenger(&Juan);
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    String15 dateApril = "";
    StringfromDateDMY(dateApril, &exampleDate);
    printf("April: %s\n", dateApril);
    printf("%d\n", databaseBusAppender(exampleDate, Juan));
    String15 strHeader = "[!] Attention:";
    String63 bodyText = "Attetion.";
    printPopUpMessage(strHeader, FOREGROUND_BLUE, bodyText);
}

/**
 * TODO Fill it functions accessible to a general user based on the diagram.
 * 
 */
int 
main()
{
    test();
    
    // char mainInput;
    // printGraphics("Bus Express");
    // do {
    //     repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
    //     printf("\n");
    //     system("cls");
    //     switch (mainInput) {
    //         case 'a':
    //             menuAdmin();
    //             break;
    //         case 'b':
    //             menuPassenger();
    //             break;
    //         case 'c':
    //             printSingleColorText(FG_RED, "[x] Terminating...\n\tPress any button again.");
    //             getchar();
    //             break;
    //         default: // loop back to main menu
    //             printErrorMessage("Invalid command, please select again.\n");
    //             break;
    //     }
    // } while (mainInput != 'c');
    return 0;
}
