#include "utilsPass.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Subsection of commands

/**
 * @brief Adds passenger to a trip and updates them.
 * @param TripDatabase[]: the Bus16 database to be added.
 * @param *newPassenger: the passenger to be added.
 * @param *tripLocation: location of the trip in the database.
 * @return None
 */
void 
addPassengers(struct Bus16 TripDatabase[], struct Passenger *newPassenger, int *tripLocation){
    TripDatabase[*tripLocation].Passengers[TripDatabase[*tripLocation].volume] = *newPassenger;
    TripDatabase[*tripLocation].volume++;
}

/**
 * @brief Switch passenger 
 * @param *passengerSRCE: a Passenger struct to swap FROM.
 * @param *passengerDEST: a Passenger struct to swap TO.
 * @return none
 */
void
switchPassengers(struct Passenger *passengerSRCE, struct Passenger *passengerDEST){
    struct Passenger holdingPassengerSpot;
    holdingPassengerSpot = *passengerSRCE;
    *passengerSRCE = *passengerDEST;
    *passengerDEST = holdingPassengerSpot;
}

/**
 * @brief Find an alternative trip for the switched passenger.
 * @param origTripLocation: The original trip's index location in the database
 * @param TripDatabase[]: A Bus16 struct array database of the trips
 * @param *newPassenger: The Passenger to transfer
 * @param exits[]: All of the information of the exits and routes in a dropOffPointList struct array.
 * @return Boolean if a Trip
 * @retval TRUE if it has an remaining passenger.
 * @retval FALSE if it has no remaining passenger.
 */
