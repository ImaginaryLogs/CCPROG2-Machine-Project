#include"utilsBase.h"

/**
 * @brief Finds the embarkation code equivalent
 * @param *embarkationPoint: String of the embarkationPoint needed to be analyzed.
 * @param *outputCode: String buffer of the wanted embarkation code
 * @return
 */
void
embarkationCodeEquivalent(char *embarkationPoint, char *outputCode){
    if (strcmp(embarkationPoint, "DLSU Manila Campus - South Gate Driveway") == 0){
        strcpy(outputCode, "MNL-LAG");
    } else {
        strcpy(outputCode, "LAG-MNL");
    }
};

// |===| Print Functions |===================================|

/**
 * @brief Prints passenger information.
 * @param *Passenger: points to a passenger struct to print.
 * @return void
 */
void
printPassenger(struct Passenger *Passenger){
    String127 Name = "";
    printf("\n#>---<|[>] Result >----------------------------------------------------#\n");
    printf("Y\n");
    printf("|   Trip Number:\t\t %s\n", Passenger->tripNumber);
    printf("|   Embarkation Point:\t %s\n", Passenger->embarkationPoint);
    printf("|   Passenger Name:\t %s\n", GetStringFromNameField(Name, Passenger->passengerName));
    printf("|   ID Number:\t\t %u\n", Passenger->idNumber);
    printf("|   Priority Number:\t %u\n", Passenger->priorityNumber);
    printf("|   Drop off Point:\t %s\n", Passenger->dropOffPoint);
    printf("A\n");
    printf("#>----------------------------------------------------------------------#\n\n");
}

/**
 * @brief Prints information on Bus16. 
 * @param Trip: a Bus16 struct of the Trip to print
 * @return void 
 */
void
printBus16 (struct Bus16 Trip){
    int i;
    printf("#>-<|[>] Trip information |>-------------------------------------------#\n");
    printf("Y\n");
    printf("|   Trip ID:\'%s\'\n", Trip.TripID);
    printf("|   Embarkation Point:\t%s\n\n", Trip.embarkationPoint);
    printf("|   Time of Departure:\t%02d:%02d\n", Trip.timeOfTrip.hour, Trip.timeOfTrip.minute);
    printf("|   No. of Passengers:\t%d\n", Trip.volume);
    printf("A\n");
    printf("#>---------------------------------------------------------------------#\n");
    for(i = 0; i < Trip.volume ;i++){
        printf("Seat: %d\n", i);
        printPassenger(&Trip.Passengers[i]);
    }
}

/**
 * @brief Prints a list of passenger
 * @param inputTripNumber: a string containing the trip number.
 * @param *BusTrip: a pointer to the Bus16 struct of the Trip to print.
 */
void
printListofPassenger(TripNo inputTripNumber, struct Bus16 *BusTrip){
	String31 NoFileErrMessage = "Trip not yet created.\n";
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    struct Bus16        sortedBusTrip;
    struct Passenger    tempHolder;
    String255 nameBuffer = "";
    int nColor = 0;  
    int i;
    int j;
    int min;

    sortedBusTrip = *BusTrip;
    if (BusTrip->volume == 0) {
        printErrorMessage(NoFileErrMessage);
        return;
    }

    // SORTING ALGORITHM
    for(i = 0; i < BusTrip->volume - 1; i++){
        min = i;
        
        for (j = i; j < BusTrip->volume; j++){
            if (sortedBusTrip.Passengers[j].priorityNumber < sortedBusTrip.Passengers[min].priorityNumber){
                min = j;
            }
        }

        if (min != j){
            tempHolder = sortedBusTrip.Passengers[j];
            sortedBusTrip.Passengers[i] = sortedBusTrip.Passengers[min];
            sortedBusTrip.Passengers[min] = tempHolder;
        }
    }

    // PRINTS INFORMATION
    SetConsoleTextAttribute(hConsoleOutput, FG_GREEN);
    printf("    Passengers of Trip %s:\n", BusTrip->TripID);
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE);
    printf("    #>-No-<#> Priority No.<#> ID  Number <#> Name -~-~-~-~-----------------------<#\n");
    printf("    Y      Y               Y              Y\n");
    for (i = 0; i < BusTrip->volume; i++){
        switch(BusTrip->Passengers[i].priorityNumber) {
            case '1':
                nColor = FG_RED | FG_INTEN;
                break;
            case '2':
                nColor = FG_RED;
                break;
            case '3':
                nColor = FG_YELLOW | FG_INTEN;
                break;
            case '4':
                nColor = FG_YELLOW;
                break;
            case '5':
                nColor = FG_GREEN | FG_INTEN;
                break;
            case '6':
                nColor = FG_GREEN;
                break;
        }
        printf("    |");
        SetConsoleTextAttribute(hConsoleOutput, nColor);
        printf("  %02d  ", i);
        SetConsoleTextAttribute(hConsoleOutput, FG_WHITE);
        printf("|       %d       |", BusTrip->Passengers[i].priorityNumber);
        printf("   %d   |", BusTrip->Passengers[i].idNumber);
        GetStringFromNameField(nameBuffer, BusTrip->Passengers[i].passengerName);
        printf(" %s \n", nameBuffer);
        strcpy(nameBuffer, "");
    }
    printf("    A      A               A              A\n");
    printf("    #>----<#>-------------<#>------------<#>------------------------------------<#\n");
    
}

