#include "UsefulFuncs.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
/*
 * REFERENCES:
 * [1] https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
 */


// |===| Essential Functions Section |=====================|
void 
passwordMenu(int *isChoosingAdminCmds, int *isInputingPass, char *realPass){
    String127 inputPass;
    String63 errorMessage = "Error, not a string.";
    String15 graphicCode = "PassMenu";
    String15 prompt = "\t> Password: ";
    String31 strWrongPass = "Wrong Input. Try again.";

    repeatGetString(inputPass, 127, graphicCode, prompt, errorMessage);
    printf("\n");

    if (strcmp(inputPass, realPass) == 0) {
        *isChoosingAdminCmds = TRUE;
    } else if (strcmp(inputPass, "quit") == 0){
        *isInputingPass = FALSE;
    } else {
        system("cls");
        printErrorMessage(strWrongPass);
    }
}

int
tripFileSearch_PassengerFull(struct DateDMY *tripDate, struct Passenger *keyPassenger){
    int i;
    int isFound = 0;
    int key = -1;
    for(i = 0; i < 16 && !isFound; i++){
        isFound = tripFile_ReturnPassenger(tripDate, keyPassenger, i) == 0 ? TRUE : -1;
        if (isFound)
            key = i;
    }
    return key;
}

int
tripFile_SearchSameTrip(struct DateDMY *tripDate, TripNo tripNumber, struct Bus16 BusOfTrip){
    FILE *pFileBusTrip;
    String15 fileName = "";
    struct Passenger tempPassenger;
    int isFileDoesNotExist = FALSE;
    int nPassengerInTrip;
    int nPassengerSearched;

    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;
    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        return EROR_FILE_NOT_FOUND;
    }
    int hasFoundPassenger = FALSE;
    
    nPassengerInTrip = 0;
    nPassengerSearched = 0;

    while (hasFoundPassenger != -1){
        hasFoundPassenger = tripFile_ReturnPassenger(tripDate, &tempPassenger, nPassengerSearched);

        if(strcmp(tempPassenger.tripNumber, tripNumber) == 0) {
            BusOfTrip.Passengers[nPassengerInTrip] = tempPassenger;
            nPassengerInTrip++;
        }

        nPassengerSearched++;
    }

    fclose(pFileBusTrip);
    return nPassengerInTrip;
}

int
tripFile_ReturnLastname(struct DateDMY *tripDate, struct Bus16 BusTrip, char *LastName, struct SearchResultField *nameResults){
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    struct NameField nameBuffer;
    int isFileDoesNotExist = FALSE;
    int hasFullSearches = FALSE;
    int hasNotFoundEOF = TRUE;
    int nMatchedNames = 0;
    int linesSearched = 0; 
    int linesIgnored = 0;
    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;
    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        printErrorMessage("ERROR DOES NOT EXIST");
        return EROR_FILE_NOT_FOUND;
    }

    fgets(temporaryBuffer, 255, pFileBusTrip);
    fgets(temporaryBuffer, 255, pFileBusTrip);

    while (hasNotFoundEOF && !hasFullSearches){
        if (fgets(strName, 255, pFileBusTrip) == NULL) {
            fclose(pFileBusTrip);
            return nMatchedNames;
        }

        removeNewline(strName);
        nameBuffer = GetNameFromString(strName);

        if (isSubString(LastName, nameBuffer.lastName)){
            printf("%s\n",strName);
            strcpy(nameResults->result[nMatchedNames], strName);
            nameResults->passengerIndex[nMatchedNames] = linesSearched;
            nMatchedNames++;
        }

        if (nMatchedNames == BUS_SIZE)
            hasFullSearches = TRUE;

        // Other information is ignored
        for(linesIgnored = 0; linesIgnored < 6 && hasNotFoundEOF; linesIgnored++)
            if (fgets(temporaryBuffer, 255, pFileBusTrip) == NULL) {
                fclose(pFileBusTrip);
                return nMatchedNames;
            }
            
        linesSearched++;
    }
    fclose(pFileBusTrip);
    return nMatchedNames;
}

/**
 * @brief  
 * @param *tripDate  
 * @param inputTrip 
 * @param BusTrip 
 * @retval 
 */