int 
findingAlternativeTrip(int origTripLocation, struct Bus16 TripDatabase[], struct Passenger *newPassenger, struct dropOffPointList exits[]){
	String127 WarningMessage1 =  "WARNING, Passenger of the following kicked out:\n";   
    struct Passenger transferringPerson;

    // Boolean Conditions
    int hasEntered          = FALSE;
    int needSpecialBus      = FALSE;
    int hasTranferee        = TRUE;

    int transferringTripLoc = 0;
    int tripRanges          = 0;
    int passengerIndex      = 0;
    int i = 0;
    
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
                        
                        printSingleColorText(FG_RED , WarningMessage1);
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

/**
 * @brief Assigns a special trip to a special trip.
 * @param *transferringPerson: a Passenger struct of the unassigned person.
 * @param TripDatabase[]: a Bus16 struct array database of the trips
 * @param exits[]: an array of dropOffPointList structs of the exits
 * @return *loc: the trip the function tried to assign the user in
 * @retval TRUE if the user has entered
 * @retval FALSE if the user has not entered.
 */
int
assignSpecialTrip(struct Passenger *transferringPerson, struct Bus16 TripDatabase[], struct dropOffPointList exits[], int *loc){
    String127 wantedDropOff = "";
    String15 graphicsCode1	= "Special1";
    String15 graphicsCode2  = "Special2";
    String127 promptSpecial 	= "You have entered a Special Shuttle; \n\tPlease select your drop-off Point:";
    String63 errorCode		= "Enter the right choice number.";
    String63 errorCodePleaseDecide = "Error, please decide if this is the Drop-off you want.";
    int hasEnterableMNLBus  = FALSE;
    int hasEnterableLAGBus  = FALSE;
    int isConfirmed         = FALSE;
    int isFindingDropOff    = FALSE;
    int hasEntered          = FALSE;
    int specialChoice       = 0;

    hasEnterableMNLBus  =   strcmp(transferringPerson->embarkationPoint, "DLSU Manila Campus - South Gate Driveway") == 0 &&
                            TripDatabase[20].volume < 16;
    hasEnterableLAGBus  =   strcmp(transferringPerson->embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy") == 0 &&
                            TripDatabase[21].volume < 16;
    isFindingDropOff = FALSE;

    if (hasEnterableMNLBus){
        while(!isFindingDropOff){
            isConfirmed = FALSE;

            printf("Dear %s %s, a special event occured with your trip:\n", transferringPerson->passengerName.firstName, transferringPerson->passengerName.lastName);
            repeatGetInteger(&specialChoice, graphicsCode1, promptSpecial, errorCode);
            
            if (specialChoice >= 0 && specialChoice < 3)
                strcpy(wantedDropOff, exits[0].dropOffs[specialChoice]);

            while(!isConfirmed && specialChoice >= 0 && specialChoice < 3){
                printf("New drop-off: %s\n", wantedDropOff);
                validateUserInput(&isConfirmed, &isFindingDropOff, errorCodePleaseDecide);
            }
        }
        *loc = 20;
        strcpy(transferringPerson->tripNumber, "SP101");
        strcpy(transferringPerson->dropOffPoint, wantedDropOff);

        addPassengers(TripDatabase, transferringPerson, loc);
        printf("%s has been added to %s. \n", transferringPerson->passengerName.lastName, TripDatabase[20].TripID);
        hasEntered = TRUE;
    } else if (hasEnterableLAGBus) {
        while(!isFindingDropOff){
            isConfirmed = FALSE;

            printf("Dear %s %s, a special event occured with your trip:\n", transferringPerson->passengerName.firstName, transferringPerson->passengerName.lastName);
            repeatGetInteger(&specialChoice, graphicsCode2, promptSpecial, errorCode);
            
            if (specialChoice >= 0 && specialChoice < 4)
                strcpy(wantedDropOff, exits[2].dropOffs[specialChoice]);

            while(!isConfirmed && specialChoice >= 0 && specialChoice < 4){
                printf("New drop-off: %s\n", wantedDropOff);
                validateUserInput(&isConfirmed, &isFindingDropOff, errorCodePleaseDecide);
            }
        }
        *loc = 21;
        strcpy(transferringPerson->tripNumber, "SP150");
        strcpy(transferringPerson->dropOffPoint, wantedDropOff);

        addPassengers(TripDatabase, transferringPerson, loc);
        printf("%s has been added to %s. \n", transferringPerson->passengerName.lastName, TripDatabase[21].TripID);
        hasEntered = TRUE;
    }

    return hasEntered;
}

// |===| PASSENGER CMD SECTION |=====================|

/**
 * @brief Embarks users in a trip manually.
 * @param TripDatabase[]: a pointer to the Bus16 struct array. 
 * @param exits[]: a pointer to the array of dropOffPointList struct.
 * @return None
 */
void
userEmbarkation(struct Bus16 TripDatabase[], struct dropOffPointList exits[]){ // Params: struct Passenger Passengers[16]
    struct Passenger    newPassenger;
    struct Bus16    BusTrip;
    String255   validationError = "Error, please decide if you want to embark based on the info.";
    int isChoosingToInput   = TRUE;
    int isEmbarking         = TRUE;
    int hasTransferee       = FALSE;
    int tripIndex   = 0;
    int Passengers  = 0;

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

        if (BusTrip.volume < 16) {
            addPassengers(TripDatabase, &newPassenger, &tripIndex);
        } else {
            hasTransferee = findingAlternativeTrip(tripIndex, TripDatabase, &newPassenger, exits);
        }

        if (hasTransferee)
            hasTransferee = assignSpecialTrip(&newPassenger, TripDatabase, exits, &tripIndex);
        
        if (hasTransferee)
            printf("Sorry, all trips for %s including %s is filled.\n", TripDatabase[tripIndex].embarkationPoint, TripDatabase[tripIndex].TripID); 
    }

}

// |===| ADMIN CMD SECTION ==========================|

/**
 * @brief Sees the number of Passenger in a Trip.
 * @param TripDatabase[]: a pointer to the Bus16 struct array. 
 * @return None
 */
void 
adminNoOfPassenger(struct Bus16 TripDatabase[]){
	String15 graphicsCode1 = "CountPassenger";
	String15 graphicsCode2 = "PassEmbark3";
	String31 promptMessage = "\t> Please Input Trip No: ";
	String31 errorMessageValid = "Enter a valid one.";
	String63 errorCodePleaseDecide = "Please choose whether to count or not.";
    struct Bus16 busHolder;
    String63    strFiller = "Admin views a Trip's no. of Passenger";
    TripNo      inputTrip = "";
    int Passengers          = 0;
    int wantedTripIndex     = 0;
    int isChoiceConfirmed   = FALSE;
    int isFinding           = TRUE;

    system("cls");
    printSingleColorText( FG_YELLOW, strFiller);

    do {
        printTrips(TripDatabase);

        isChoiceConfirmed = FALSE;
        while(!isChoiceConfirmed){
            printGraphics(graphicsCode1);
            validateUserInput(&isChoiceConfirmed, &isFinding, errorCodePleaseDecide);
        }
        
        if (!isFinding)
            isFinding = 0;
        else
            return;

        repeatGetTripNo(inputTrip, graphicsCode2, promptMessage, errorMessageValid);

        Passengers = tripStruct_SearchBusTrip(inputTrip, TripDatabase, &busHolder, &wantedTripIndex);

        if (Passengers <= 13)
            printSeats13(busHolder.volume);
        else if (Passengers < 16)
            printSeats16(busHolder.volume);

    } while (isFinding);
}

/**
 * @brief Counts the Passenger in a Drop Off.
 * @param TripDatabase[]: a pointer to the Bus16 struct array.
 * @return None
 */
void 
adminCountPassengerDropOff(struct Bus16 TripDatabase[]){
    String63 strFiller = "Admin counts number of Passenger in a drop-off.";
    TripNo inputTripNumber = ""; 
    
    String15 graphicsCode1 = "CountPassenger";
    String15 graphicsCode2 = "DropOff1";
    String15 graphicsCode3 = "DropOff2";
	String31 promptMessage = "\t> Trip Number:";
	String31 errorMessageValid = "Please input an existing trip.";
	String63 errorCodePleaseDecide = "Please choose whether to count or not.";
    
    struct Bus16 pickedTrip;
    struct SearchResultField DropOffResults;
    int isChoiceConfirmed = FALSE;
    int isChoosing = TRUE;
    int results;
    int loc;

    system("cls");
    printSingleColorText(FG_YELLOW, strFiller);

    while (isChoosing) {

        isChoiceConfirmed = FALSE;
        while(!isChoiceConfirmed){
            printGraphics(graphicsCode1);
            validateUserInput(&isChoiceConfirmed, &isChoosing, errorCodePleaseDecide);
        }

        if (!isChoosing)
            isChoosing = !isChoosing;
        else
            return;

        repeatGetTripNo(inputTripNumber, graphicsCode1, promptMessage, errorMessageValid);
        results = tripStruct_SearchBusTrip(inputTripNumber, TripDatabase, &pickedTrip, &loc);

        if (strcmp(inputTripNumber, "quit") == 0){
            isChoosing = FALSE;
            return;
        }

        //system("cls");
        printGraphics(graphicsCode2);
        printf("| Trip: %s \n| Results: %d\n", inputTripNumber, results);
        if (results > 0) {            
            DropOffResults = countDropOffFrequency(&pickedTrip);
            printDropOffs(&DropOffResults);
        }
        printGraphics(graphicsCode3);

    }
}

/**
 * @brief Admin command for view passengers.
 * @param TripDatabase[]: a pointer to the Bus16 struct array. 
 * @return None
 */
void 
adminViewPassengerInfo(struct Bus16 TripDatabase[]){
    String63 strFiller = "Admin views the passenger info.";
    String15 graphicsCode1 = "CountPassenger";
    String15 promptMessage1 = "\t> Trip Number:";
    String63 ErrorMessage1 = "Please input an existing trip. \n\tType \'0\' to exit.";
    printSingleColorText( FG_YELLOW, strFiller);
    struct Bus16 BusHolder;
    TripNo inputTripNumber = "";
    int isDoneVieweing = FALSE;
    int loc;

    while (!isDoneVieweing) {
        repeatGetTripNo(inputTripNumber, graphicsCode1, promptMessage1, ErrorMessage1);
        
        if (strcmp(inputTripNumber, "quit") == 0)
            return;
        
        printf("Trip: %s\n", inputTripNumber);    
        tripStruct_SearchBusTrip(inputTripNumber, TripDatabase, &BusHolder, &loc);
        
        printListofPassenger(inputTripNumber, &BusHolder);
    }
}

/**
 * @brief Admin command for searching the last name of a passenger in a trip file and lets the user search each one.
 * @param TripDatabase[]: a pointer to the Bus16 struct array. 
 * @return void
 */
void
adminSearchPassenger(struct Bus16 TripDatabase[]){
    struct SearchResultField lastNameResults;
    String63 strFiller = "Admin searches the passenger in a trip.";
    String15 nameToSearch;
    String15 graphicsCode1 = "SearchPass";
    String15 promptMessage1 = "\t> Last Name: ";
    String31 errorCode1 = "Error, not a last name";
    int isFinding = TRUE;
    
    printSingleColorText( FG_YELLOW, strFiller);

    while (isFinding){
        repeatGetString(nameToSearch, 15, graphicsCode1, promptMessage1, errorCode1);
        printf("\n");
        removeNewline(nameToSearch);

        if (strcmp(nameToSearch, "quit") == 0)
            return;
        
        lastNameResults.size = tripStruct_ReturnLastname(TripDatabase, nameToSearch, &lastNameResults);
    
        printSearchResults(&lastNameResults, TripDatabase, nameToSearch);
    }
}

/**
 * @brief Admin command for embarkation using a .txt file.
 * @param TripDatabase[]: a pointer to the Bus16 struct array. 
 * @param exits[]: a pointer to the array of dropOffPointList struct.
 * @return None
 */
void 
adminEmbarkation(struct Bus16 TripDatabase[], struct dropOffPointList exits[]){
    struct Passenger    newPassenger;
    struct Passenger    transfreePassenger;
    struct Bus16    newBusTripDatabase[22];
    struct DateDMY      date;
    String255   validationError = "Error, please decide if you want to embark based on the info.";
    int isChoosingToInput   = TRUE;
    int isEmbarking         = TRUE;
    int hasTransferee       = FALSE;
    int Passengers  = 0;
    int newLocation = 0;
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
                    hasTransferee = assignSpecialTrip(&newPassenger, TripDatabase, exits, &newLocation);
                
                if (hasTransferee) {
                    printf("Passenger has been dropped even in trip %s:\n", TripDatabase[newLocation].TripID);
                    printPassenger(&newPassenger);
                }
            }
        }
    }
}