/**
 * @brief Prints search result in the SearchResultField
 * @param *lastNameResults: a pointer to a SearchResultField struct.
 * @param BusTrip[]: an array of Bus15 structs to refer from.
 * @param *nameToSearch: a string of the name to search through.
 * @return void
 */
void
printSearchResults (struct SearchResultField *lastNameResults, struct Bus16 BusTrip[], char *nameToSearch){
	String15 graphicsCode1 	= "SearchResult1";
	String15 graphicsCode2 	= "SearchResult2";
	String15 graphicsCode3 	= "SearchResult3";
	String15 promptMessage 	= "\t> Choice: ";
	String15 errorMessage1	= "Not a number.";
	String31 errorMessage2 	= "Search number is invalid.";
    struct Passenger ResultingPassenger;
    int isQuitingSearch         = FALSE;
    int hasChosenBeyondGiven    = FALSE;
    int hasChosenAResult        = FALSE;
    int isSearching             = TRUE;

    int tripResultIndex = 0;
    int passResultIndex = 0;

    int userChoice;
    int i;
    

    if (!lastNameResults->size) {
        isSearching = FALSE;
        system("cls");
        printf("\nLast name: \"%s\" produces %d result.\n\n", nameToSearch, lastNameResults->size);
    }

    while (lastNameResults->size > 0 && isSearching){
        printGraphics(graphicsCode1);

        for(i = 0; i < lastNameResults->size; i++)
            printf("        | %02d) Name: \"%s\"\n", i + 1, lastNameResults->result[i]);

        printGraphics(graphicsCode2);
        
        repeatGetInteger(&userChoice, graphicsCode3, promptMessage, errorMessage1);

        isQuitingSearch = userChoice == -1;
        hasChosenBeyondGiven = userChoice > lastNameResults->size;
        hasChosenAResult = userChoice > 0 && userChoice <= lastNameResults->size;
        printf("Result: %d %d", userChoice, hasChosenAResult);

        if (isQuitingSearch) {
            isSearching = FALSE;
            system("cls");
        } else if (hasChosenBeyondGiven) {
            system("cls");
            printErrorMessage(errorMessage2);
        } else if (hasChosenAResult) {
            tripResultIndex = lastNameResults->tripNumber[userChoice - 1];
            passResultIndex = lastNameResults->passengerIndex[userChoice - 1];
            ResultingPassenger = BusTrip[tripResultIndex].Passengers[passResultIndex];
            system("cls");
            printPassenger(&ResultingPassenger);
        } 
    }
}

/**
 * @brief Prints the trip information.
 * @param Trips[]: an array of Bus16 structs that contains trip information and acts like the Database.
 * @return void
 */
void 
printTrips (struct Bus16 Trips[]){
    String15 stringEmbarkCode = "";
    String15 graphicsCode1 = "PassEmbark1";
    String15 graphicsCode2 = "PassEmbark2";
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int nColor = FG_WHITE;
    int i;
    printGraphics(graphicsCode1);
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

        printf("\t| |    ");

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
    printGraphics(graphicsCode2);
}

/**
 * @brief Print the results of a SearchResultField struct in an organized manner.
 * @param *DropOffResults: a SearchResultField struct
 * @return void
 */