int
tripFile_GetBusTrip(struct DateDMY *tripDate, TripNo inputTrip, struct Bus16 *BusTrip){
    struct Passenger holder;
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    String15 strTimeOfTrip = "";
    String15 strPriorityNumber = "";
    String15 strIdNumber = "";
    struct TimeHM tempTime;
    int isFileDoesNotExist = FALSE;
    int hasMatchedTrip = FALSE;
    int hasNotFoundEOF = TRUE;
    int ReadingState = FALSE;
    int nLineIgnored = 0;
    int nBusPassenger = 0;
    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;

    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        printErrorMessage("tripFile_GetBusTrip FAIL.\n ERROR: FILE DOES NOT EXIST");
        return EROR_FILE_NOT_FOUND;
    }

    while (hasNotFoundEOF) {
        
        ReadingState = tripFile_GetCurrentPassenger(pFileBusTrip, &holder);

        if (ReadingState == EROR_KEY_NOT_FOUND)
            hasNotFoundEOF = FALSE;
        
        hasMatchedTrip = strcmp(holder.tripNumber, inputTrip) == 0 && hasNotFoundEOF;

        if (hasMatchedTrip) {
            BusTrip->Passengers[nBusPassenger] = holder;
            BusTrip->volume++;
            nBusPassenger++;
        }
    }
    
    return nBusPassenger;
}

/**
 * @brief  
 * @note   
 * @param  *tripDate: 
 * @param  inputTrip: 
 * @param  TripInfo[]: 
 * @param  TripKey: 
 * @return How many passengers are there in the inputted Trip.
 */
int
tripStruct_GetBusTrip(struct DateDMY *tripDate, TripNo inputTrip, struct Bus16 TripInfo[], struct Bus16 *TripKey){
    int i;
    int isTripFound = FALSE;
    int key = 0;
    for(i = 0; i < TOTAL_TRIPS && !isTripFound; i++){
        if(strcmp(TripInfo[i].TripID, inputTrip) == 0){
            isTripFound = TRUE;
            key = i;
        }
    }

    if (isTripFound)
        tripCopy(TripKey, &TripInfo[key]);

    return TripInfo[key].volume;
}

int
tripStruct_ReturnLastname(struct Bus16 BusTrip[], char *LastName, struct SearchResultField *nameResults){
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    struct NameField nameBuffer;
    int hasFullSearches = FALSE;
    int nMatchedNames = 0;
    int i;
    int j;
    // File Handling
    for (i = 0; i < 22 && !hasFullSearches; i++){
        for(j = 0; j < 16 && !hasFullSearches; j++){
            nameBuffer = BusTrip[i].Passengers[j].passengerName;
            GetStringFromNameField(strName, nameBuffer);
            removeNewline(nameBuffer.lastName);

            if (isSubString(LastName, nameBuffer.lastName)){
                strcpy(nameResults->result[nMatchedNames], strName);
                nameResults->passengerIndex[nMatchedNames] = j;
                nameResults->tripNumber[nMatchedNames] = i;
                nMatchedNames++;
            }

            if (nMatchedNames == BUS_SIZE)
                hasFullSearches = TRUE;
        }
    }

    return nMatchedNames;
}

void
printBus16(struct Bus16 Trip){
    int i;
    printf("Trip: \'%s\'\n", Trip.TripID);
    printf("Time: %02d:%02d\n", Trip.timeOfTrip.hour, Trip.timeOfTrip.minute);
    printf("Passengers: %d\n", Trip.volume);
    for(i = 0; i < 16 ;i++){
        printf("Seat: %d", i);
        printPassenger(&Trip.Passengers[i]);
    }
}

void
printPassengerInfo(TripNo inputTripNumber, struct Bus16 *BusTrip, int passengers){
    String255 nameBuffer = "";    
    int i;

    if (passengers > 0){   
        printf("Passengers of %s:\n", inputTripNumber);
        printf("#=>-------------------------- - -\n");
        for(i = 0; i < passengers; i++){
            printf("Y\tName: %s\n", GetStringFromNameField(nameBuffer, BusTrip->Passengers[i].passengerName));
            strcpy(nameBuffer, "");
            printf("|\tIDno: %d\n", BusTrip->Passengers[i].idNumber);
            printf("A\tPriorityNo: %d\n", BusTrip->Passengers[i].priorityNumber);
            printf("#=>-------------------------- - -\n");
        }
    } else {
        printErrorMessage("Trip not yet created.\n");
    }
}

