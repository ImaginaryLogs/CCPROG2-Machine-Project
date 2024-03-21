#include "utilsPass.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// |===| PASSENGER CMD SECTION |=====================|

void
userEmbarkation(struct Bus16 Trips[], struct dropOffPointList *exits){ // Params: struct Passenger Passengers[16]
    struct Bus16 BusTrip;
    struct Passenger newPassenger;
    struct Passenger transfreePassenger;
    struct Passenger holdingPassengerSpot;
    TripNo numInput;
    int Passengers;
    int isFinding = TRUE;
    int prioInput;
    int i, j;
    int loc;
    int tripRanges;
    int hasEntered;
    int hasNotEnteredOriginalTrip = FALSE;
    int hasTransferee = FALSE;
    int isSearchingDropOff = TRUE;
    String63 strFiller = "User creates an embarkation trip.";
    printSingleColorText(BACKGROUND_GREEN, strFiller);

    while (isFinding){
        hasNotEnteredOriginalTrip = FALSE;
        printTrips(Trips);
        repeatGetTripNo(numInput, "PassEmbark3", "\t> Please Input Trip No: ", "Enter a valid one.");

        if (strcmp(numInput, "quit") == 0){
            return;
        }

        Passengers = tripStruct_GetBusTrip(numInput, Trips, &BusTrip, &loc);

        repeatGetString(&newPassenger.passengerName.firstName, 63, "PassEmbark4", "\t> First Name ", "Please enter within the range.");
        repeatGetString(&newPassenger.passengerName.lastName, 63, "PassEmbark4", "\t> Last Name ", "Please enter within the range.");
        repeatGetInteger(&newPassenger.priorityNumber, "PassEmbark4", "\t> Pass Ranking: ", "Please enter within the range.");

        if (Passengers <= 13)
            printSeats13(BusTrip.volume);
        else if (Passengers < 16)
            printSeats16(BusTrip.volume);

        if (BusTrip.volume == 0){
            BusTrip.Passengers[BusTrip.volume] = newPassenger;
            BusTrip.volume++;
            for (i = 0; i < 4 && isSearchingDropOff; i++){
                for (j = 0; j < exits->size && isSearchingDropOff; j++){
                    if (strcmp(exits->dropOffs[j], newPassenger.dropOffPoint) == 0){
                        strcpy(BusTrip.route, exits->route);
                        isSearchingDropOff = FALSE;
                    }
                }
            }
            Trips[loc] = BusTrip;
        } else if (BusTrip.volume > 0 && BusTrip.volume < 14) {
            BusTrip.Passengers[BusTrip.volume] = newPassenger;
            BusTrip.volume++;
            Trips[loc] = BusTrip;
        } else {
            hasNotEnteredOriginalTrip = TRUE;
        }

        while (hasNotEnteredOriginalTrip){
            tripRanges = (loc < 9) ? 9 : 20;
            hasEntered = FALSE;
            for(i = loc; i < tripRanges && !hasEntered; i++) {
                for(j = 0; j < 16 && !hasEntered; j++){
                    if (Trips[i].Passengers[j].priorityNumber < newPassenger.priorityNumber){
                        holdingPassengerSpot = Trips[i].Passengers[j];
                        Trips[i].Passengers[j] = transfreePassenger;
                        transfreePassenger = holdingPassengerSpot;
                        hasTransferee = TRUE;
                        hasEntered = TRUE;
                        hasNotEnteredOriginalTrip = FALSE;
                        printPassenger(&transfreePassenger);
                        printf("Has been kicked out. Transferring...\n");
                    }
                }
            }

            if (i == tripRanges || tripRanges == -1) {
                printf("All normal trips gone.\n");
            }
        }

        while (hasTransferee){
            tripRanges = (loc + 1 < 9) ? 9 : (loc + 1 == 10) ? -1 : (loc + 1 > 10 && loc + 1 < 20) ? 20 : -1;
            hasEntered = FALSE;
            for(i = loc + 1; i < tripRanges && !hasEntered; i++) {
                for(j = 0; j < 16 && !hasEntered; j++){
                    if (Trips[i].Passengers[j].priorityNumber < transfreePassenger.priorityNumber){
                        transfreePassenger = Trips[i].Passengers[j];
                        Trips[i].Passengers[j] = newPassenger;
                        hasTransferee = TRUE;
                        hasEntered = TRUE;
                        hasNotEnteredOriginalTrip = FALSE;
                        printPassenger(&transfreePassenger);
                        printf("Has been kicked out. Transferring...\n");
                    }
                }
            }
        }

    }
    

    // if (prioInput == 'Y' || prioInput == 'y' && Passengers > 12 /*return for full tripFileGetBusTrip */){

    // } else if (prioInput == 'N' || prioInput == 'n' && Passengers > 12 /*return for full*/) {

    // } else if ((prioInput == 'Y' || prioInput == 'y' || prioInput == 'N' || prioInput == 'n') && Passengers < 12 /*return for not full tripFileGetBusTrip*/){
    //     //put passenger in bus right away
    // }

}