void
printDropOffs (struct SearchResultField *DropOffResults){
	String15 graphicsCode1 = "DropOff2";
    int i;
    printGraphics(graphicsCode1);
    printf("    |  Y--- START\n     |  |\n");
    for(i = 0; i < DropOffResults->size; i++) {
        printf("    | %02d -- Drop Off: %s\n", i + 1, DropOffResults->result[i]);
        printf("    |  |      Count: %d\n", DropOffResults->passengerIndex[i]);
    }
    printf("    |  A--- END\n");
}

/**
 * @brief Prints people in a size 13 Bus.
 * @param passCount: How many passengers there are. 
 * @return void
 */
void 
printSeats13(int passCount) {
	String15 graphicsCode1 	= "None";
	String31 promptMessage1 = "Please Enter a seat";
	String31 errorMessage1	= "Error, not a Integer"; 
    char seats13[5][3] = {{'\0'}};
    
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
        repeatGetInteger(&passCount, graphicsCode1, promptMessage1, errorMessage1);
}

/**
 * @brief Prints people in a size 16 Bus.
 * @param passCount: How many passengers there are. 
 * @return void
 */
void 
printSeats16(int passCount) {
	String15 graphicsCode1 	= "None";
	String31 promptMessage1 = "Please Enter a seat";
	String31 errorMessage1	= "Error, not a Integer"; 
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
        printf("#--------------#\n");
        printf("|              |\n");
        
        for (i = 0; i < 4; i++) {
            printf("|  ");
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("|\n");
        }
        printf("|              |\n");
        printf("#--------------#\n");
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
        printf("#--------------#\n");
        printf("|              |\n");
        
        for (i = 0; i < 4; i++) {
            printf("|  ");
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("|\n");
        }
        printf("|              |\n");
        printf("#--------------#\n");

    } else if (passCount != 0 || passCount > 16)
        repeatGetInteger(&passCount, graphicsCode1, promptMessage1, errorMessage1);
}


// |===| Trip File Functions |=============================|

/**
 * @brief Searches the index of a Trip.
 * @param *inputTripNo: string of the Trip you want to find the index of.
 * @param TripDatabase[]: a Bus16 struct array to find the tripNo. in
 * @return Integer whether it is found or not
 * @retval 0+ index in the struct Bus16 TripDatabase array
 * @retval -1 if not found.
 */
