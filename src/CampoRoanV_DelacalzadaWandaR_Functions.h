#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <sys/stat.h>
/*
 * REFERENCES:
 * [1] https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
 * [2] time_t: https://en.cppreference.com/w/c/chrono/time
 * [3] 
 */

// |===| Define and Typedef |===================================|

#define BUS_SIZE 16
#define NUM_PASSENGER_DETAILS 5
#define BG_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define BG_BLACK (0x0)
#define BG_RED BACKGROUND_RED
#define FG_RED FOREGROUND_RED
#define FG_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#define PROG_SUCCESS 0
#define EROR_KEY_NOT_FOUND -1
#define EROR_FILE_NOT_FOUND -2


typedef int ErrorInt;

typedef char String511[512];
typedef char String255[256];
typedef char String127[128];
typedef char String63[64];
typedef char String31[32];
typedef char String15[16];
typedef char String7[8];
typedef char TripNo[7];

struct TimeHM {
    unsigned int hour;
    unsigned int minute;
};
struct DateDMY {
    unsigned int day;
    unsigned int month;
    unsigned int year;
};

struct NameField {
    String63 lastName;
    String63 firstName;
    char midI;
};

struct Passenger {
    String255 embarkationPoint;
    String255 dropOffPoint;
    struct NameField passengerName;
    TripNo tripNumber;
    struct DateDMY dateOfTrip;
    struct TimeHM timeOfTrip;
    unsigned int idNumber;
    unsigned int priorityNumber;
};

struct SearchResultField{
    String255 result[BUS_SIZE];
    int index[BUS_SIZE];
    int size;
};

typedef struct Passenger Bus16[BUS_SIZE];


// |===| Helpful Functions |=========================|

/**
 * Clears the input for anything not a new line.
 */
void 
clearInput(){
    while (getchar() != '\n'); // Get character while its not a new line and do nothing.
}

/**
 * @brief Prints a single text statement with color, and returns to white and black.
 * 
 * @param nCOLOR Integer code for colors defined by a windows.h macro
 * @param strInput String to be printed in color
 * Precondition: Given nColor is defined by MACRO
 */
void
printSingleColorText(int nColor, char *strInput){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleOutput, nColor);
    printf(strInput);
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

/**
 * @brief Prints an Error message.
 * 
 * @param *errorMessage Error message to be sent to the console.
 */
