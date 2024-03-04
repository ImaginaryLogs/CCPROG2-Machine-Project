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

    strcpy(input->tripNumber, "AE101");
    strcpy(input->embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
    strcpy(input->passengerName, "Juan A. Dela Cruz");
    input->idNumber = 12345637;
    input->priorityNumber = 0;
    input->dateOfTrip = exampleDate;
    input->timeOfTrip = exampleTime;
    strcpy(input->dropOffPoint, "2nd drop-off point – Phase 5, San Jose Village");
}

void
printPassenger(struct Passenger *Passenger){
    printf("%s\n", Passenger->tripNumber);
    printf("%s\n", Passenger->embarkationPoint);
    printf("%s\n", Passenger->passengerName);
    printf("%u", Passenger->idNumber);
    printf("%u", Passenger->priorityNumber);
    printf("%d %d %d", Passenger->dateOfTrip.year, Passenger->dateOfTrip.month, Passenger->dateOfTrip.day);
    printf("%d %d", Passenger->timeOfTrip.hour, Passenger->timeOfTrip.minute);
    printf("%s", Passenger->dropOffPoint);
}

int 
main(){
    struct Passenger Juan;
    initializePassenger(&Juan);
    struct DateDMY exampleDate;
    exampleDate.year = 2024;
    exampleDate.month = 4;
    exampleDate.day = 1;
    String15 dateApril = "";
    StringfromDateDMY(dateApril, &exampleDate, FALSE);
    printf("April: %s\n", dateApril);
    printf("%d\n", tripFilePassengerAppender(exampleDate, &Juan));
    printPassenger(&Juan);
}