int
searchIndexOfTripNo(char *inputTripNo, struct Bus16 TripDatabase[]){
    int i;
    for (i = 0; i < TOTAL_TRIPS; i++) {
        if (strcmp(TripDatabase[i].TripID, inputTripNo) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Finds the corresponding String of the Time of Estimated Departure the Trip
 * @param *inputTripNo: String to the trip you want to find
 * @param *strOutputTime: String buffer of the output trip
 * @retval TRUE if found
 * @retval FALSE if not found
 */
ErrorInt
findStringTimeOfTripNo(char *inputTripNo, char *strOutputTime){
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

/**
 * @brief Reads contents of a .txt file and updates the memory
 * @param *tripDate: a DateDMY struct of trip Dates
 * @param BusTrip[]: a struct Bus16 array of BusTrips
 * @param *exits: a dropOffPointList struct array of exits
 * @retval -2 if File Error
 * @retval 0 if Success
 */
ErrorInt
tripFile_GetBusTrip(struct DateDMY *tripDate, struct Bus16 BusTrip[], struct dropOffPointList *exits){
	String63 errorMessage1	= "Error: FILE DOES NOT EXIST. \ntripFile_GetBusTrip error."; 
    struct Passenger    tempPassenger;
    FILE *pFileBusTrip;
    String255 temporaryBuffer[3] = {""};
    String15 fileName   = "";
    String15 timeOfTrip = "";

    int isFileDoesNotExist      = FALSE;
	int tripIndex = 0;
    int i;
    int j;

    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;

    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        printErrorMessage(errorMessage1);
        return EROR_FILE_NOT_FOUND;
    }

    for (i = 0; i < 2; i++) {
            fgets(temporaryBuffer[i], 255, pFileBusTrip);
            removeNewline(temporaryBuffer[i]);
    }

    while (!feof(pFileBusTrip)) {
        // Check if its a new Trip
        if (isStringTripNo(temporaryBuffer[0])) {
            tripIndex = searchIndexOfTripNo(temporaryBuffer[0], BusTrip);
            findStringTimeOfTripNo(temporaryBuffer[0], timeOfTrip);
            strcpy(BusTrip[tripIndex].TripID, temporaryBuffer[0]);
            strcpy(BusTrip[tripIndex].embarkationPoint, temporaryBuffer[1]);
            BusTrip[tripIndex].timeOfTrip = TimeHMfromString(timeOfTrip);
            BusTrip[tripIndex].volume = 0;       
        // Else Assume its a passenger. 
        } else {
            strcpy(tempPassenger.embarkationPoint, BusTrip[tripIndex].embarkationPoint);
            strcpy(tempPassenger.tripNumber, BusTrip[tripIndex].TripID);
            tempPassenger.passengerName = GetNameFieldFromString(temporaryBuffer[0]);   
            tempPassenger.idNumber = atoi(temporaryBuffer[1]);
            for (i = 0; i < 3; i++) {
                fgets(temporaryBuffer[i], 255, pFileBusTrip);
                removeNewline(temporaryBuffer[i]);
            }

            tempPassenger.priorityNumber = atoi(temporaryBuffer[0]);
            strcpy(tempPassenger.dropOffPoint, temporaryBuffer[1]);

            for (i = 0; i < 4; i++){
                for (j = 0; j < exits[i].size; j++){
                    if (strcmp(tempPassenger.dropOffPoint, exits[i].dropOffs[j]) == 0){
                        strcpy(tempPassenger.route, exits[i].route);
                    }
                }
            }
            
            
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
    return PROG_SUCCESS;
}

/**
 * @brief Writes the results of today's passenger to a .txt file.
 * @param *tripDate: the date today in struct DateDMY
 * @param TripDatabase[]: an array of Bus16 struct of the trips needed to search into.
 * @return void
 */
void
tripFile_WriteBusTrip(struct DateDMY *tripDate, struct Bus16 TripDatabase[]){
	String63 errMesFileError = "File writing error! \nCheck if filename is available to use.";
    FILE *busTripFile;
    String15 fileName = "";
    String127 passengerName = "";
    int i, j;

    StringfromDateDMY(fileName, tripDate, TRUE);

    busTripFile = fopen(fileName, "w");

    if (busTripFile == NULL){
        printErrorMessage(errMesFileError);
        fclose(busTripFile);
    }

    for(i = 0; i < TOTAL_TRIPS; i++){
        if (TripDatabase[i].volume > 0) {
            fprintf(busTripFile, "%s\n", TripDatabase[i].TripID);
            fprintf(busTripFile, "%s\n", TripDatabase[i].embarkationPoint);
            for (j = 0; j < TripDatabase[i].volume; j++){
                GetStringFromNameField(passengerName, TripDatabase[i].Passengers[j].passengerName);
                fprintf(busTripFile, "%s\n", passengerName);
                fprintf(busTripFile, "%d\n", TripDatabase[i].Passengers[j].idNumber);
                fprintf(busTripFile, "%d\n", TripDatabase[i].Passengers[j].priorityNumber);
                fprintf(busTripFile, "%s\n\n", TripDatabase[i].Passengers[j].dropOffPoint);
            }
        }
    }

    fclose(busTripFile);
}


// |===| Trip Struct Functions |=============================|
/**
 * @brief Grabs the bus in a trip database based on the ID.
 * @param inputTrip: the TripID of the bus needed.
 * @param TripDatabase[]: an array of Bus16 struct of the trips needed to search into.
 * @param TripKey: a pointer to a Bus16 struct to be filled with the wanted Bus Trip.
 * @param *keyloc: the index of the wanted Bus16's location in TripDatabase. 
 * @return How many passengers are there in the inputted Trip.
 */
int
tripStruct_SearchBusTrip (TripNo inputTrip, struct Bus16 TripDatabase[], struct Bus16 *TripKey, int *keyLoc){
    int i;
    int isTripFound = FALSE;
    int key = 0;
    for(i = 0; i < TOTAL_TRIPS && !isTripFound; i++){
        if(strcmp(TripDatabase[i].TripID, inputTrip) == 0){
            isTripFound = TRUE;
            key = i;
        }
    }

    if (isTripFound)
        *TripKey = TripDatabase[key];

    *keyLoc = key;
    
    return TripDatabase[key].volume;
}

/**
 * @brief Finds passengers by last name and store them in a SearchResultField to be interpretted.
 * @param BusTrip[]: a Bus16 struct that holds.
 * @param *LastName: a string that points to the last name.
 * @param *nameResults: a SearchResultField struct containing exits;
 * @return returns the size of the tripStruct
 */
int
tripStruct_ReturnLastname (struct Bus16 BusTrip[], char *LastName, struct SearchResultField *nameResults){
    String255 strName           = "";
    struct NameField nameBuffer;
    int hasFullSearches = FALSE;
    int i;
    int j;

    nameResults->size = 0;

    // Search Result
    for (i = 0; i < TOTAL_TRIPS && !hasFullSearches; i++){
        for(j = 0; j < 16 && !hasFullSearches; j++){
            nameBuffer = BusTrip[i].Passengers[j].passengerName;
            GetStringFromNameField(strName, nameBuffer);
            removeNewline(nameBuffer.lastName);

            if (isSubString(LastName, nameBuffer.lastName)){
                strcpy(nameResults->result[nameResults->size], strName);
                nameResults->passengerIndex[nameResults->size] = j;
                nameResults->tripNumber[nameResults->size] = i;
                nameResults->size++;
            }

            if (nameResults->size == BUS_SIZE)
                hasFullSearches = TRUE;
        }
    }
    return nameResults->size;
}

// |===| Copy functions |====================================|

/**
 * @brief Initializes dropOffs structs to the required values.
 * @param *input: an array of dropOffPointList structs needed to be initialized with values.
 * @return none
 */
void 
initializeDropOffPointList (struct dropOffPointList input[]){
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

/**
 * @brief A password checker that sees if the given input is equal to the admin password. 
 * @note This is only meant to prevent unauthorized access, and not meant to be cybersecure.
 * @param *isChoosingAdminCmds: boolean condition of the subsequent loop. If true, the user has full access to the mode.
 * @param *isInputingPass: boolean condition of the inner while loop. If false, the password loop ends
 * @param *realPass: String of the real password.
 * @retval None
 */
void 
passwordChecker (int *isChoosingAdminCmds, int *isInputingPass, char *realPass){
    String127 inputPass;
    String63 errorMessage   = "Error, not a string.";
    String31 strWrongPass   = "Wrong Input. Try again.";
    String15 graphicCode    = "PassMenu";
    String15 prompt         = "\t\t> Password: ";
    
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

/**
 * @brief Initializes Bus Trips 
 * @param Triplist[]: An array of Bus16 struct of the trips needed to fill up
 * @param *date: A DateDMY struct of a date
 * @param isStartingFromFile: Boolean for the developers need to start from a txt file quickly
 * @param exits[]: points to a dropOffPointList struct 
 * */
void
initializeBusTrip (struct Bus16 Triplist[], struct DateDMY *date, int isStartingFromFile, struct dropOffPointList *exits){
    TripNo Codes[TOTAL_TRIPS] = {
        "AE101", "AE102", "AE103", "AE104", "AE105",
        "AE106", "AE107", "AE108", "AE109", 
        "AE150", "AE151", "AE152", "AE153", "AE154", 
        "AE155", "AE156", "AE157", "AE158", "AE159", "AE160",
        "SP101", "SP150",
    };
    
    String7 strTime = "";
    int i;
    int j;

    // Initializes every with clear values
    for(i = 0; i < TOTAL_TRIPS; i++) {
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
            strcpy(Triplist[i].Passengers[j].route, "");
            strcpy(Triplist[i].Passengers[j].tripNumber, "");
            Triplist[i].Passengers[j].idNumber = 0;
            Triplist[i].Passengers[j].priorityNumber = 0;
        }
    }

    // Fills them up back with required values.
    for(i = 0; i < TOTAL_TRIPS; i++){
        strcpy(Triplist[i].TripID, Codes[i]);
        findStringTimeOfTripNo(Triplist[i].TripID, strTime);
        Triplist[i].timeOfTrip = TimeHMfromString(strTime);
        Triplist[i].volume = 0;

        if (i < 9 || i == 20)
            strcpy(Triplist[i].embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
        else if (i < 20 || i == 21)
            strcpy(Triplist[i].embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy");
    
        if (i == 20 || (i < 9 && i % 2 == 0))
            strcpy(Triplist[i].route, "Mamplasan Exit");
        else if (i < 9 && i % 2 == 1)
            strcpy(Triplist[i].route, "ETON Exit");
        else if (i == 21 || (i < 20 && i % 2 == 0))
            strcpy(Triplist[i].route, "Estrada");
        else if (i < 20 && i % 2 == 1)
            strcpy(Triplist[i].route, "Buendia/LRT");
    }

    if (isStartingFromFile) {
        tripFile_GetBusTrip(date, Triplist, exits);
    } 
}

/**
 * @brief Clears garbage values of DropOffResult.
 * @param *DropOffResults: a SearchResultField struct to initialize starting zereos
 * @return void
 */
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

/**
 * @brief Counts the frequency of people on a Drop-off.
 * @param BusTrip a pointer Bus16 struct
 * @return a SearchResultField struct
 */
struct SearchResultField
countDropOffFrequency (struct Bus16 BusTrip[]){
    struct SearchResultField DropOffResults;
    int foundSameDropOff = FALSE;;
    int nPass;
    int nRslt;
    int max;
    int i;
    int j;

    initializeSearchResult(&DropOffResults);
    // Set-up
    strcpy(DropOffResults.result[0], BusTrip->Passengers[0].dropOffPoint);
    DropOffResults.passengerIndex[0] = 1;
    DropOffResults.size++;
    // For every passenger
    for (nPass = 1; nPass < BusTrip->volume; nPass++){
        foundSameDropOff = FALSE;  
        // Check results
        for (nRslt = 0; nRslt < DropOffResults.size && !foundSameDropOff; nRslt++){
            // is ALREADY ENCOUNTERED?
            if (strcmp(DropOffResults.result[nRslt], BusTrip->Passengers[nPass].dropOffPoint) == 0){
                foundSameDropOff = TRUE;
                DropOffResults.passengerIndex[nRslt] += 1;
            }
        }
        // Add new category
        if (!foundSameDropOff) {
            strcpy(DropOffResults.result[DropOffResults.size], BusTrip->Passengers[nPass].dropOffPoint);
            DropOffResults.passengerIndex[DropOffResults.size] = 1;
            DropOffResults.size++;
        }
    }

    // SORTING ALGORITHM for Search Results
    for(i = 0; i < DropOffResults.size - 1; i++){
        max = i;
        for(j = i; j < DropOffResults.size; j++){
            // Since ASCII value of '2' > '1', then STRING of max > STRING of j, then strcmp > 0. 
            // All drop-offs start with a number, so arrange increasingly to align.
            if (strcmp(DropOffResults.result[max], DropOffResults.result[j]) > 0)
                max = j;
        }
        // SWAP CODE
        if (max != j) {
            swapStr(DropOffResults.result[i], DropOffResults.result[max]);
            swapInt(DropOffResults.passengerIndex + i, DropOffResults.passengerIndex + max);
        }
    }
    return DropOffResults;
}

/**
 * @brief Promts the user for passenger information. If the given is information is not correct, send error and repromt again.
 * @param *pInput: a pointer to a Passenger struct
 * @param TripDatabase[]: an array to a Bus16 struct
 * @param *exits: an array of dropOffPointList struct
 * @return void 
 */
void
repeatGetPassenger(struct Passenger *pInput, struct Bus16 TripDatabase[], struct dropOffPointList exits[]){
    String63 validationErrorMessage = "Please enter either Y or N only.";
    String15 graphicCodes[12] = {
		"PassEmbark3",
		"PassEmbark4-1",
		"PassEmbark4-2",
		
		"PassEmbark4-3",
		"PassEmbark4-4",
		"PassEmbark5",
		
		"PassEmbark6",
		"PassEmbark7",
		"PassEmbark8",
		
		"PassEmbark9"
	};

	String63 promptMessage[12] = {
		"\t> Please Input Trip No: ",
		"\t> First Name ",
		"\t> Last Name ",	
		"\t> Middle Initial ",
		"\t> ID Number ",
		"\t> Pass Ranking: ",
		"\t> Embarkation Point: ",
		"\t> DropOff Point: ",
		"\t> Route: "
	};
	String63 errorMessage[13] = {
		"Enter a valid one.",
		"Please enter a valid last name within 64 characters.",
		"Please enter a valid first name within 64 characters.",
		"Please enter one valid character.",
		"Please enter a valid integer.",
		"Not a valid ID Number.",
		"Not a valid priority number.",
		"Please choose the number corresponding \nto where you are now.",
		"Not a valid Embarkation selection.",
		"Please choose the number corresponding to the route you want.",
		"Not a valid Route selection.",
		"Please choose which drop off you want.",
		"Not a valid Drop Off selection."
	};
    struct Passenger holder;
    int embarkationNum;
    int routeNum;
    int isValidRanges;
    int dropOffNum;
    int isValidatingAll = TRUE;
    int isValidDetail[5] = {FALSE};
    int isConfirmed = FALSE;

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
            repeatGetTripNo(holder.tripNumber, graphicCodes[0], promptMessage[0], errorMessage[0]);
            system("cls");
            
            isConfirmed = FALSE;
            while(!isConfirmed){
                printf("Is %s correct?\n", holder.tripNumber);
                validateUserInput(&isConfirmed, &isValidDetail[0], validationErrorMessage);
            }
        }

        while (!isValidDetail[1]){
            isConfirmed = FALSE;
            repeatGetString(holder.passengerName.firstName, 63, graphicCodes[1], promptMessage[1], errorMessage[1]);
            system("cls");
            repeatGetString(holder.passengerName.lastName, 63, graphicCodes[2], promptMessage[2], errorMessage[2]);
            system("cls");
            repeatGetChar(&holder.passengerName.midI, graphicCodes[3], promptMessage[3], errorMessage[3]);
            system("cls");

            
            while (!isConfirmed) {
                printGraphics(graphicCodes[4]); // 4 - - // 4 3 3
                printf("Is %s, %s %c. correct?\n", holder.passengerName.lastName, holder.passengerName.firstName, holder.passengerName.midI);
                validateUserInput(&isConfirmed, &isValidDetail[1], validationErrorMessage);
            }
        }

        while (!isValidDetail[2]){
            isConfirmed = FALSE;
            repeatGetInteger(&holder.idNumber, graphicCodes[5], promptMessage[4], errorMessage[4]); // 5 4 4 
            system("cls");
            
            if (holder.idNumber <= 0){
                printErrorMessage(errorMessage[5]); // - - 5 // 5 4 5
            }
            
            while (!isConfirmed && holder.idNumber > 0) {
                printGraphics(graphicCodes[5]); // 5 - - // 5 4 5
                printf("Is %d correct?\n", holder.idNumber);
                validateUserInput(&isConfirmed, &isValidDetail[2], validationErrorMessage);
            }
        }

        while (!isValidDetail[3]){
            isConfirmed = FALSE;
            repeatGetInteger(&holder.priorityNumber, graphicCodes[6], promptMessage[5], errorMessage[6]); // 6 5 6
            system("cls");
            if (holder.priorityNumber <= 0 && holder.priorityNumber > 6){
                printErrorMessage(errorMessage[7]); // - - 7 // 6 5 7
            }
 
            while (!isConfirmed && holder.priorityNumber > 0 && holder.priorityNumber <= 6) {
                printGraphics(graphicCodes[6]); // 6 - - // 6 5 6
                printf("Is %d correct?\n", holder.priorityNumber);
                validateUserInput(&isConfirmed, &isValidDetail[3], validationErrorMessage);
            }
        }

        while (!isValidDetail[4]){
            isConfirmed = FALSE;
            isValidRanges = TRUE;
            system("cls");
            repeatGetInteger(&embarkationNum, graphicCodes[7], promptMessage[6], errorMessage[7]); // 7 6 7

            if (embarkationNum < 0 || embarkationNum > 1) {
                printErrorMessage(errorMessage[8]); // - - 8 // 7 6 8
                isValidRanges = FALSE;
            } else {
                if (embarkationNum == 0) 
                    strcpy(holder.embarkationPoint, "DLSU Manila Campus - South Gate Driveway");
                else
                    strcpy(holder.embarkationPoint, "DLSU Laguna Campus - Milagros Del Rosario (MRR) Building - East Canopy");
            }

            if (isValidRanges) {
                system("cls");
                repeatGetInteger(&routeNum, graphicCodes[8], promptMessage[7], errorMessage[9]); // 9 7 9

                if (routeNum < 0 || routeNum > 3) {
                    printErrorMessage(errorMessage[10]); // - - 10 // 9 7 10
                    isValidRanges = FALSE;
                } else {
                    strcpy(holder.route, exits[routeNum].route);
                }
            }

            if (isValidRanges) {
                system("cls");
                repeatGetInteger(&dropOffNum, graphicCodes[9], promptMessage[8], errorMessage[11]);

                if (dropOffNum < 0 || dropOffNum >= 13) {
                    printErrorMessage(errorMessage[12]);
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