void
printErrorMessage(char *errorMessage){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleOutput, FG_RED | FOREGROUND_INTENSITY);
    printf("\n#>==<|[X] ERROR: |>=========================#\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_RED);
    printf("\n\t%s \n", errorMessage);
    SetConsoleTextAttribute(hConsoleOutput, FG_RED | FOREGROUND_INTENSITY);
    printf("\n#>-----------------------------------------<#\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

void
printPopUpMessage(char *headerString, int headerColor, char *bodyMessage){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int i;
    printf("\n");
    SetConsoleTextAttribute(hConsoleOutput, headerColor | FOREGROUND_INTENSITY);
    printf("#>==<|%s |>", headerString);
    for(i = 0; i < 34 - strlen(headerString); i++){
        printf("=");
    }
    printf("<#");
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n\n\t%s\n\n", bodyMessage);


    SetConsoleTextAttribute(hConsoleOutput, headerColor | FOREGROUND_INTENSITY);
    printf("#>---------");
    for(i = 0; i < 32; i++){
        printf("-");
    }
    printf("<#");
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

/**
 * @brief Prints ASCII graphics found in ASCII_Art.txt
 * 
 * @param graphicsID Name of the graphic to render.
 * @return 0 if No error, 1 if File not Found, 2 if EOF, 3 if Misalignment
 * Pre-condition: Given graphicsID must exist in ASCII_Art.txt.
 */
ErrorInt 
printGraphics(char *graphicsID){
    String255 graphicsData = "";
    
    String63 strErrorEndOfFile = "PG Error: EOF. Graphics Not found.\n";
    String63 strErrorMisalign = "PG Error: Misalignment. Next Graphics Metadata not found.\n";
    String63 strErrorFileNotFound = "PG Error: File not found. \"ASCII_Art.txt\" not found.\n";
    String31 strMetadataFormat = "ID:%s %s\n";
    String15 stringGraphicHeight = "";
    String15 scannedGraphicId = "";
    String15 prevGraphicHeight = "";
    String15 prevGraphicId = "";

    FILE *fileGraphics;

    ErrorInt errorCode = 0;
    int graphicHeight = 0;
    int haveNotFoundGraphic = 1;
    int gLine;

    // Boolean definitions
    int isEndOfFile = 0;
    int isPreviousMetadataSame = 0;
    int isSameGraphicId = 0;

    char idScanned;

    fileGraphics = fopen("ASCII_Art.txt", "r");
    
    if (fileGraphics == NULL) {
        printErrorMessage(strErrorFileNotFound);
        haveNotFoundGraphic = 0;
        errorCode = 1;
    }

    while(haveNotFoundGraphic){
        //Format: "ID:<graphic id>, <height of graphic>;"
        idScanned = fscanf(fileGraphics, strMetadataFormat, scannedGraphicId, stringGraphicHeight);

        // printf("Scanned Id: %s, Height: %s\n", scannedGraphicId, stringGraphicHeight);
        
        graphicHeight = atoi(stringGraphicHeight); // converts strings into integer when possible.

        // Conditions unpacked for better reading
        isEndOfFile = idScanned == EOF;
        isPreviousMetadataSame = strcmp(prevGraphicHeight, stringGraphicHeight) == 0 && 
                                strcmp(prevGraphicId, scannedGraphicId) == 0;
        isSameGraphicId = strcmp(scannedGraphicId, graphicsID) == 0;
        
        haveNotFoundGraphic = 0; // Assume a closing statement like an error or found the graphics
       
        if (isEndOfFile) {
            printErrorMessage(strErrorEndOfFile);
            errorCode = 2;
        } else if (isPreviousMetadataSame) {
            printErrorMessage(strErrorMisalign);
            printf("Height:  Previous: \"%s\" vs Next: \"%s\"\n", prevGraphicHeight, stringGraphicHeight);
            printf("Graphic: Previous: \"%s\" vs Next: \"%s\"\n", prevGraphicId, scannedGraphicId);
            errorCode = 3;
        } else if (isSameGraphicId) {
            for (gLine = 0; gLine < graphicHeight; gLine++) {
                fgets(graphicsData, 256, fileGraphics);
                printf("%s", graphicsData);
            }
        } else {
            // Else, loop through again with the next graphic cell
            haveNotFoundGraphic = 1;
        }

        for (gLine = 0; gLine < graphicHeight; gLine++) 
            fgets(graphicsData, 256, fileGraphics);
        
        strcpy(prevGraphicHeight, stringGraphicHeight);
        strcpy(prevGraphicId, scannedGraphicId);
    }
    
    fclose(fileGraphics);
    return errorCode;
}

void
repeatGetTripNo(char *pInput, char choiceMenuGraphicsCode[], char *promtMessage, char *errorMessage){
    int isIncorrectInput = 1;
    int isInputSuccessful = 0;
    int isPossibleOverflow = 0;
    int isRemainWhitespace = 0;
    int TripNumber;
    int typeReturned = 0;
    char closingChar;
    String7 strTripNumber = "";
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        typeReturned = scanf("%d%c", &TripNumber, &closingChar);
        if ((TripNumber < 99 && TripNumber > 0) || TripNumber < 0|| TripNumber > 1000 || typeReturned != 2 || closingChar != '\n'){
            system("cls");
            if (closingChar != '\n')
                clearInput();
            printErrorMessage(errorMessage);
        } else {
            isIncorrectInput = 0;
            if (TripNumber == 0){
                strcat(pInput, "quit");
            } else {
                strcat(pInput, "AE");
                itoa(TripNumber, strTripNumber, 10);
                strcat(pInput, strTripNumber);
            }
        }

    } while(isIncorrectInput);
}

/**
 * @brief Promts the user for a string. If the given does not fit into given length of the string, send error and repromt again.
 * 
 * @param pInput Pointer to the string to be assigned. 
 * @param length Length of the string format.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
 * Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 */
void 
repeatGetString(char *pInput, int maxLength, char choiceMenuGraphicsCode[], char *promtMessage, char *errorMessage){
    int isIncorrectInput = 1;
    int isInputSuccessful = 0;
    int isPossibleOverflow = 0;
    int isRemainWhitespace = 0;

    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);

        // Replace the last non-null char in string with something temporarily
        pInput[maxLength] = 'A';
        fgets(pInput, maxLength + 1, stdin); // size of array = maxLength + 1
        
        isPossibleOverflow = pInput[maxLength - 1] != '\n' && pInput[maxLength] == '\0';
        isRemainWhitespace = isPossibleOverflow ? getchar() == '\n' : FALSE;

        if (isPossibleOverflow && !isRemainWhitespace){
            clearInput();
            printErrorMessage(errorMessage);
        } else 
            isIncorrectInput = FALSE;
        
        if (!isRemainWhitespace) {
            pInput[maxLength] = '\0';
            pInput[strlen(pInput) - 1] = '\0';
        }
    } while(isIncorrectInput);
}