// |===| ADMIN CMD SECTION ==========================|
/**
 * @brief  
 * @note   
 * @retval None
 */
void 
adminNoOfPassenger(struct Bus16 Trips[]){
    String63 strFiller = "Admin views a Trip's no. of Passenger";
    TripNo inputTrip = "";
    struct Bus16 busHolder;
    int Passengers;
    int loc;
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

        Passengers = tripStruct_GetBusTrip(inputTrip, Trips, &busHolder, &loc);
        if (Passengers <= 13)
            printSeats13(busHolder.volume);
        else if (Passengers < 16)
            printSeats16(busHolder.volume);
        getchar();
        
    } while (isFinding);
}

/**
 * @brief  Counts the Passenger in a Drop Off.
 * @note   
 * @param *tripDate: 
 * @param Trips[]: 
 */
void 
adminCountPassengerDropOff(struct Bus16 Trips[]){
    String63 strFiller = "Admin counts number of Passenger in a drop-off.";
    TripNo inputTripNumber = ""; 
    
    struct Bus16 pickedTrip;
    struct SearchResultField DropOffResults;
    int isChoosing = TRUE;
    int userChoice;
    int results;
    int loc;

    system("cls");
    printSingleColorText(FG_YELLOW, strFiller);

    while (isChoosing) {
        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip.");
        results = tripStruct_GetBusTrip(inputTripNumber, Trips, &pickedTrip, &loc);

        if (strcmp(inputTripNumber, "quit") == 0){
            isChoosing = FALSE;
            return;
        }

        system("cls");
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
 * @param  BusTrips[]: 
 * @retval None
 */
void 
adminViewPassengerInfo(struct Bus16 BusTrips[]){
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
        passengers = tripStruct_GetBusTrip(inputTripNumber, BusTrips, &BusHolder, &loc);
        
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

/**
 * @brief  
 * @note   
 * @retval None
 */
void 
adminEmbarkation(struct Bus16 Trips[]){
    String63 strFiller = "Admin creates an embarkation trip.";
    printSingleColorText( FG_YELLOW, strFiller);
    int isEmbarking = TRUE;

    while (isEmbarking){

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
            case 'a':
                userEmbarkation(Trips, exits);
                break;
            default:
                printErrorMessage(errorMessage);
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
menuAdmin(struct Bus16 BusTrip[], int size, struct DateDMY *date, struct dropOffPointList *exits){
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
            case 'a':
                adminNoOfPassenger(BusTrip);
                break;
            case 'b':
                adminCountPassengerDropOff(BusTrip);
                break;
            case 'c':
                adminViewPassengerInfo(date);
                break;
            case 'd':
                adminSearchPassenger(date);
                break;
            case 'e':
                adminEmbarkation(date);
                break;
            case 'f':
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