#include"utilsBase.h"

// |===| Print Functions |===================================|

void
printPassenger(struct Passenger *Passenger){
    printf("\n#>---<|[>] Result|>--------------#\n");
    printf("Y Trip Number:\t\t %s\n", Passenger->tripNumber);
    printf("| Embarkation Point:\t %s\n", Passenger->embarkationPoint);
    printf("| Passenger Name:\t %s, %s ", Passenger->passengerName.lastName, Passenger->passengerName.firstName);
    if (Passenger->passengerName.midI){
        printf("%c.", Passenger->passengerName.midI);
    }
    printf("\n");
    printf("| ID Number:\t\t %u\n", Passenger->idNumber);
    printf("| Priority Number:\t %u\n", Passenger->priorityNumber);
    printf("A Drop off Point:\t %s\n", Passenger->dropOffPoint);
    printf("#>-------------------------------#\n\n");
}


void
printBus16 (struct Bus16 Trip){
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
printPassengerInfo (TripNo inputTripNumber, struct Bus16 *BusTrip, int passengers){
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
printSearchResults (struct SearchResultField *lastNameResults, struct Bus16 BusTrip[], char *nameToSearch){
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
printTrips (struct Bus16 Trips[]){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int nColor = FG_WHITE;
    int i;
    int j;
    printGraphics("PassEmbark1");
    for(i = 0; i < TOTAL_TRIPS; i++){
        
        if (Trips[i].volume > 10){
            nColor = FG_RED;
        } else if (Trips[i].volume > 5){
            nColor = FG_YELLOW;
        } else if (Trips[i].volume > 0){
            nColor = FG_GREEN;
        } else {
            nColor = FG_WHITE;
        }

        printf("| |    ");
        SetConsoleTextAttribute(hConsoleOutput, nColor);
        printf("%s", Trips[i].TripID);
        SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
        printf("    | ");
        SetConsoleTextAttribute(hConsoleOutput, nColor);
        printf("%02d/%02d", Trips[i].volume, (Trips[i].volume > 13) ? 16 : 13);
        SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
        printf("        | ");
        SetConsoleTextAttribute(hConsoleOutput, nColor);

        if(strcmp(Trips[i].Passengers[0].embarkationPoint, "") != 0){
            printf("%s via %s \n", Trips[i].Passengers[0].embarkationPoint, Trips[i].route);
        } else {
            printf("--- \n");
        }
        SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    }
    printGraphics("PassEmbark2");
}

void
printDropOffs (struct SearchResultField *DropOffResults){
    int i;
    printGraphics("DropOff2");
    printf("|  Y--- START\n|  |\n");
    for(i = 0; i < DropOffResults->size; i++) {
        printf("| %02d -- Drop Off: %s\n", i + 1, DropOffResults->result[i]);
        printf("|  |      Count: %d\n", DropOffResults->passengerIndex[i]);
    }
    printf("|  A--- END\n");
}

void 
printSeats13(int passCount) {  
    char seats13[5][3];
    
    int i, j;

    if (passCount == 0) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                seats13[i][j] = 'O';
            }
        }

        seats13[4][1] = ' ';
        seats13[4][2] = ' ';

        printf("\n13 - Seater Bus: All Vacant\n");
        printf("#-----------#\n");
        printf("|           |\n");
        
        
        for (i = 0; i < 5; i++) {
            printf("|  ");
            for (j = 0; j < 3; j++) {
                printf("%c  ", seats13[i][j]);
            }
            printf("|\n");
        }
        printf("|           |\n");
        printf("#-----------#\n");
    } else if (passCount > 0 && passCount < 14) {
        for (i = 1; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                seats13[i][j] = 'O';
            }
        }
        
        for (i = 0; i < passCount; i++) {
            seats13[0][i] = 'X';
        }

        seats13[4][1] = ' ';
        seats13[4][2] = ' ';

        printf("\n13 - Seater Bus: %d Passenger/s\n", passCount);
        printf("#-----------#\n");
        printf("|           |\n");
        
        
        for (i = 0; i < 5; i++) {
            printf("|  ");
            for (j = 0; j < 3; j++) {
                printf("%c  ", seats13[i][j]);
            }
            printf("|\n");
        }
        printf("|           |\n");
        printf("#-----------#\n");
    } else if (passCount != 0 || passCount > 13)
        repeatGetInteger(&passCount, "None", "Please Enter a seat", "Error, not a Integer");
}