/**
 * @brief Promts the user for an integer. If the given is not an integer, send error and repromt again.
 * 
 * @param pInput Pointer to the integer to be assigned.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
* Pre-condition: Scanned Input is within the range of an Integer defined by the Computer. ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 */
void
repeatGetInteger(int *pInput, char choiceMenuGraphicsCode[], char promtMessage[], char errorMessage[]){
    int isIncorrectInput = 1;
    char closingChar;
 
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        if (scanf("%d%c", pInput, &closingChar) != 2 || closingChar != '\n'){
            system("cls");
            clearInput();
            printErrorMessage(errorMessage);
        } else
            isIncorrectInput = 0;
    }
    while(isIncorrectInput);
}

/**
 * @brief Promts the user for an char. If the given is not a char, send error and repromt again.
 *  
 * @param pInput[]: Pointer to the char to be assigned.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
 * Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
*/
void
repeatGetChar(char *pInput, char choiceMenuGraphicsCode[], char promtMessage[], char errorMessage[]){
    int isIncorrectInput = TRUE;
    char closingChar;
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s ", promtMessage);
        if(scanf("%c%c", pInput, &closingChar) != 2 || closingChar != '\n'){
            clearInput();
            system("cls");
            printErrorMessage(errorMessage);
        }
        else
            isIncorrectInput = 0;
        
    }
    while(isIncorrectInput);
}

void
StringfromDateDMY(char *dateString, struct DateDMY *date, int isFilename){
    String15 fileName = "Trip-";
    String15 strYear = "";
    String15 strMonth = "";
    String15 strDay = "";
    String15 fileExtension = ".txt";
    // Uses itoa, converts int into a string based on the given base REF: [01]. 
    itoa(date->year, strYear, 10);
    itoa(date->month, strMonth, 10);
    itoa(date->day, strDay, 10);

    if (isFilename)
        strcat(dateString, fileName);

    strcat(dateString, strYear);
    dateString[strlen(dateString)] = '-';
    strcat(dateString, strMonth);
    dateString[strlen(dateString)] = '-';
    strcat(dateString, strDay);

    if (isFilename)
        strcat(dateString, fileExtension);
}

int
DateDMYfromString(struct DateDMY *date, char *dateString){
    String15 year = "";
    int i;
    int foundYearMonthSeparator;
    strcpy(year, dateString);
    for (i = 0; i < strlen(dateString) && !foundYearMonthSeparator; i++){
        if (dateString[i] == '-')
            foundYearMonthSeparator = TRUE;
    }
    if (!foundYearMonthSeparator)
        return -1;
    year[i] = '\0';
    printf("%s", year);

    return PROG_SUCCESS;
}

struct TimeHM 
TimeHMfromString( char *timeString){
    struct TimeHM time;
    int i;
    int foundYearMonthSeparator;
    int separator = timeString[1] == ':' ? 1 : 2;
    String7 hour = "";
    strcpy(hour, timeString);
    hour[separator] = '\0';
    time.hour = atoi(hour);
    time.minute = atoi(hour + separator + 1);
    return time;
}

int
isSubString(char *subStr, char *str){
    int i, j, nSameChars;
    int isFound = 0;
    int subStrLength, strLength;
    subStrLength = strlen(subStr);
    strLength = strlen(str);
    for(i = 0; i < strLength - subStrLength + 1 && !isFound; i++){
        nSameChars = 0;
        for(j = 0; j < subStrLength; j++){
            nSameChars += str[i + j] == subStr[j];
        }
        if (nSameChars == subStrLength)
            isFound = 1;
    }
    
    return isFound;
}