void
printSearchResults(struct SearchResultField *lastNameResults, struct Bus16 BusTrip[], char *nameToSearch){
    int isSearching = TRUE;
    int isQuitingSearch = FALSE;
    int hasChosenBeyondGiven = FALSE;
    int hasChosenAResult = FALSE;
    int tripResultIndex = 0;
    int passResultIndex = 0;
    int i;
    int userChoice;
    struct Passenger ResultingPassenger;
    

    if (!lastNameResults->size) {
        isSearching = FALSE;
        system("cls");
        printf("\nLast name: \"%s\" produces %d result.\n\n", nameToSearch, lastNameResults->size);
    }

    while (lastNameResults->size > 0 && isSearching){
        printGraphics("SearchResult1");

        for(i = 0; i < lastNameResults->size; i++)
            printf("| %02d) Name: \"%s\"\n", i + 1, lastNameResults->result[i]);

        printGraphics("SearchResult2");
        
        repeatGetInteger(&userChoice, "SearchResult3", "\t> Choice: ", "Not a number.");

        isQuitingSearch = userChoice == -1;
        hasChosenBeyondGiven = userChoice > lastNameResults->size;
        hasChosenAResult = userChoice > 0 && userChoice < lastNameResults->size;

        if (isQuitingSearch) {
            isSearching = FALSE;
            system("cls");
        } else if (hasChosenBeyondGiven) {
            system("cls");
            printErrorMessage("Search number is invalid.");
        } else if (hasChosenAResult) {
            tripResultIndex = lastNameResults->tripNumber[userChoice - 1];
            passResultIndex = lastNameResults->passengerIndex[userChoice - 1];
            ResultingPassenger = BusTrip[tripResultIndex].Passengers[passResultIndex];
            system("cls");
            printPassenger(&ResultingPassenger);
        } 
    }
}

void
initializeBusTrip(struct Bus16 Triplist[], int size, struct DateDMY *date, int isStartingFromFile){
    TripNo Codes[22] = {
        "AE101", "AE102", "AE103", "AE104", "AE105",
        "AE106", "AE107", "AE108", "AE109", 
        "AE150", "AE151", "AE152", "AE153", "AE154", 
        "AE155", "AE156", "AE157", "AE158", "AE159", "AE160"
    };
    String7 Time[22] = {
         "6:00",  "7:30",  "9:30", "11:00", "13:00",
        "14:30", "15:30", "17:00", "18:15",
         "5:30",  "5:45",  "7:00",  "7:30",  "9:00",
        "11:00", "13:00", "14:30", "15:30", "17:00", "18:15"
    };
    int i;
    int j;
    for(i = 0; i < 22; i++){
        strcpy(Triplist[i].TripID, "");
        Triplist[i].volume = 0;
        Triplist[i].timeOfTrip.hour = 0;
        Triplist[i].timeOfTrip.minute = 0;
        for (j = 0; j < 16; j++){
            strcpy(Triplist[i].Passengers[j].dropOffPoint, "");
            strcpy(Triplist[i].Passengers[j].embarkationPoint, "");
            strcpy(Triplist[i].Passengers[j].passengerName.lastName, "");
            strcpy(Triplist[i].Passengers[j].passengerName.firstName, "");
            Triplist[i].Passengers[j].passengerName.midI = '\0';
            strcpy(Triplist[i].Passengers[j].tripNumber, "");
            Triplist[i].Passengers[j].idNumber = 0;
            Triplist[i].Passengers[j].priorityNumber = 0;
        }
    }

    if (!isStartingFromFile)
        return;

    for(i = 0; i < 20; i++){
        strcpy(Triplist[i].TripID, Codes[i]);
        Triplist[i].timeOfTrip = TimeHMfromString(Time[i]);
        Triplist[i].volume = tripFile_GetBusTrip(date, Codes[i], &Triplist[i]);
    }
}

void
initializeSearchResult(struct SearchResultField *DropOffResults){
    int i;
    for (i = 0; i < BUS_SIZE; i++){
        strcmp(DropOffResults->result[i], "");
        DropOffResults->passengerIndex[i] = 0;
        DropOffResults->tripNumber[i] = 0;
    }
    DropOffResults->size = 0;
}

struct SearchResultField
countWordFrequency(struct Bus16 BusTrip, int results){
    struct SearchResultField DropOffResults;
    int foundSameDropOff = FALSE;;
    int resultIndex;
    int j;

    initializeSearchResult(&DropOffResults);
    strcpy(DropOffResults.result[0], BusTrip.Passengers[0].dropOffPoint);

    DropOffResults.passengerIndex[0] = 1;
    DropOffResults.size++;

    for (resultIndex = 1; resultIndex < results; resultIndex++){
        foundSameDropOff = FALSE;  

        for (j = 0; j < DropOffResults.size && !foundSameDropOff; j++){
            if (strcmp(DropOffResults.result[j], BusTrip.Passengers[resultIndex].dropOffPoint) == 0){
                foundSameDropOff = TRUE;
                DropOffResults.passengerIndex[j] += 1;
            }
        }

        if (!foundSameDropOff){
            strcpy(DropOffResults.result[DropOffResults.size], BusTrip.Passengers[resultIndex].dropOffPoint);
            DropOffResults.passengerIndex[DropOffResults.size] = 1;
            DropOffResults.size++;
        }
    }
    return DropOffResults;
}


// |===| PASSENGER CMD SECTION |=====================|

