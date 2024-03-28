#include"utilsBase.h"

// |===| Print Functions |===================================|

void
printPassenger(struct Passenger *Passenger){
    printf("\n#>---<|[>] Result >--------------------------------------------#\n");
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
    printf("#>--------------------------------------------------------------#\n\n");
}


void
printBus16 (struct Bus16 Trip){
    int i;
    printf("Trip:              \'%s\'\n", Trip.TripID);
    printf("Time:               %02d:%02d\n", Trip.timeOfTrip.hour, Trip.timeOfTrip.minute);
    printf("Passengers:         %d\n", Trip.volume);
    printf("Embarkation Point:  %s\n\n", Trip.embarkationPoint);
    for(i = 0; i < Trip.volume ;i++){
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
        printf("#=>--------------------------- - -\n");
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
embarkationCodeEquivalent(char * embarkationPoint, char *outputCode){
    if (strcmp(embarkationPoint, "DLSU Manila Campus - South Gate Driveway") == 0){
        strcpy(outputCode, "MNL-LAG");
    } else {
        strcpy(outputCode, "LAG-MNL");
    }
};

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
        hasChosenAResult = userChoice > 0 && userChoice <= lastNameResults->size;
        printf("Result: %d %d", userChoice, hasChosenAResult);
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
    String15 stringEmbarkCode = "";
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

        printf("    %02d/%02d", Trips[i].volume, (Trips[i].volume > 13) ? 16 : 13);
        SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
        printf("    | ");

        SetConsoleTextAttribute(hConsoleOutput, nColor);
        embarkationCodeEquivalent(Trips[i].embarkationPoint, stringEmbarkCode);
        printf("%s ", stringEmbarkCode);
        printf("%s\n", Trips[i].route);
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
tripFile_GetBusTrip2 (struct DateDMY *tripDate, TripNo inputTrip, struct Bus16 *BusTrip, struct dropOffPointList *exits){
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


int
findIndexOfTripNo(char *inputTripNo){
    TripNo Codes[TOTAL_TRIPS] = {
        "AE101", "AE102", "AE103", "AE104", "AE105",
        "AE106", "AE107", "AE108", "AE109", 
        "AE150", "AE151", "AE152", "AE153", "AE154", 
        "AE155", "AE156", "AE157", "AE158", "AE159", "AE160", 
        "SP101", "SP102"
    };
    int i;

    for (i = 0; i < TOTAL_TRIPS; i++) {
        if (strcmp(Codes[i], inputTripNo) == 0) {
            return i;
        }
    }
    return -1;
}

ErrorInt
findTimeOfTripNo(char *inputTripNo, char *strOutputTime){
    TripNo Codes[TOTAL_TRIPS] = {
        "AE101", "AE102", "AE103", "AE104", "AE105",
        "AE106", "AE107", "AE108", "AE109", 
        "AE150", "AE151", "AE152", "AE153", "AE154", 
        "AE155", "AE156", "AE157", "AE158", "AE159", "AE160",
        "SP101", "SP102"
    };

    String7 Time[TOTAL_TRIPS] = {
         "6:00",  "7:30",  "9:30", "11:00", "13:00",
        "14:30", "15:30", "17:00", "18:15",
         "5:30",  "5:45",  "7:00",  "7:30",  "9:00",
        "11:00", "13:00", "14:30", "15:30", "17:00", "18:15",
        "00:00", "00:00"
    };

    int i;

    for (i = 0; i < TOTAL_TRIPS; i++) {
        if (strcmp(Codes[i], inputTripNo) == 0) {
            strcpy(strOutputTime, Time[i]);
            return PROG_SUCCESS;
        }
    }

    return EROR_KEY_NOT_FOUND;
}

/***/
int
tripFile_GetBusTrip(struct DateDMY *tripDate, struct Bus16 BusTrip[], struct dropOffPointList *exits){
    struct Passenger    tempPassenger;
    struct TimeHM       tempTime;
    FILE *pFileBusTrip;
    String255 temporaryBuffer[3] = {""};
    String15 fileName   = "";
    String15 timeOfTrip = "";

    int isFileDoesNotExist      = FALSE;
    int hasMatchedTrip          = FALSE;
    int hasNotFoundEOF          = TRUE;
    int ReadingState            = FALSE;
    int hasBusPreviousBusTrip   = FALSE;

    int nLineIgnored    = 0;
    int nBusPassenger   = 0;
    int nFoundDropOff   = 0;
    int tripIndex       = 0;
    int i;

    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    printf("%s", fileName);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;

    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        printErrorMessage("tripFile_GetBusTrip2 FAIL.\n ERROR: FILE DOES NOT EXIST");
        return EROR_FILE_NOT_FOUND;
    }

    for (i = 0; i < 2; i++) {
            fgets(temporaryBuffer[i], 255, pFileBusTrip);
            removeNewline(temporaryBuffer[i]);
            printf("%s\n", temporaryBuffer[i]);
    }

    while (!feof(pFileBusTrip)) {
        // Check if its a new Trip
        if (isStringTripNo(temporaryBuffer[0])) {
            tripIndex = findIndexOfTripNo(temporaryBuffer[0]);
            findTimeOfTripNo(temporaryBuffer[0], timeOfTrip);
            strcpy(BusTrip[tripIndex].TripID, temporaryBuffer[0]);
            strcpy(BusTrip[tripIndex].embarkationPoint, temporaryBuffer[1]);
            BusTrip[tripIndex].timeOfTrip = TimeHMfromString(timeOfTrip);
            BusTrip[tripIndex].volume = 0;       
        // Else Assume its a passenger. 
        } else {
            strcpy(tempPassenger.embarkationPoint, BusTrip[tripIndex].embarkationPoint);
            strcpy(tempPassenger.tripNumber, BusTrip[tripIndex].TripID);
            tempPassenger.passengerName = GetNameFromString(temporaryBuffer[0]);
            tempPassenger.idNumber = atoi(temporaryBuffer[1]);

            for (i = 0; i < 3; i++) {
                fgets(temporaryBuffer[i], 255, pFileBusTrip);
                removeNewline(temporaryBuffer[i]);
            }

            tempPassenger.priorityNumber = atoi(temporaryBuffer[0]);
            strcpy(tempPassenger.dropOffPoint, temporaryBuffer[1]);

            BusTrip[tripIndex].Passengers[BusTrip[tripIndex].volume] = tempPassenger;
            BusTrip[tripIndex].volume++;
        }
        // Move forward by two
        for (i = 0; i < 2; i++){
            fgets(temporaryBuffer[i], 255, pFileBusTrip);
            removeNewline(temporaryBuffer[i]);
        }
        
    }

    fclose(pFileBusTrip);
    return 0;
}

void
tripFile_WriteBusTrip(struct DateDMY *tripDate, struct Bus16 BusTrip[]){
    FILE *busTripFile;
    String15 fileName = "";
    String127 passengerName = "";
    int i, j;

    StringfromDateDMY(fileName, tripDate, TRUE);

    busTripFile = fopen(fileName, "w");

    if (busTripFile == NULL){
        printErrorMessage("File writing error! Check if filename is available to use.");
        fclose(busTripFile);
    }

    for(i = 0; i < TOTAL_TRIPS; i++){
        if (BusTrip[i].volume > 0) {
            fprintf(busTripFile, "%s\n", BusTrip[i].TripID);
            fprintf(busTripFile, "%s\n", BusTrip[i].embarkationPoint);
            for (j = 0; j < BusTrip[i].volume; j++){
                GetStringFromNameField(passengerName, BusTrip[i].Passengers[j].passengerName);
                fprintf(busTripFile, "%s\n", passengerName);
                fprintf(busTripFile, "%d\n", BusTrip[i].Passengers[j].idNumber);
                fprintf(busTripFile, "%d\n", BusTrip[i].Passengers[j].priorityNumber);
                fprintf(busTripFile, "%s\n\n", BusTrip[i].Passengers[j].dropOffPoint);
            }
        }
    }

    fclose(busTripFile);
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
tripStruct_SearchBusTrip (TripNo inputTrip, struct Bus16 TripInfo[], struct Bus16 *TripKey, int *keyLoc){
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
    for (i = 0; i < TOTAL_TRIPS && !hasFullSearches; i++){
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
            "1st drop-off point - College of St. Benilde (CSB) along Taft Avenue",
            "2nd drop-off point - Gate 4: Gokongwei Gate",
            "3rd drop-off point - Gate 2: North Gate (HSSH)",
            "4th drop-off point - Gate 1: South Gate (LS Building Entrance)"
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
    TripNo Codes[TOTAL_TRIPS] = {
        "AE101", "AE102", "AE103", "AE104", "AE105",
        "AE106", "AE107", "AE108", "AE109", 
        "AE150", "AE151", "AE152", "AE153", "AE154", 
        "AE155", "AE156", "AE157", "AE158", "AE159", "AE160",
        "SP101", "SP150",
    };
    
    String7 strTime = "";
    int returnedPassengers = 0;
    int i;
    int j;

    for(i = 0; i < TOTAL_TRIPS; i++){
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

    
    
    for(i = 0; i < TOTAL_TRIPS; i++){
        strcpy(Triplist[i].TripID, Codes[i]);
        findTimeOfTripNo(Triplist[i].TripID, strTime);
        Triplist[i].timeOfTrip = TimeHMfromString(strTime);
        
        if (i < 9 || i == 20)
            strcpy(Triplist[i].embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
        else if (i < 20 || i == 21)
            strcpy(Triplist[i].embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy");
    
        if (i == 20 || i < 9 && i % 2 == 0)
            strcpy(Triplist[i].route, "Mamplasan Exit");
        else if (i < 9 && i % 2 == 1)
            strcpy(Triplist[i].route, "ETON Exit");
        else if (i == 21 || i < 20 && i % 2 == 0)
            strcpy(Triplist[i].route, "Estrada");
        else if (i < 20 && i % 2 == 1)
            strcpy(Triplist[i].route, "Buendia/LRT");

        Triplist[i].volume = 0;
    }

    if (isStartingFromFile) {
        returnedPassengers = tripFile_GetBusTrip(date, Triplist, exits);
        printBus16(*Triplist);
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
countWordFrequency (struct Bus16 BusTrip, int returnedPassengers){
    struct SearchResultField DropOffResults;
    String255 strTemp;
    int foundSameDropOff = FALSE;;
    int nPass;
    int nResult;
    int max;
    int nTemp;
    int i;
    int j;

    initializeSearchResult(&DropOffResults);
    strcpy(DropOffResults.result[0], BusTrip.Passengers[0].dropOffPoint);

    DropOffResults.passengerIndex[0] = 1;
    DropOffResults.size++;

    for (nPass = 1; nPass < returnedPassengers; nPass++){
        foundSameDropOff = FALSE;  

        for (nResult = 0; nResult < DropOffResults.size && !foundSameDropOff; nResult++){
            // is ALREADY ENCOUNTERED?
            if (strcmp(DropOffResults.result[nResult], BusTrip.Passengers[nPass].dropOffPoint) == 0){
                foundSameDropOff = TRUE;
                DropOffResults.passengerIndex[nResult] += 1;
            }
        }

        if (!foundSameDropOff) {
            strcpy(DropOffResults.result[DropOffResults.size], BusTrip.Passengers[nPass].dropOffPoint);
            DropOffResults.passengerIndex[DropOffResults.size] = 1;
            DropOffResults.size++;
        }
    }
    // SORTING ALGORITHM for Search Results
    for(i = 0; i < DropOffResults.size - 1; i++){
        max = i;
        for(j = 1; j < DropOffResults.size; j++){
            // Since ASCII value of '2' > '1', then STRING of max > STRING of j, then strcmp > 0. 
            // All drop-offs start with a number, so arrange increasingly to align.
            if (strcmp(DropOffResults.result[max], DropOffResults.result[j]) > 0)
                max = j;
        }
        // SWAP CODE
        if (max != j){
            strcpy(strTemp, DropOffResults.result[i]);
            strcpy(DropOffResults.result[i], DropOffResults.result[max]);
            strcpy(DropOffResults.result[max], strTemp);

            nTemp = DropOffResults.passengerIndex[i];
            DropOffResults.passengerIndex[i] = DropOffResults.passengerIndex[max];
            DropOffResults.passengerIndex[max] = nTemp;
        }
    }
    return DropOffResults;
}

void
repeatGetPassenger(struct Passenger *pInput, struct Bus16 TripDatabase[], struct dropOffPointList *exits){
    String63 validationErrorMessage = "Please enter either Y or N only.";
    struct Passenger holder;
    String15 Route = "";
    int embarkationNum;
    int routeNum;
    int isValidRanges;
    int dropOffNum;
    int isValidatingAll = TRUE;
    int isValidDetail[5] = {FALSE};
    char userValidation;
    int isConfirmed = FALSE;
    int temp;

    String255 strDropOffs[13] = {
        "1st drop-off point - Mamplasan Toll Exit", 
        "2nd drop-off point - Phase 5, San Jose Village", 
        "3rd drop-off point - Milagros Del Rosario (MRR) Building - East Canopy",
        "1st drop-off point - Laguna Blvd. Guard House (across Paseo PHOENIX Gasoline Station)", 
        "2nd drop-off point - Milagros Del Rosario (MRR) Building - East Canopy",
        "1st drop-off point - Petron Gasoline Station along Gil Puyat Avenue",
        "2nd drop-off point - Gate 4: Gokongwei Gate",
        "3rd drop-off point - Gate 2: North Gate (HSSH)",
        "4th drop-off point - Gate 1: South Gate (LS Building Entrance)",
        "1st drop-off point - College of St. Benilde (CSB) along Taft Avenue",
        "2nd drop-off point - Gate 4: Gokongwei Gate",
        "3rd drop-off point - Gate 2: North Gate (HSSH)",
        "4th drop-off point - Gate 1: South Gate (LS Building Entrance)"
    };

    while(isValidatingAll){
        
        while (!isValidDetail[0]){
            printTrips(TripDatabase);
            repeatGetTripNo(holder.tripNumber, "PassEmbark3", "\t> Please Input Trip No: ", "Enter a valid one.");
            system("cls");
            
            isConfirmed = FALSE;
            while(!isConfirmed){
                printf("Is %s correct?\n", holder.tripNumber);
                validateUserInput(&isConfirmed, &isValidDetail[0], validationErrorMessage);
            }
        }

        while (!isValidDetail[1]){
            isConfirmed = FALSE;
            repeatGetString(holder.passengerName.firstName, 63, "PassEmbark4-1", "\t> First Name ", "Please enter a valid last name within 64 characters.");
            system("cls");
            repeatGetString(holder.passengerName.lastName, 63, "PassEmbark4-2", "\t> Last Name ", "Please enter a valid first name within 64 characters.");
            system("cls");
            repeatGetChar(&holder.passengerName.midI, "PassEmbark4-3", "\t> Middle Initial ", "Please enter one valid character.");
            system("cls");

            
            while (!isConfirmed) {
                printGraphics("PassEmbark4-4");
                printf("Is %s, %s %c. correct?\n", holder.passengerName.lastName, holder.passengerName.firstName, holder.passengerName.midI);
                validateUserInput(&isConfirmed, &isValidDetail[1], validationErrorMessage);
            }
        }

        while (!isValidDetail[2]){
            isConfirmed = FALSE;
            repeatGetInteger(&holder.idNumber, "PassEmbark5", "\t> Pass Ranking: ", "Please enter a valid integer.");
            system("cls");
            
            if (holder.idNumber <= 0){
                printErrorMessage("Not a valid ID Number.");
            }
            
            while (!isConfirmed && holder.idNumber > 0) {
                printGraphics("PassEmbark5");
                printf("Is %d correct?\n", holder.idNumber);
                validateUserInput(&isConfirmed, &isValidDetail[2], validationErrorMessage);
            }
        }

        while (!isValidDetail[3]){
            isConfirmed = FALSE;
            repeatGetInteger(&holder.priorityNumber, "PassEmbark6", "\t> Pass Ranking: ", "Please enter a valid integer.");
            system("cls");
            if (holder.priorityNumber <= 0 && holder.priorityNumber > 6){
                printErrorMessage("Not a valid priority number.");
            }
 
            while (!isConfirmed && holder.priorityNumber > 0 && holder.priorityNumber <= 6) {
                printGraphics("PassEmbark6");
                printf("Is %d correct?\n", holder.priorityNumber);
                validateUserInput(&isConfirmed, &isValidDetail[3], validationErrorMessage);
            }
        }

        while (!isValidDetail[4]){
            isConfirmed = FALSE;
            isValidRanges = TRUE;
            system("cls");
            repeatGetInteger(&embarkationNum, "PassEmbark7", "\t Embarkation Point: ", "Please choose the number corresponding \n to where you are now.");

            if (embarkationNum < 0 || embarkationNum > 1) {
                printErrorMessage("Not a valid Embarkation selection.");
                isValidRanges = FALSE;
            } else {
                if (embarkationNum == 0) 
                    strcpy(holder.embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
                else
                    strcpy(holder.embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy");
            }

            if (isValidRanges) {
                system("cls");
                repeatGetInteger(&routeNum, "PassEmbark8", "\t Embarkation Point: ", "Please choose the number corresponding to the route you want.");

                if (routeNum < 0 || routeNum > 3) {
                    printErrorMessage("Not a valid Embarkation selection.");
                    isValidRanges = FALSE;
                } else {
                    strcpy(holder.route, exits[routeNum].route);
                }
            }

            if (isValidRanges) {
                system("cls");
                repeatGetInteger(&dropOffNum, "PassEmbark9", "\t Route: ", "Please choose which drop off you want.");

                if (dropOffNum < 0 || dropOffNum >= 13) {
                    printErrorMessage("Not a valid Drop Off selection.");
                    isValidRanges = FALSE;
                } else {
                    strcpy(holder.dropOffPoint, strDropOffs[dropOffNum]);
                }
            }

            while (!isConfirmed && isValidRanges) {
                printf("\nCheck if Correct:\n");
                printf("Embarkation Point: \t%s\nDrop point: \t\t%s\nRoute: \t\t\t%s\n\n", holder.embarkationPoint, holder.dropOffPoint, holder.route);
                validateUserInput(&isConfirmed, &isValidDetail[4], validationErrorMessage);
            }
        }
        
        isConfirmed = FALSE;
        while (!isConfirmed){
            printPassenger(&holder);
            validateUserInput(&isConfirmed, &isValidatingAll, validationErrorMessage);

            if (isConfirmed) {
                isValidDetail[0] = FALSE;
                isValidDetail[1] = FALSE;
                isValidDetail[2] = FALSE;
                isValidDetail[3] = FALSE;
                isValidDetail[4] = FALSE;
            }
        }   
    }
    *pInput = holder;
    return;
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
