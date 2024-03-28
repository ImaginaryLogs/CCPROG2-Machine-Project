#include "utilsPass.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// |===| PASSENGER CMD SECTION |=====================|

void 
addPassengers(struct Bus16 TripDatabase[], struct Passenger *newPassenger, int *loc){
    TripDatabase[*loc].Passengers[TripDatabase[*loc].volume] = *newPassenger;
    TripDatabase[*loc].volume++;
}

void 
setUpBusTrip(struct Bus16 *BusTrip, struct Bus16 TripDatabase[], struct Passenger *newPassenger, int *loc,  struct dropOffPointList *exits){
    addPassengers(TripDatabase, newPassenger, loc);
    strcpy(TripDatabase[*loc].route, newPassenger->route);
}

void
switchPassengers(struct Passenger *passengerSRCE, struct Passenger *passengerDEST){
    struct Passenger holdingPassengerSpot;
    holdingPassengerSpot = *passengerSRCE;
    *passengerSRCE = *passengerDEST;
    *passengerDEST = holdingPassengerSpot;
}


int 
findingAlternativeTrip(int origTripLocation, struct Bus16 TripDatabase[], struct Passenger *newPassenger, struct dropOffPointList exits[]){   
    // Boolean Conditions
    int hasEntered          = FALSE;
    int needSpecialBus      = FALSE;
    int hasTranferee        = TRUE;

    int transferringTripLoc = 0;
    int tripRanges          = 0;
    int i = 0;
    int passengerIndex = 0;
    
    struct Passenger transferringPerson;
    struct Bus16 BusTrip;
    
    transferringPerson = *newPassenger;
    transferringTripLoc = origTripLocation + 1;

    if (transferringTripLoc < 9) {
        tripRanges = 9;
    } else if (transferringTripLoc == 9) {
        needSpecialBus = TRUE;
        transferringTripLoc = 21;
    } else if (transferringTripLoc < 20) {
        tripRanges = 20;
    } else {
        needSpecialBus = TRUE;
        transferringTripLoc = 22;
    }
    
    while (hasTranferee && !needSpecialBus) {
        hasEntered = FALSE;
        for (i = transferringTripLoc; i < tripRanges && !hasEntered; i++){
            if (TripDatabase[i].volume < 16 && strcmp(TripDatabase[i].route, transferringPerson.route) == 0){

                hasTranferee    = FALSE;
                hasEntered      = TRUE;

                addPassengers(TripDatabase, &transferringPerson, &i);
                printPassenger(&transferringPerson);
                printf("%s has been added to %s. \n", transferringPerson.passengerName.lastName, TripDatabase[i].TripID);
            } else if (strcmp(TripDatabase[i].route, transferringPerson.route) == 0) {
                for(passengerIndex = 0; passengerIndex < 16 && !hasEntered; passengerIndex++){
            
                    if (TripDatabase[i].Passengers[passengerIndex].priorityNumber > transferringPerson.priorityNumber){
                        hasTranferee    = TRUE;
                        hasEntered      = TRUE;

                        switchPassengers(TripDatabase[i].Passengers + passengerIndex, &transferringPerson);
                        
                        printSingleColorText( FG_RED , "WARNING, Passenger of the following kicked out:\n");
                        printPassenger(&transferringPerson);
                        printf("%s Has been kicked out by %s in trip %s.", transferringPerson.passengerName.lastName, TripDatabase[i].Passengers[passengerIndex].passengerName.lastName, TripDatabase[i].TripID);
                        printf("Transferring...\n\n");
                    }
                }
            }
        }

        if (!hasEntered) {
            needSpecialBus = TRUE;
            *newPassenger = transferringPerson;
        }
    }

    return hasTranferee;
}