struct DateDMY 
GetDateToday(){
    struct DateDMY today;
    struct tm Time;
    time_t t = time(NULL);
    Time = *localtime(&t);
    today.year = Time.tm_year + 1900;
    today.month = Time.tm_mon + 1;
    today.day = Time.tm_mday;
    return today;
}

struct TimeHM 
GetTimeHmToday(){
    struct TimeHM timeNow;
    struct tm Time;
    time_t t = time(NULL);
    Time = *localtime(&t);
    timeNow.hour = Time.tm_hour;
    timeNow.minute = Time.tm_min;
    return timeNow;
}

void
removeNewLine(char *strInput){
    if(strInput[strlen(strInput)-1] == '\n')
        strInput[strlen(strInput)-1] = '\0';
}

char *
GetStringFromNameField(char *strName, struct NameField name){
    int length;
    strcat(strName, name.lastName);
    strcat(strName, ", ");
    strcat(strName, name.firstName);
    if (name.midI != 0) {
        strcat(strName, " ");
        strcat(strName, &name.midI);
        strcat(strName, ".");
    }
    return strName;
}

struct NameField
GetNameFromString(char *strName){
    struct NameField output;
    int fullNIndex;
    int subNIndex;
    int stringLength = strlen(strName);
    int firstNameCeiling = 0;
    int firstNameFloor = 0;
    for(fullNIndex = 0; fullNIndex < stringLength; fullNIndex++){

        if(strName[fullNIndex] == ',') {
            firstNameFloor = fullNIndex + 2;
            
            for(subNIndex = 0; subNIndex < fullNIndex; subNIndex++)
                output.lastName[subNIndex] = strName[subNIndex];
            output.lastName[subNIndex] = '\0';
        } else if (strName[fullNIndex] == '.'){

            firstNameCeiling = fullNIndex - 2;
            output.midI = strName[fullNIndex - 1];

            for(subNIndex = firstNameFloor; subNIndex < firstNameCeiling; subNIndex++)
                output.firstName[subNIndex - firstNameFloor] = strName[subNIndex];
             
            output.firstName[firstNameCeiling - firstNameFloor] = '\0';
        } else if (fullNIndex == stringLength - 1){
            strcpy(output.firstName, strName + firstNameFloor);
        }
    }

    return output;
}

// |===| Essential Functions Section |=====================|


/**
 * @brief Checks if the date today matches with internal date of the program
 * @note   
 * @retval 
 */
int
checkDate(){

}

void
loadCurrentState(){

}

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
    printf("| Time:\t\t\t %02d%02d\n", Passenger->timeOfTrip.hour, Passenger->timeOfTrip.minute);
    printf("A Drop off Point:\t %s\n", Passenger->dropOffPoint);
    printf("#>-------------------------------#\n\n");
}

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
tripFilePassengerAppender(struct DateDMY tripDate, struct Passenger *newPassenger){
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
    fprintf(pFileBusTrip, "%d %d\n", newPassenger->timeOfTrip.hour, newPassenger->timeOfTrip.minute);
    fprintf(pFileBusTrip, "%s\n", newPassenger->dropOffPoint);
    
    fclose(pFileBusTrip);
    
    return PROG_SUCCESS;
}

int 
tripFileGetPassenger(struct DateDMY *tripDate, struct Passenger *keyPassenger, int key){
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    String15 strDateOfTrip = "";
    String15 strTimeOfTrip = "";
    String15 strPriorityNumber = "";
    String15 strIdNumber = "";
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
    if(fgets(keyPassenger->tripNumber, 255, pFileBusTrip) == NULL) {
        fclose(pFileBusTrip);
        printErrorMessage("EOF REACHED! Search others.");
        return EROR_KEY_NOT_FOUND;
    };

    fgets(keyPassenger->embarkationPoint, 255, pFileBusTrip);
    fgets(strName, 255, pFileBusTrip);
    fgets(strIdNumber, 255, pFileBusTrip);
    fgets(strPriorityNumber, 255, pFileBusTrip);
    fgets(strTimeOfTrip, 255, pFileBusTrip);
    fgets(keyPassenger->dropOffPoint, 255, pFileBusTrip);
    
    // Clean the Key's details;
    removeNewLine(keyPassenger->tripNumber);
    removeNewLine(keyPassenger->embarkationPoint);
    removeNewLine(strName);
    removeNewLine(strIdNumber);
    removeNewLine(strPriorityNumber);
    removeNewLine(keyPassenger->dropOffPoint);
    keyPassenger->passengerName = GetNameFromString(strName);
    keyPassenger->idNumber = atoi(strIdNumber);
    keyPassenger->priorityNumber = atoi(strPriorityNumber);
    keyPassenger->timeOfTrip = TimeHMfromString(strTimeOfTrip);
    
    fclose(pFileBusTrip);

    return PROG_SUCCESS;
}