void 
printSeats16(int passCount) { 
    char seats16[4][4];
    int i, j;

    if (passCount == 0) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                seats16[i][j] = 'O';
            }
        }

        seats16[3][2] = ' ';
        seats16[3][3] = ' ';

        printf("\n16 - Seater Bus: All Vacant\n");
        printf("#-----------#\n");
        printf("|           |\n");
        
        for (i = 0; i < 4; i++) {
            printf("|  ");
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("|\n");
        }
        printf("|           |\n");
        printf("#-----------#\n");
    } else if (passCount > 0 && passCount < 17) {
        
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                seats16[i][j] = 'O';
            }
        }
        
        for (i = 0; i < passCount; i++) {
            seats16[0][i] = 'X';
        }

        seats16[3][2] = ' ';
        seats16[3][3] = ' ';

        printf("\n16 - Seater Bus: %d Passenger/s\n", passCount);
        printf("#-----------#\n");
        printf("|           |\n");
        
        for (i = 0; i < 4; i++) {
            printf("|  ");
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("|\n");
        }
        printf("|           |\n");
        printf("#-----------#\n");

    } else if (passCount != 0 || passCount > 16)
        repeatGetInteger(&passCount, "None", "Please Enter a seat", "Error, not a Integer");
}


int
containsDropOffPointList (struct dropOffPointList *input, char *dropOff){
    int i;
    for(i = 0; i < input->size; i++){
        if (strcmp(dropOff, input->dropOffs[i]) == 0)
            return TRUE;
    }
    return FALSE;
}


// |===| Trip File Functions |=============================|

/**
 * @brief  
 * @note   
 * @param  TripNumber: 
 * @param  *EmbarkationPoint: 
 * @param  *PassengerName: 
 * @param  idNumber: 
 * @param  priorityNumber: 
 * @param  date: 
 * @param  time: 
 * @param  *dropOffPoint: 
 * @return
 */
ErrorInt 
tripFile_PassengerAppender (struct DateDMY tripDate, struct Passenger *newPassenger){
    FILE *pFileBusTrip;
    String15 fileName = "";
    String15 strTripOfDate = "";
    int isFileDoesNotExist = FALSE;

    StringfromDateDMY(fileName, &tripDate, TRUE);

    printf("Date: %s\n", fileName);
    pFileBusTrip = fopen(fileName, "a");
    
    isFileDoesNotExist = pFileBusTrip == NULL;
    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        return EROR_FILE_NOT_FOUND;
    }

    // Actual Passenger Append
    fprintf(pFileBusTrip, "%s\n", newPassenger->tripNumber);
    fprintf(pFileBusTrip, "%s\n", newPassenger->embarkationPoint);
    fprintf(pFileBusTrip, "%s\n", newPassenger->passengerName);
    fprintf(pFileBusTrip, "%u\n", newPassenger->idNumber);
    fprintf(pFileBusTrip, "%u\n", newPassenger->priorityNumber);
    fprintf(pFileBusTrip, "%s\n", newPassenger->dropOffPoint);
    
    fclose(pFileBusTrip);
    
    return PROG_SUCCESS;
}

ErrorInt
tripFile_GetCurrentPassenger (FILE * pFileBusTrip, struct Passenger *keyPassenger){
    String255 strName = "";
    String15 strDateOfTrip = "";
    String15 strTimeOfTrip = "";
    String15 strPriorityNumber = "";
    String15 strIdNumber = "";
    if (feof(pFileBusTrip)) {
        fclose(pFileBusTrip);
        return EROR_KEY_NOT_FOUND;
    };
    fgets(keyPassenger->tripNumber, 255, pFileBusTrip);
    fgets(keyPassenger->embarkationPoint, 255, pFileBusTrip);
    fgets(strName, 255, pFileBusTrip);
    fgets(strIdNumber, 255, pFileBusTrip);
    fgets(strPriorityNumber, 255, pFileBusTrip);
    fgets(keyPassenger->dropOffPoint, 255, pFileBusTrip);
    
    // Clean the Key's details;
    removeNewline(keyPassenger->tripNumber);
    removeNewline(keyPassenger->embarkationPoint);
    removeNewline(strName);
    removeNewline(strIdNumber);
    removeNewline(strPriorityNumber);
    removeNewline(keyPassenger->dropOffPoint);
    
    keyPassenger->passengerName = GetNameFromString(strName);
    keyPassenger->idNumber = atoi(strIdNumber);
    keyPassenger->priorityNumber = atoi(strPriorityNumber);
    return PROG_SUCCESS;
}