/**
 * @brief Admin command for loading a .txt savefile into current TripDatabase[]  
 * @param TripDatabase[]: a pointer to a Bus16 struct array.
 * @param exits[]: a pointer to the array of dropOffPointList struct.
 * @return None
 */
void
adminLoadFile(struct Bus16 TripDatabase[], struct dropOffPointList exits[]){
    FILE *databaseFile;
    struct DateDMY  date;
    String31 fileName;
    String63 validationError = "Error, please choose whether or not you want to load the file.";
    String31 fileErrorMessage = "File does not exist.\n";
    String63 confirmationMessage = "File does exist. Continue with Load?\n";
    repeatGetDateDMY(&date);
    int isFileExist;
    int isConfirmed = FALSE;
    int isChoosingFile = TRUE;
    
    while (isChoosingFile){
        StringfromDateDMY(fileName, &date, TRUE);

        databaseFile = fopen(fileName, "r");
        isFileExist = (databaseFile == NULL) ? FALSE : TRUE;
        fclose(databaseFile);

        if (!isFileExist) {
            printSingleColorText(FG_RED, fileErrorMessage);
        }

        isConfirmed = FALSE;
        while (!isConfirmed && isFileExist){
            printSingleColorText(FG_GREEN, confirmationMessage);
            validateUserInput(&isConfirmed, &isChoosingFile, validationError);
        }

        if (isConfirmed && isFileExist)
            tripFile_GetBusTrip(&date, TripDatabase, exits);
    }
    
}