int
tripFileReturnLastname(struct DateDMY *tripDate, Bus16 BusTrip, String63 LastName, struct SearchResultField *nameResults){
    struct Passenger holder;
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    struct TimeHM tempTime;
    struct NameField nameBuffer;
    int isFileDoesNotExist = FALSE;
    int hasFullSearches = FALSE;
    int hasNotFoundEOF = TRUE;
    int numSubstring = 0;
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
            return numSubstring;
        }

        removeNewLine(strName);
        nameBuffer = GetNameFromString(strName);
        if (isSubString(LastName, nameBuffer.lastName) == TRUE){
            printf("%s\n",strName);
            strcpy(nameResults->result[numSubstring], strName);
            nameResults->index[numSubstring] = linesSearched;
            numSubstring++;
        }

        if (numSubstring == BUS_SIZE)
            hasFullSearches = TRUE;
        // Other information is ignored
        for(linesIgnored = 0; linesIgnored < 6 && hasNotFoundEOF; linesIgnored++)
            if (fgets(temporaryBuffer, 255, pFileBusTrip) == NULL) {
                fclose(pFileBusTrip);
                return numSubstring;
            }
        linesSearched++;
    }
    fclose(pFileBusTrip);
    return numSubstring;
}

int
tripFileGetBusTrip(struct DateDMY *tripDate, TripNo inputTrip, Bus16 BusTrip){
    struct Passenger holder;
    FILE *pFileBusTrip;
    String255 temporaryBuffer = "";
    String255 strName = "";
    String15 fileName = "";
    String15 strTimeOfTrip = "";
    String15 strPriorityNumber = "";
    String15 strIdNumber = "";
    struct TimeHM tempTime;
    ErrorInt nIndex = -1;
    int isFileDoesNotExist = 0;
    int line;
    int BusPassenger = 0;
    int hasNotFoundEOF = TRUE;
    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;
    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        printErrorMessage("ERROR DOES NOT EXIST");
        return EROR_FILE_NOT_FOUND;
    }

    while (hasNotFoundEOF){
        if (fgets(holder.tripNumber, 255, pFileBusTrip) == NULL) {
            fclose(pFileBusTrip);
            hasNotFoundEOF = FALSE;
        } else {
            removeNewLine(holder.tripNumber);
        }

        if (strcmp(holder.tripNumber, inputTrip) == 0 && hasNotFoundEOF){
            fgets(holder.embarkationPoint, 255, pFileBusTrip);
            fgets(strName, 255, pFileBusTrip);
            fgets(strIdNumber, 255, pFileBusTrip);
            fgets(strPriorityNumber, 255, pFileBusTrip);
            fgets(strTimeOfTrip, 255, pFileBusTrip);
            fgets(holder.dropOffPoint, 255, pFileBusTrip);

            removeNewLine(holder.embarkationPoint);
            removeNewLine(strName);
            removeNewLine(strIdNumber);
            removeNewLine(strPriorityNumber);
            removeNewLine(holder.dropOffPoint);

            holder.idNumber = atoi(strIdNumber);
            holder.priorityNumber = atoi(strPriorityNumber);
            holder.passengerName = GetNameFromString(strName);
            holder.timeOfTrip = tempTime;
            holder.timeOfTrip = TimeHMfromString(strTimeOfTrip);
            BusTrip[BusPassenger] = holder;
            BusPassenger++;
        } else if (hasNotFoundEOF) {
            for(line = 0; line < 6 ; line++)
                fgets(temporaryBuffer, 255, pFileBusTrip) == NULL;
        }
    }      

    fclose(pFileBusTrip);

    return BusPassenger;
}