void 
userEmbarkation(){
    String63 strFiller = "User creates an embarkation trip.";
    printf("[O] Enter Trip Number: \n");
    printSingleColorText(BACKGROUND_GREEN, strFiller);
    //Get User
}

// |===| ADMIN CMD SECTION ==========================|

void 
adminNoOfPassenger(){
    String63 strFiller = "Admin views a Trip's no. of Passenger";
    printSingleColorText( FG_YELLOW, strFiller);
}

void 
adminCountPassengerDropOff(struct DateDMY *tripDate, struct Bus16 Trips[]){
    String63 strFiller = "Admin counts number of Passenger in a drop-off.";
    TripNo inputTripNumber = ""; 
    
    struct Bus16 pickedTrip;
    struct SearchResultField DropOffResults;
    int isChoosing = TRUE;
    int userChoice;
    int results;
    int i;

    printSingleColorText(FG_YELLOW, strFiller);

    while (isChoosing) {
        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip.");
        results = tripStruct_GetBusTrip(tripDate, inputTripNumber, Trips, &pickedTrip);
        if (strcmp(inputTripNumber, "quit") == 0){
            isChoosing = FALSE;
        } else if (results > 0) {            
            printf("Trip: %s %d\n", inputTripNumber, results);
            DropOffResults = countWordFrequency(pickedTrip, results);
            printf("%d\n", DropOffResults.size);
            printGraphics("DropOff1");
            printf("|  v -- START\n|  |\n");
            for(i = 0; i < DropOffResults.size; i++) {
                printf("| %02d -- Drop Off: %s\n", i + 1, DropOffResults.result[i]);
                printf("|  |      Count: %d\n", DropOffResults.passengerIndex[i]);
                
            }
            printf("|  ^ -- END\n");
            printGraphics("DropOff2");
        }
    }
}

void 
adminViewPassengerInfo(struct DateDMY *tripDate, struct Bus16 BusTrips[]){
    String63 strFiller = "Admin views the passenger info.";
    printSingleColorText( FG_YELLOW, strFiller);
    struct Bus16 BusHolder;
    TripNo inputTripNumber = "";
    int isDoneVieweing = FALSE;
    int passengers;
    
    while (!isDoneVieweing) {
        
        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip. \n\tType \'0\' to exit.");
        if (strcmp(inputTripNumber, "quit") == 0)
            return;
        
        printf("Trip: %s\n", inputTripNumber);    
        passengers = tripStruct_GetBusTrip(tripDate, inputTripNumber, BusTrips, &BusHolder);
        
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
adminSearchPassenger(struct DateDMY *dateToday, struct Bus16 Trips[]){
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
adminEmbarkation(){
    String63 strFiller = "Admin creates an embarkation trip.";
    printSingleColorText( FG_YELLOW, strFiller);

}

// |===| MENU SECTION |=============================|

/**
 * TODO:
 * Detects the current state of the program and picks up where the user left off.
 * > If the passenger is in the middle of something and quits, it will pick it up after. 
 * > If the user is admin, it asks for password and promptly returns back to the starting point.
 * If it cannot detect the corresponding trip file with the time, it asks the user for the date.
 * Keeps TimeHM Updated which restricts users.
 * 
 */

/**
 * TODO fill it functions accessible to a passenger based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 * 
 */
void 
menuPassenger(){
    char inputPassMenu;
    String63 errorMessage = "Dear Passenger, Please select the following valid cmds\n";
    String15 graphicCode = "PassengerMenu";
    String15 strPrompt = "\t> Action: ";
    do {
        repeatGetChar(&inputPassMenu, graphicCode, strPrompt, errorMessage);
        switch(inputPassMenu) {
            case 'a':
                userEmbarkation();
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
void menuAdmin(struct Bus16 BusTrip[], int size, struct DateDMY *date){
    String255 realPass = "Admin123"; // to be changed
    String63 errorMessage = "Please input a valid admin cmd.\n";
    String15 graphicCode = "AdminMenu";
    String15 strPrompt = "\t> Command: ";

    int isChoosingAdminCmds = FALSE;
    int isInputing = TRUE;
    
    char userInput;
    
    while (isInputing && !isChoosingAdminCmds)
        passwordMenu(&isChoosingAdminCmds, &isInputing, realPass);
  
    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, graphicCode, strPrompt, errorMessage);
        switch(userInput){
            case 'a':
                adminNoOfPassenger();
                break;
            case 'b':
                adminCountPassengerDropOff(date, BusTrip);
                break;
            case 'c':
                adminViewPassengerInfo(date, BusTrip);
                break;
            case 'd':
                adminSearchPassenger(date, BusTrip);
                break;
            case 'e':
                adminEmbarkation();
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