ErrorInt 
tripFile_ReturnPassenger (struct DateDMY *tripDate, struct Passenger *keyPassenger, int key){
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    
    String15 fileName = "";

    struct TimeHM tempTime;
    ErrorInt nIndex = -1;
    int isFileDoesNotExist = 0;
    int line;
    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;

    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        return EROR_FILE_NOT_FOUND;
    }

    // Skip Unnecessary Lines;
    for(line = 0; line < key * 7 ; line++)
        if(fgets(temporaryBuffer, 255, pFileBusTrip) == NULL) {
            fclose(pFileBusTrip);
            printErrorMessage("EOF REACHED! Search others.");
            return EROR_KEY_NOT_FOUND;
        }
    
    // Update Key;
    ErrorInt isSuccessfulRead = tripFile_GetCurrentPassenger(pFileBusTrip, keyPassenger);
    if (isSuccessfulRead = PROG_SUCCESS){
        fclose(pFileBusTrip);
        return PROG_SUCCESS;
    }

    return EROR_KEY_NOT_FOUND;
}

int
tripFile_SearchSameTrip (struct DateDMY *tripDate, TripNo tripNumber, struct Bus16 BusOfTrip){
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
tripFile_ReturnLastname (struct DateDMY *tripDate, struct Bus16 BusTrip, char *LastName, struct SearchResultField *nameResults){
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

int
tripFileSearch_PassengerFull (struct DateDMY *tripDate, struct Passenger *keyPassenger){
    int i;
    int isFound = 0;
    int key = -1;
    for(i = 0; i < 16 && !isFound; i++){
        isFound = tripFile_ReturnPassenger(tripDate, keyPassenger, i) == 0 ? TRUE : -1;
        if (isFound > 0)
            key = i;
    }
    return key;
}

/**
 * @brief  
 * @param *tripDate Date Struct of the Trip.
 * @param inputTrip A String of the Trip Number used to identify the trip. 
 * @param BusTrip A struct Bus16 pointer to the array of Bustrips.
 * @return Number of passengers in the trip.
 */
int
tripFile_GetBusTrip (struct DateDMY *tripDate, TripNo inputTrip, struct Bus16 *BusTrip, struct dropOffPointList *exits){
    struct Passenger holder;
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    int i;
    struct TimeHM tempTime;
    int isFileDoesNotExist = FALSE;
    int hasMatchedTrip = FALSE;
    int hasNotFoundEOF = TRUE;
    int ReadingState = FALSE;
    int nLineIgnored = 0;
    int nBusPassenger = 0;
    int nFoundDropOff = 0;

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

        if (ReadingState == EROR_KEY_NOT_FOUND){
            hasNotFoundEOF = FALSE;
            return nBusPassenger;
        }
        
        hasMatchedTrip = strcmp(holder.tripNumber, inputTrip) == 0 && hasNotFoundEOF;

        if (hasMatchedTrip && nBusPassenger == 0) {
        	nFoundDropOff = 0;
            for(i = 0; i < 4 && !nFoundDropOff; i++){
                if (containsDropOffPointList(exits + i, holder.dropOffPoint)){
                	strcpy(BusTrip->route, exits[i].route);
                	nFoundDropOff = 1;
				}
            }
        }
        
        if (hasMatchedTrip) {
            BusTrip->Passengers[nBusPassenger] = holder;
            BusTrip->volume++;
            nBusPassenger++;
        }
    }
    
    return nBusPassenger;
}


void
tripCopy(struct Bus16 *Destination, struct Bus16 *Source){
    int i;
    for(i = 0; i < 16; i++)
        Destination->Passengers[i] = Source->Passengers[i];

    strcpy(Destination->TripID, Source->TripID);
    Destination->timeOfTrip = Source->timeOfTrip;
    Destination->volume = Source->volume;
}


// |===| Trip Struct Functions |=============================|

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
tripStruct_GetBusTrip (TripNo inputTrip, struct Bus16 TripInfo[], struct Bus16 *TripKey, int *keyLoc){
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

    *keyLoc = key;
    
    return TripInfo[key].volume;
}

int
tripStruct_ReturnLastname (struct Bus16 BusTrip[], char *LastName, struct SearchResultField *nameResults){
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

// |===| Copy functions |====================================|

/**
 * @brief  
 * @note   
 * @param *input: 
 * @retval None
 */
void 
initializeDropOffPointList (struct dropOffPointList *input){
    String15 strExitRoutes[4] = {
        "Mamplasan Exit",
        "ETON Exit",
        "Estrada",
        "Buendia/LRT"
    };
    String127 strDropOffs[4][4] = {
        {
            "1st drop-off point - Mamplasan Toll Exit", 
            "2nd drop-off point - Phase 5, San Jose Village", 
            "3rd drop-off point - Milagros Del Rosario (MRR) Building - East Canopy"
        }, {
            "1st drop-off point - Laguna Blvd. Guard House (across Paseo PHOENIX Gasoline Station)", 
            "2nd drop-off point - Milagros Del Rosario (MRR) Building - East Canopy"
        }, {
            "1st drop-off point - Petron Gasoline Station along Gil Puyat Avenue",
            "2nd drop-off point - Gate 4: Gokongwei Gate",
            "3rd drop-off point - Gate 2: North Gate (HSSH)",
            "4th drop-off point - Gate 1: South Gate (LS Building Entrance)"
        }, {
            "2nd drop-off point - College of St. Benilde (CSB) along Taft Avenue",
            "3rd drop-off point - Gate 4: Gokongwei Gate",
            "4th drop-off point - Gate 2: North Gate (HSSH)",
            "5th drop-off point - Gate 1: South Gate (LS Building Entrance)"
        }
    };
    int dropOffSizes[4] = { 3, 2, 4, 4 };
    int i;
    int j;

    
    for(i = 0; i < 4; i++){
        for (j = 0; j < dropOffSizes[i]; j++){
            strcpy((input+i)->dropOffs[j], strDropOffs[i][j]);
        }
        strcpy((input+i)->route, strExitRoutes[i]);
        (input+i)->size = dropOffSizes[i];
    }
}

void 
passwordMenu (int *isChoosingAdminCmds, int *isInputingPass, char *realPass){
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

void
initializeBusTrip (struct Bus16 Triplist[], int size, struct DateDMY *date, int isStartingFromFile, struct dropOffPointList *exits){
    int returnedPassengers = 0;
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
        strcpy(Triplist[i].TripID, "--000");
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

    for(i = 0; i < 20; i++){
        strcpy(Triplist[i].TripID, Codes[i]);
        Triplist[i].timeOfTrip = TimeHMfromString(Time[i]);
        if (isStartingFromFile)
            returnedPassengers = tripFile_GetBusTrip(date, Codes[i], &Triplist[i], exits);

        if (isStartingFromFile && returnedPassengers > 0){
            Triplist[i].volume = returnedPassengers;
        } else {
            strcpy(Triplist[i].TripID, "--000");
            Triplist[i].volume = 0;
        }
    }
}

void
initializeSearchResult (struct SearchResultField *DropOffResults){
    int i;
    for (i = 0; i < BUS_SIZE; i++){
        strcmp(DropOffResults->result[i], "");
        DropOffResults->passengerIndex[i] = 0;
        DropOffResults->tripNumber[i] = 0;
    }
    DropOffResults->size = 0;
}

struct SearchResultField
countWordFrequency (struct Bus16 BusTrip, int results){
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

        if (!foundSameDropOff) {
            strcpy(DropOffResults.result[DropOffResults.size], BusTrip.Passengers[resultIndex].dropOffPoint);
            DropOffResults.passengerIndex[DropOffResults.size] = 1;
            DropOffResults.size++;
        }
    }
    return DropOffResults;
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