int
tripFileSearchPassengerFull(struct DateDMY *tripDate, struct Passenger *keyPassenger){
    int i;
    int isFound = 0;
    int key = -1;
    for(i = 0; i < 16 && !isFound; i++){
        isFound = tripFileGetPassenger(tripDate, keyPassenger, i) == 0 ? TRUE : -1;
        if (isFound)
            key = i;
    }
    return key;
}

int
tripFileSearchSameTrip(struct DateDMY *tripDate, TripNo tripNumber, Bus16 BusOfTrip){
    FILE *pFileBusTrip;
    String511 temporaryBuffer = "";
    String15 fileName = "";
    String15 strDateOfTrip = "";
    struct Passenger tempPassenger;
    struct TimeHM tempTime;
    ErrorInt nIndex = -1;
    int isFileDoesNotExist = 0;
    int tripIndex, fileIndex, busIndex;
    int hasTripID, isSameTripLength; 
    // File Handling
    StringfromDateDMY(fileName, tripDate, TRUE);
    StringfromDateDMY(strDateOfTrip, tripDate, FALSE);
    pFileBusTrip = fopen(fileName, "r");
    isFileDoesNotExist = pFileBusTrip == NULL;
    if (isFileDoesNotExist) {   
        fclose(pFileBusTrip);
        return EROR_FILE_NOT_FOUND;
    }
    int hasFoundPassenger = FALSE;
    // Skip Unnecessary Lines;
    
    tripIndex = 0;
    fileIndex = 0;
    while(hasFoundPassenger != -1){
        hasFoundPassenger = tripFileGetPassenger(tripDate, &tempPassenger, fileIndex);
        printf("%s\n", tempPassenger.tripNumber);
        if(strcmp(tempPassenger.tripNumber, tripNumber) == 0){
            BusOfTrip[tripIndex] = tempPassenger;
            tripIndex++;
        }
        fileIndex++;
    }

    fclose(pFileBusTrip);
    return tripIndex;
}

void
initializeSearchResult(struct SearchResultField * DropOffResults){
    int i;
    for (i = 0; i < BUS_SIZE; i++){
        strcmp(DropOffResults->result[i], "");
        DropOffResults->index[i] = 0;
    }
    DropOffResults->size = 0;
}

// |===| PASSENGER CMD SECTION |=====================|

void 
userEmbarkation(){
    String63 strFiller = "User creates an embarkation trip.";
    printf("[O] Enter Trip Number: \n");
    printSingleColorText(BACKGROUND_GREEN, strFiller);
}

// |===| ADMIN CMD SECTION ==========================|

void 
adminNoOfPassenger(){
    String63 strFiller = "Admin views a Trip's no. of Passenger";
    printSingleColorText( FG_YELLOW, strFiller);
}

void 
adminCountPassengerDropOff(struct DateDMY *tripDate){
    String63 strFiller = "Admin counts number of Passenger in a drop-off.";
    printSingleColorText( FG_YELLOW, strFiller);
    Bus16 BusTrip;
    struct SearchResultField DropOffResults;
    
    TripNo inputTripNumber = ""; 
    initializeSearchResult(&DropOffResults);

    repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip.");
    printf("Trip: %s\n", inputTripNumber);
    int results = tripFileGetBusTrip(tripDate, inputTripNumber, BusTrip);
    int i, j;
    int foundSameDropOff;
    printPassenger(&BusTrip[15]);
    if (results > 0) {
        strcpy(DropOffResults.result[0], BusTrip[0].dropOffPoint);
        DropOffResults.index[0] = 1;
        
        for (i = 1; i < results; i++){
            foundSameDropOff = FALSE;
            for (j = 0; j < DropOffResults.size && !foundSameDropOff; j++){
                if (strcmp(DropOffResults.result[j], BusTrip[i].dropOffPoint) == 0){
                    foundSameDropOff = TRUE;
                    DropOffResults.index[j] += 1;
                }
            }

            if (!foundSameDropOff){
                strcpy(DropOffResults.result[DropOffResults.size], BusTrip[i].dropOffPoint);
                DropOffResults.index[DropOffResults.size] += 1;
                DropOffResults.size++;
            }
        }


        for(i = 0; i < DropOffResults.size; i++){
            printf("Drop Off: %s\n", DropOffResults.result[i]);
            printf("   Count: %d\n", DropOffResults.index[i]);
        }
    }
}

