#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
void 
initializePassenger(struct Passenger *input){
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    struct TimeHM exampleTime;
    exampleTime.hour = 7;
    exampleTime.minute = 30;
    String63 location = "DLSU Manila Campus - South Gate Driveway";
    String63 name =  "Juan A. Dela Cruz";
    String127 dropOff = "2nd drop-off point - Phase 5, San Jose Village";
    strcpy(input->tripNumber, "AE101");
    strcpy(input->embarkationPoint, location);
    strcpy(input->passengerName, name);
    input->idNumber = 12345637;
    input->priorityNumber = 0;
    input->dateOfTrip = exampleDate;
    input->timeOfTrip = exampleTime;
    strcpy(input->dropOffPoint, dropOff);
}

void
printPassenger(struct Passenger *Passenger){
    printf("\nTrip Number:\t\t %s\n", Passenger->tripNumber);
    printf("Embarkation Point:\t %s\n", Passenger->embarkationPoint);
    printf("Passenger Name:\t\t %s\n", Passenger->passengerName);
    printf("ID Number:\t\t %u\n", Passenger->idNumber);
    printf("Priority Number:\t %u\n", Passenger->priorityNumber);
    printf("Date of Trip:\t\t %d %d %d\n", Passenger->dateOfTrip.year, Passenger->dateOfTrip.month, Passenger->dateOfTrip.day);
    printf("Time:\t\t\t %d:%d\n", Passenger->timeOfTrip.hour, Passenger->timeOfTrip.minute);
    printf("Drop off Point:\t\t %s\n\n", Passenger->dropOffPoint);
}

int 
main(){
    struct Passenger tempPassenger;
    initializePassenger(&tempPassenger);
    printPassenger(&tempPassenger);

    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;

    
    String15 dateApril = "";
    StringfromDateDMY(dateApril, &exampleDate, FALSE);
    printf("April: %s\n", dateApril);

    printPopUpMessage("[?] Check at Key = 0", FG_YELLOW, "Check.");
    printf("%d\n", tripFileGetCurrentPassenger(&exampleDate, &tempPassenger, 0));
    printPassenger(&tempPassenger);

    printPopUpMessage("[?] Check at Key = 1", FG_YELLOW, "Check.");
    printf("%d\n", tripFileGetCurrentPassenger(&exampleDate, &tempPassenger, 1));
    printPassenger(&tempPassenger);

    printPopUpMessage("[?] Check at Key = 2", FG_YELLOW, "Check.");
    printf("%d\n", tripFileGetCurrentPassenger(&exampleDate, &tempPassenger, 2));
    printPassenger(&tempPassenger);

    printPopUpMessage("[?] Check at Key = 3", FG_YELLOW, "Check.");
    printf("%d\n", tripFileGetCurrentPassenger(&exampleDate, &tempPassenger, 3));
    printPassenger(&tempPassenger);
    return 0;
}