// |===| MENU SECTION |=============================|

/**
 * @brief Menu for User
 * @param BusTripDatabase[]: an array of Bus16 structs
 * @param *date: a pointer to a DateDMY struct of today
 * @param exits[]: an array of dropOffPointList struct
 */
void 
menuPassenger(struct Bus16 BusTripDatabase[], struct dropOffPointList *exits){
    char inputPassMenu;
    String63 errorMessage = "Dear Passenger, Please select the following valid cmds\n";
    String15 graphicCode = "PassengerMenu";
    String15 strPrompt = "\t> Action: ";
    do {
        repeatGetChar(&inputPassMenu, graphicCode, strPrompt, errorMessage);
        switch(inputPassMenu) {
            case 'A':
            case 'a':
                userEmbarkation(BusTripDatabase, exits);
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
 * @brief Menu for Admin
 * @param BusTripDatabase[]: an array of Bus16 structs
 * @param *date: a pointer to a DateDMY struct of today
 * @param exits[]: an array of dropOffPointList struct
 */
void 
menuAdmin(struct Bus16 BusTripDatabase[], struct DateDMY *date, struct dropOffPointList exits[]){
    String255 realPass      = "Admin123"; // to be changed
    String63 errorMessage   = "Please input a valid admin cmd.\n";
    String15 graphicCode    = "AdminMenu";
    String15 strPrompt      = "\t> Command: ";

    int isChoosingAdminCmds = FALSE;
    int isInputing = TRUE;
    char userInput;

    system("cls");

    while (isInputing && !isChoosingAdminCmds)
        passwordChecker(&isChoosingAdminCmds, &isInputing, realPass);
  
    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, graphicCode, strPrompt, errorMessage);
        switch(userInput){
            case 'A':
            case 'a':
                adminNoOfPassenger(BusTripDatabase);
                break;
            case 'B':
            case 'b':
                adminCountPassengerDropOff(BusTripDatabase);
                break;
            case 'C':
            case 'c':
                adminViewPassengerInfo(BusTripDatabase);
                break;
            case 'D':
            case 'd':
                adminSearchPassenger(BusTripDatabase);
                break;
            case 'E':
            case 'e':
                adminEmbarkation(BusTripDatabase, exits);
                break;
            case 'F':
            case 'f':
                adminLoadFile(BusTripDatabase, exits);
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