void 
adminViewPassengerInfo(struct DateDMY *tripDate){
    String63 strFiller = "Admin views the passenger info.";
    printSingleColorText( FG_YELLOW, strFiller);
    Bus16 BusTrip;
    TripNo inputTripNumber = ""; 
    String255 nameBuffer = "";
    int passengers;
    int i;
    int j;
    int isDoneVieweing = FALSE;
    

    while (!isDoneVieweing){
        printf("\n");
        repeatGetTripNo(inputTripNumber, "CountPassenger", "\n\t> Trip Number:", "Please input an existing trip. \n\tType \'0\' to exit.");
        
        if (strcmp(inputTripNumber, "quit") == 0){
            isDoneVieweing = TRUE;
            return;
        }
        printf("Trip: %s\n", inputTripNumber);    
    
        passengers = tripFileGetBusTrip(tripDate, inputTripNumber, BusTrip);
        
        if (passengers > 0) {
            printf("Passengers of %s:\n", inputTripNumber);
            printf("#=>-------------------------- - -\n");
            for(i = 0; i < passengers; i++){
                printf("Y\tName: %s\n", GetStringFromNameField(nameBuffer, BusTrip[i].passengerName));
                strcpy(nameBuffer, "");
                printf("|\tIDno: %d\n", BusTrip[i].idNumber);
                printf("A\tPriorityNo: %d\n", BusTrip[i].priorityNumber);
                printf("#=>-------------------------- - -\n");
            }
        }
    }
}

void
adminSearchPassenger(struct DateDMY *dateToday){
    String63 strFiller = "Admin searches the passenger in a trip.";
    printSingleColorText( FG_YELLOW, strFiller);
    Bus16 BusTrip;
    struct Passenger searchingPassenger;
    struct SearchResultField lastNameResults;
    struct NameField searchResults[16];
    int i;
    String15 name;
    int userChoice;
    int isFinding = TRUE;
    int isSearching = TRUE;
    while(isFinding){
        repeatGetString(name, 15, "SearchPass", "\t> Last Name: ", "Error, not a last name");
        removeNewLine(name);
        if (strcmp(name, "quit") == 0){
            isFinding = FALSE;
            return;
        }

        lastNameResults.size = tripFileReturnLastname(dateToday, BusTrip, name, &lastNameResults);
        isSearching = TRUE;
        if (!lastNameResults.size) {
            system("cls");
            printf("\nLast name: \"%s\" produces %d result.\n\n", name, lastNameResults.size);
        }

        while (lastNameResults.size > 0 && isSearching){
            printGraphics("SearchResult1");

            for(i = 0; i < lastNameResults.size; i++)
                printf("| %02d) Name: \"%s\"\n", i + 1, lastNameResults.result[i]);

            printGraphics("SearchResult2");
            
            repeatGetInteger(&userChoice, "SearchResult3", "\t> Choice: ", "Not a number.");
            if (userChoice == -1) {
                isSearching = FALSE;
                system("cls");
            } else if (userChoice > 0) {
                tripFileGetPassenger(dateToday, &searchingPassenger, lastNameResults.index[userChoice - 1]);
                printPassenger(&searchingPassenger);
            }
        }
          
        
    }
    
}

void adminEmbarkation(){
    String63 strFiller = "Admin creates an embarkation trip.";
    printSingleColorText( FG_YELLOW, strFiller);

}

// |===| MENU SECTION |=============================|

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
void menuAdmin(){
    String255 realPass = "Admin123"; // to be changed
    String63 errorMessage = "Please input a valid admin cmd.\n";
    String15 graphicCode = "AdminMenu";
    String15 strPrompt = "\t> Command: ";
    int isInputing = TRUE;
    int isChoosingAdminCmds = FALSE;
    char userInput;
    struct DateDMY date;
    
    
    while (isInputing && !isChoosingAdminCmds)
        passwordMenu(&isChoosingAdminCmds, &isInputing, realPass);

    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, graphicCode, strPrompt, errorMessage);
        date = GetDateToday();
        switch(userInput){
            case 'a':
                adminNoOfPassenger();
                break;
            case 'b':
                adminCountPassengerDropOff(&date);
                break;
            case 'c':
                adminViewPassengerInfo(&date);
                break;
            case 'd':
                adminSearchPassenger(&date);
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