int
assignSpecialTrip(struct Passenger *transferringPerson, struct Bus16 TripDatabase[], struct dropOffPointList exits[]){
    String127 wantedDropOff = "";
    
    int hasEnterableMNLBus  = FALSE;
    int hasEnterableLAGBus  = FALSE;
    int isConfirmed         = FALSE;
    int isFindingDropOff    = FALSE;
    int hasEntered          = FALSE;
    int specialChoice       = 0;
    int loc;

    hasEnterableMNLBus  =   strcmp(transferringPerson->embarkationPoint, "DLSU Manila Campus - South Gate Driveway") == 0 &&
                            TripDatabase[20].volume < 16;
    hasEnterableLAGBus  =   strcmp(transferringPerson->embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy") == 0 &&
                            TripDatabase[21].volume < 16;
    isFindingDropOff = FALSE;

    if (hasEnterableMNLBus){
        while(!isFindingDropOff){
            isConfirmed = FALSE;

            printf("Dear %s %s, a special event occured with your trip:\n", transferringPerson->passengerName.firstName, transferringPerson->passengerName.lastName);
            repeatGetInteger(&specialChoice, "Special1", "You have entered a Special Shuttle; \n\tPlease select your drop-off Point:", "Enter");
            
            if (specialChoice >= 0 && specialChoice < 3)
                strcpy(wantedDropOff, exits[0].dropOffs[specialChoice]);

            while(!isConfirmed && specialChoice >= 0 && specialChoice < 3){
                printf("New drop-off: %s\n", wantedDropOff);
                validateUserInput(&isConfirmed, &isFindingDropOff, "Error, please decide if this is the Drop-off you want.");
            }
        }
        loc = 20;
        strcpy(transferringPerson->tripNumber, "SP101");
        strcpy(transferringPerson->dropOffPoint, wantedDropOff);

        addPassengers(TripDatabase, transferringPerson, &loc);
        printf("%s has been added to %s. \n", transferringPerson->passengerName.lastName, TripDatabase[20].TripID);
        hasEntered = TRUE;
    } else if (hasEnterableLAGBus) {
        while(!isFindingDropOff){
            isConfirmed = FALSE;

            printf("Dear %s %s, a special event occured with your trip:\n", transferringPerson->passengerName.firstName, transferringPerson->passengerName.lastName);
            repeatGetInteger(&specialChoice, "Special2", "You have entered a Special Shuttle, please select your drop-off Point:", "Enter");
            
            if (specialChoice >= 0 && specialChoice < 4)
                strcpy(wantedDropOff, exits[2].dropOffs[specialChoice]);

            while(!isConfirmed && specialChoice >= 0 && specialChoice < 4){
                printf("New drop-off: %s\n", wantedDropOff);
                validateUserInput(&isConfirmed, &isFindingDropOff, "Error, please decide if this is the Drop-off you want.");
            }
        }
        loc = 21;
        strcpy(transferringPerson->tripNumber, "SP150");
        strcpy(transferringPerson->dropOffPoint, wantedDropOff);

        addPassengers(TripDatabase, &transferringPerson, &loc);
        printf("%s has been added to %s. \n", transferringPerson->passengerName.lastName, TripDatabase[21].TripID);
        hasEntered = TRUE;
    } else {
        printf("Sorry, all trips have been occupied.");
    }
    return hasEntered;
}

void
userEmbarkation(struct Bus16 TripDatabase[], struct dropOffPointList exits[]){ // Params: struct Passenger Passengers[16]
    struct Passenger    newPassenger;
    struct Passenger    transfreePassenger;
    struct Bus16    BusTrip;
    String255   validationError = "Error, please decide if you want to embark based on the info.";
    TripNo      numInput;
    int isChoosingToInput   = TRUE;
    int isEmbarking         = TRUE;
    int hasTransferee       = FALSE;
    int tripIndex   = 0;
    int Passengers  = 0;
    int i   = 0;
    int j   = 0;
    
    String63 strFiller = "User creates an embarkation trip.";
    printSingleColorText(FG_GREEN, strFiller);

    while (isEmbarking) {
        isChoosingToInput = TRUE;
        printTrips(TripDatabase);

        while (isChoosingToInput){
            printf("Do you want to embark?\n");
            validateUserInput(&isChoosingToInput, &isEmbarking, validationError);
        }

        if (isEmbarking)
            return;

        printTrips(TripDatabase);
        repeatGetPassenger(&newPassenger, TripDatabase, exits);

        Passengers = tripStruct_SearchBusTrip(newPassenger.tripNumber, TripDatabase, &BusTrip, &tripIndex);

        if (Passengers <= 13)
            printSeats13(BusTrip.volume);
        else if (Passengers < 16)
            printSeats16(BusTrip.volume);

        if (BusTrip.volume == 0) {
            setUpBusTrip(&BusTrip, TripDatabase, &newPassenger, &tripIndex, exits);
        } else if (BusTrip.volume > 0 && BusTrip.volume < 16) {
            addPassengers(TripDatabase, &newPassenger, &tripIndex);
        } else {
            hasTransferee = findingAlternativeTrip(tripIndex, TripDatabase, &newPassenger, exits);
        }

        if (hasTransferee)
            assignSpecialTrip(&newPassenger, TripDatabase, exits);
        
    }

}

// |===| ADMIN CMD SECTION ==========================|
/**
 * @brief  
 * @note   
 * @retval None
 */
void 
adminNoOfPassenger(struct Bus16 Trips[]){
    String63    strFiller = "Admin views a Trip's no. of Passenger";
    TripNo      inputTrip = "";
    struct Bus16 busHolder;
    int Passengers          = 0;
    int wantedTripIndex     = 0;
    int isFinding = TRUE;

    system("cls");
    printSingleColorText( FG_YELLOW, strFiller);

    do {
        printTrips(Trips);
        repeatGetTripNo(inputTrip, "PassEmbark3", "\t> Please Input Trip No: ", "Enter a valid one.");

        if (strcmp(inputTrip, "quit") == 0){
            isFinding = 0;
            return;
        }

        Passengers = tripStruct_SearchBusTrip(inputTrip, Trips, &busHolder, &wantedTripIndex);

        if (Passengers <= 13)
            printSeats13(busHolder.volume);
        else if (Passengers < 16)
            printSeats16(busHolder.volume);

    } while (isFinding);
}

/**
 * @brief  Counts the Passenger in a Drop Off.
 * @note   
 * @param *tripDate: 
 * @param Trips[]: 
 */
void 
adminCountPassengerDropOff(struct Bus16 TripDatabase[]){
    String63 strFiller = "Admin counts number of Passenger in a drop-off.";
    TripNo inputTripNumber = ""; 
    
    struct Bus16 pickedTrip;
    struct SearchResultField DropOffResults;
    int isChoiceConfirmed = FALSE;
    int isChoosing = TRUE;
    int userChoice;
    int results;
    int loc;

    system("cls");
    printSingleColorText(FG_YELLOW, strFiller);

    while (isChoosing) {

        isChoiceConfirmed = FALSE;

        while(!isChoiceConfirmed){
            printGraphics("CountPassenger");
            validateUserInput(&isChoiceConfirmed, &isChoosing, "Please choose whether to count or not.");
        }

        if (!isChoosing)
            isChoosing = !isChoosing;
        else
            return;

        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip.");
        results = tripStruct_SearchBusTrip(inputTripNumber, TripDatabase, &pickedTrip, &loc);

        if (strcmp(inputTripNumber, "quit") == 0){
            isChoosing = FALSE;
            return;
        }

        //system("cls");
        printGraphics("DropOff1");
        printf("| Trip: %s \n| Results: %d\n", inputTripNumber, results);
        if (results > 0) {            
            DropOffResults = countWordFrequency(pickedTrip, results);
            printDropOffs(&DropOffResults);
        }
        printGraphics("DropOff3");

    }
}

/**
 * @brief  
 * @note   
 * @param  *tripDate: 
 * @param  TripDatabase[]: 
 * @retval None
 */
void 
adminViewPassengerInfo(struct Bus16 TripDatabase[]){
    String63 strFiller = "Admin views the passenger info.";
    printSingleColorText( FG_YELLOW, strFiller);
    struct Bus16 BusHolder;
    TripNo inputTripNumber = "";
    int isDoneVieweing = FALSE;
    int passengers;
    int loc;

    while (!isDoneVieweing) {
        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip. \n\tType \'0\' to exit.");
        
        if (strcmp(inputTripNumber, "quit") == 0)
            return;
        
        printf("Trip: %s\n", inputTripNumber);    
        passengers = tripStruct_SearchBusTrip(inputTripNumber, TripDatabase, &BusHolder, &loc);
        
        printPassengerInfo(inputTripNumber, &BusHolder, passengers);
    }
}

/**
 * @brief Searches the last name of a passenger in a trip file and lets the user search each one.
 * @note   
 * @param *dateToday: A DateDMY struct that represents the date of the trip to be opened
 * @return void
 */
void
adminSearchPassenger(struct Bus16 Trips[]){
    struct SearchResultField lastNameResults;
    String63 strFiller = "Admin searches the passenger in a trip.";
    String15 nameToSearch;
    int isFinding = TRUE;
    
    
    printSingleColorText( FG_YELLOW, strFiller);

    while (isFinding){
        repeatGetString(nameToSearch, 15, "SearchPass", "\t> Last Name: ", "Error, not a last name");
        printf("\n");
        removeNewline(nameToSearch);

        if (strcmp(nameToSearch, "quit") == 0)
            return;
        
        lastNameResults.size = tripStruct_ReturnLastname(Trips, nameToSearch, &lastNameResults);
    
        printSearchResults(&lastNameResults, Trips, nameToSearch);
    }
}


void 
adminEmbarkation(struct Bus16 TripDatabase[], struct dropOffPointList exits[]){
    struct Passenger    newPassenger;
    struct Passenger    transfreePassenger;
    struct Bus16    newBusTripDatabase[22];
    struct Bus16    newBusTrip;
    struct DateDMY      date;
    String255   validationError = "Error, please decide if you want to embark based on the info.";
    TripNo      numInput;
    int isChoosingToInput   = TRUE;
    int isEmbarking         = TRUE;
    int hasTransferee       = FALSE;
    int tripIndex   = 0;
    int Passengers  = 0;
    int i   = 0;
    int j   = 0;

    String63 strFiller = "Admin creates an embarkation trip.";
    printSingleColorText( FG_YELLOW, strFiller);

    while (isEmbarking) {
        
        isChoosingToInput = TRUE;
        printTrips(TripDatabase);

        while (isChoosingToInput){
            printf("Do you want to embark?\n");
            validateUserInput(&isChoosingToInput, &isEmbarking, validationError);
        }

        if (isEmbarking)
            return;

        printTrips(TripDatabase);

        repeatGetDateDMY(&date);
        tripFile_GetBusTrip(&date, newBusTripDatabase, exits);

        for (i = 0; i < TOTAL_TRIPS; i++){
            Passengers = newBusTripDatabase[i].volume;

            if (Passengers > 0 && Passengers <= 16) {
                hasTransferee = FALSE;

                for (j = 0; j < Passengers; j++){
                    transfreePassenger = newBusTripDatabase[i].Passengers[j];
                    if (TripDatabase[i].volume < 16)
                        addPassengers(TripDatabase, &transfreePassenger, &i);
                    else if (i < 20) // special trips are 20 and 21
                        hasTransferee = findingAlternativeTrip(i, TripDatabase, &newPassenger, exits);
                }

                if (hasTransferee)
                    hasTransferee = assignSpecialTrip(&newPassenger, TripDatabase, exits);
                
                if (hasTransferee){
                    printf("Passenger has been dropped:\n");
                    printPassenger(&newPassenger);
                }
            }
        }
    }
}

void
adminLoadFile(struct Bus16 TripDatabase[], struct dropOffPointList *exits){
    FILE *databaseFile;
    struct DateDMY  date;
    String31 fileName;
    String63 validationError = "Error, please choose whether or not you want to load the file.";
    repeatGetDateDMY(&date);
    int isFileExist;
    int isConfirmed = FALSE;
    int isChoosingFile = TRUE;
    
    while (isChoosingFile){
        StringfromDateDMY(fileName, &date, TRUE);
        databaseFile = fopen(fileName, "r");
        if (databaseFile == NULL)
            isFileExist = FALSE;
        else 
            isFileExist = TRUE;
        fclose(databaseFile);

        if (!isFileExist) {
            printSingleColorText(FG_RED, "File does not exist.\n");
        }

        isConfirmed = FALSE;
        while (!isConfirmed && isFileExist){
            printSingleColorText(FG_GREEN, "File does exist. Continue with Load?\n");
            validateUserInput(&isConfirmed, &isChoosingFile, &validationError);
        }

        if (isConfirmed && isFileExist)
            tripFile_GetBusTrip(&date, TripDatabase, exits);
    }
    
}

// |===| MENU SECTION |=============================|

/**
 * TODO fill it functions accessible to a passenger based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 */
void 
menuPassenger(struct Bus16 Trips[], struct dropOffPointList *exits){
    char inputPassMenu;
    String63 errorMessage = "Dear Passenger, Please select the following valid cmds\n";
    String15 graphicCode = "PassengerMenu";
    String15 strPrompt = "\t> Action: ";
    do {
        repeatGetChar(&inputPassMenu, graphicCode, strPrompt, errorMessage);
        switch(inputPassMenu) {
            case 'A':
            case 'a':
                userEmbarkation(Trips, exits);
                break;
            default:
                printErrorMessage(errorMessage);
            case 'B':
            case 'b':
                break;
        }
    } while (inputPassMenu != 'b');
    system("cls");
}

/**
 * TODO fill it functions accessible to an admin based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 * 
 */
void 
menuAdmin(struct Bus16 BusTrip[], int size, struct DateDMY *date, struct dropOffPointList exits[]){
    String255 realPass = "Admin123"; // to be changed
    String63 errorMessage = "Please input a valid admin cmd.\n";
    String15 graphicCode = "AdminMenu";
    String15 strPrompt = "\t> Command: ";
    int isChoosingAdminCmds = FALSE;
    int isInputing = TRUE;
    char userInput;
    system("cls");
    while (isInputing && !isChoosingAdminCmds)
        passwordMenu(&isChoosingAdminCmds, &isInputing, realPass);
  
    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, graphicCode, strPrompt, errorMessage);

        switch(userInput){
            case 'A':
            case 'a':
                adminNoOfPassenger(BusTrip);
                break;
            case 'B':
            case 'b':
                adminCountPassengerDropOff(BusTrip);
                break;
            case 'C':
            case 'c':
                adminViewPassengerInfo(BusTrip);
                break;
            case 'D':
            case 'd':
                adminSearchPassenger(BusTrip);
                break;
            case 'E':
            case 'e':
                adminEmbarkation(BusTrip, exits);
                break;
            case 'F':
            case 'f':
                adminLoadFile(BusTrip, exits);
                break;
            case 'G':
            case 'g':
                isChoosingAdminCmds = FALSE;
        }
    } 
    system("cls");
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
