/*
 * REFERENCES:
 * [1] itoa():  https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
 * [2] atoi():  https://www.ibm.com/docs/en/i/7.4?topic=functions-atoi-convert-character-string-integer
 * [3] SetConsoleTextAttribute():   https://learn.microsoft.com/en-us/windows/console/setconsoletextattribute
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// |===| Define and Typedef |===================================|
#define TOTAL_TRIPS 22
#define BUS_SIZE 16
#define NUM_PASSENGER_DETAILS 5
#define BG_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define BG_BLACK (0x0)
#define BG_RED BACKGROUND_RED
#define FG_RED FOREGROUND_RED
#define FG_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FG_GREEN (FOREGROUND_GREEN)

#define PROG_SUCCESS 0
#define EROR_KEY_NOT_FOUND -1
#define EROR_FILE_NOT_FOUND -2

typedef char String511[512];
typedef char String255[256];
typedef char String127[128];
typedef char String63[64];
typedef char String31[32];
typedef char String15[16];
typedef char String7[8];
typedef char TripNo[7];

typedef int ErrorInt;

struct TimeHM {
    int hour;
    int minute;
};

struct DateDMY {
    int day;
    int month;
    int year;
};

struct NameField {
    String63 lastName;
    String63 firstName;
    char midI;
};

struct dropOffPointList {
    String127 dropOffs[4];
    String31 route;
    int size;
};

struct Passenger {
    String255 embarkationPoint;
    String255 dropOffPoint;
    struct NameField passengerName;
    TripNo tripNumber;
    unsigned int idNumber;
    unsigned int priorityNumber;
};

struct SearchResultField{
    String255 result[BUS_SIZE];
    int passengerIndex[BUS_SIZE];
    int tripNumber[BUS_SIZE];
    int size;
};

struct Bus16 {
    struct Passenger Passengers[BUS_SIZE];
    struct TimeHM timeOfTrip;
    String31 route;
    TripNo TripID;
    int volume;
};

struct routePoints {
    String127 strDropOffs[4];
    String15 strExitRoutes;
    int routeSize;
};
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
 * @param *errorMessage Error message to be sent to the console.
 */
void
printErrorMessage(char *errorMessage){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleOutput, FG_RED );
    printf("\n#>==<|[X] ERROR: |>===============================================#\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_RED | FOREGROUND_INTENSITY);
    printf("\n\t%s \n", errorMessage);
    SetConsoleTextAttribute(hConsoleOutput, FG_RED);
    printf("\n#>---------------------------------------------------------------<#\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

void
printPopUpMessage(char *headerString, int headerColor, char *bodyMessage){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    String63 strTopBar = "";
    int i;
    for(i = 0; i < 34 - (int) strlen(headerString); i++)
        strcat(strTopBar, "=");
    printf("\n");
    SetConsoleTextAttribute(hConsoleOutput, headerColor | FOREGROUND_INTENSITY);
    printf("#>==<|%s |>%s<#", headerString, strTopBar);
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n\n\t%s\n\n", bodyMessage);
    SetConsoleTextAttribute(hConsoleOutput, headerColor | FOREGROUND_INTENSITY);
    printf("#>-----------------------------------------<#");
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
    String63 strErrorEndOfFile = "PG Error: EOF. \n\tGraphics Not found.\n";
    String63 strErrorMisalign = "PG Error: Misalignment. \n\tNext Graphics Metadata not found.\n";
    String63 strErrorFileNotFound = "PG Error: File not found. \n\t\"ASCII_Art.txt\" not found.\n";
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
    int isEndOfFile = FALSE;
    int isPreviousMetadataSame = FALSE;
    int isSameGraphicId = FALSE;

    fileGraphics = fopen("ASCII_Art.txt", "r");
    
    if (fileGraphics == NULL) {
        printErrorMessage(strErrorFileNotFound);
        haveNotFoundGraphic = 0;
        errorCode = 1;
    }

    while (haveNotFoundGraphic){
        //Format: "ID:<graphic id>, <height of graphic>;"
        fscanf(fileGraphics, strMetadataFormat, scannedGraphicId, stringGraphicHeight);
        
        graphicHeight = atoi(stringGraphicHeight); // converts strings into integer when possible.

        // Conditions unpacked for better reading
        isEndOfFile = feof(fileGraphics);
        isPreviousMetadataSame = strcmp(prevGraphicHeight, stringGraphicHeight) == 0 && 
                                strcmp(prevGraphicId, scannedGraphicId) == 0;
        isSameGraphicId = strcmp(scannedGraphicId, graphicsID) == 0;
        
        haveNotFoundGraphic = FALSE; // Assume a closing statement like an error or found the graphics
       
        if (isEndOfFile) {
            printf("Cannot find %s\n", graphicsID);
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
        } else 
            haveNotFoundGraphic = TRUE; // Else, loop through again with the next graphic cell

        for (gLine = 0; gLine < graphicHeight; gLine++) 
            fgets(graphicsData, 256, fileGraphics);
        
        strcpy(prevGraphicHeight, stringGraphicHeight);
        strcpy(prevGraphicId, scannedGraphicId);
    }
    fclose(fileGraphics);
    return errorCode;
}

int
isInputSuccesful(int inputs, char closingChar, char *errorMessage){
    int isInputSuccessful = TRUE;
    if (inputs != 2){
        isInputSuccessful = FALSE;
        system("cls");
        printErrorMessage(errorMessage);
    }

    if (closingChar != '\n') {
        isInputSuccessful = FALSE;
        clearInput();
    }
    return isInputSuccessful;
}

void
repeatGetTripNo(char *pInput, char choiceMenuGraphicsCode[], char *promtMessage, char *errorMessage){
    int isIncorrectInput = 1;
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
                strcpy(pInput, "quit");
            } else {
                strcpy(pInput, "AE");
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
    int isIncorrectInput = TRUE;
    int isPossibleOverflow = FALSE;
    int isRemainWhitespace = FALSE;

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
    int isIncorrectInput = TRUE;
    int returnedInputs = 0;
    char closingChar;
 
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        returnedInputs = scanf("%d%c", pInput, &closingChar);
        isIncorrectInput = !isInputSuccesful(returnedInputs, closingChar, errorMessage);
    } while(isIncorrectInput);
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
    int returnedInputs = 0;
    char closingChar;
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        returnedInputs = scanf("%c%c", pInput, &closingChar);
        isIncorrectInput = !isInputSuccesful(returnedInputs, closingChar, errorMessage);
    } while(isIncorrectInput);
}

/**
 * @brief Checks if the month has 31 days.
 * @note   
 * @param Month Integer representation of a month.
 * @returns Boolean if is the month is a Day 31 Month 
 */
int 
checkValidDay(int Year, int Month, int Day){
    int isMonthAtDay31;
    int isLeapYear;
    
    isLeapYear = ((Year % 4 == 0) && (Year % 100 != 0)) || (Year % 400 == 0);
    isMonthAtDay31 = ((Month > 0) && (Month < 8) && (Month % 2 == 1)) || 
                     ((Month > 7) && (Month <= 12) && (Month % 2 == 0));
    return  Day > 0 &&
            (!(Month == 2 && isLeapYear) || Day < 30) &&
            (isMonthAtDay31 ? Day <= 31 : Day <= 30);
}

void
printDate(struct DateDMY *date){
    String7 strMonths[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("%d, %s %d", date->year, strMonths[date->month - 1], date->day);
};

/**
 * @brief  
 * @note   
 * @param  *pInput: 
 * @retval 
 */
void
repeatGetDateDMY(struct DateDMY *pInput){
    String63 errorMessage = "Please input the correct format.";
    String63 validationErrorMessage = "Please enter either Y or N only.";

    int isIncorrectInput = TRUE;
    int successfulInputs = 0;
    int returnedInputs = 0;
    int isValidDay = FALSE;
    int isValidMonth = FALSE;
    int isValidYear = FALSE;
    int isValidChoice;

    char userValidation; 
    char closingChar;

    do {
        successfulInputs = 0;
        printGraphics("EnterDate");
        isValidChoice = TRUE;
        while (successfulInputs < 3) {
            switch (successfulInputs) {
                case 0:
                    printf("\t> Enter Year: ");
                    returnedInputs = scanf("%d%c", &pInput->year, &closingChar);

                    if (pInput->year < 2000) 
                        printErrorMessage("Enter a recent year after 2000.");

                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, errorMessage) &&
                                        (pInput->year >= 2000);
                    break;
                case 1:
                    printf("\t> Enter Month: ");
                    returnedInputs = scanf("%d%c", &pInput->month, &closingChar);
                    isValidMonth = pInput->month >= 1 && pInput->month < 13;

                    if (!isValidMonth) 
                        printErrorMessage("Enter a valid month.");

                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, errorMessage) &&
                                        isValidMonth;
                    break;
                case 2:
                    printf("\t> Enter Day: ");
                    returnedInputs = scanf("%d%c", &pInput->day, &closingChar);
                    isValidDay = checkValidDay(pInput->year, pInput->month, pInput->day);

                    if (!isValidDay) 
                        printErrorMessage("Enter a valid day.");

                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, errorMessage) &&
                                        isValidDay;
                    break;
            }
        }
        
        while (isValidChoice) {
            printf("Should the date entered be: ");
            printDate(pInput);
            printf("?\n");
            repeatGetChar(&userValidation, "None", "\t> [Y/N]:", "Enter a valid char.");
            switch(userValidation){
                case 'Y':
                case 'y':
                    isIncorrectInput = FALSE;
                case 'N':
                case 'n':
                    isValidChoice = FALSE;
                    system("cls");
                    break;
                default:
                    printErrorMessage(validationErrorMessage);
            }
        }
    } while (isIncorrectInput);
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

/**
 * @brief If it sees a new line character at the end, then this function removes it.
 * @param *strInput: The string you want to modify
 * @return char pointer to the modified string. 
 */
char *
removeNewline(char *strInput){
    if(strInput[strlen(strInput)-1] == '\n')
        strInput[strlen(strInput)-1] = '\0';
    return strInput;
}


char *
GetStringFromNameField(char *strName, struct NameField name){
    strcpy(strName, name.lastName);
    strcat(strName, ", ");
    strcat(strName, name.firstName);

    if (name.midI != 0) {
        strcat(strName, " ");
        strName[strlen(strName) + 1] = '\0';
        strName[strlen(strName)] = name.midI;
        strcat(strName, ".");
    }

    return strName;
}


void
copySubnameFromName(char *strSubname, char *strFullname, int subnameStartingIndex, int subnameLastIndex){
    int nCharPosOfSubname = 0;

    for(nCharPosOfSubname = subnameStartingIndex; nCharPosOfSubname < subnameLastIndex; nCharPosOfSubname++)
        strSubname[nCharPosOfSubname - subnameStartingIndex] = strFullname[nCharPosOfSubname];

    strSubname[subnameLastIndex - subnameStartingIndex] = '\0';
}

/**
 * @brief Breaksdown a full name into its individual components. 
 * @param *strName: The string you want to breakdown. 
 * @return A struct Namefield from the converted string.
 * @note Pre-condition: strName is in the format of "lastname, firstName M." or "lastname, firstname"
 */
struct NameField
GetNameFromString(char *strName){
    struct NameField output;
    int hasSeenFirstName;
    int hasSeenMiddileInitial;
    int hasNoMiddleInitial;
    int nCharPosOfFullname;
    int firstNameCeiling = 0;
    int firstNameFloor = 0;
    int isDone = FALSE; 

    for(nCharPosOfFullname = 0; nCharPosOfFullname < strlen(strName) && !isDone; nCharPosOfFullname++){
        hasSeenFirstName = strName[nCharPosOfFullname] == ','; // Ceiling of last name section
        hasSeenMiddileInitial = strName[nCharPosOfFullname] == '.'; // Ceiling of middle initial
        hasNoMiddleInitial = nCharPosOfFullname + 1 == strlen(strName) && 
                             strName[nCharPosOfFullname] != '.'; // Remember that the index starts at 0. So the index's ranges from 0 to strlen - 1, so Offset by 1 to make it from 1 to strlen.
            
        if (hasSeenFirstName) {
            firstNameFloor = nCharPosOfFullname + 2; // Floor / first char of first name is two chars away from ',' 
            copySubnameFromName(output.lastName, strName, 0, nCharPosOfFullname);
        } else if (hasSeenMiddileInitial){
            output.midI = strName[nCharPosOfFullname - 1];
            firstNameCeiling = nCharPosOfFullname - 2; // Ceiling / last char of first name is two chars away from the middle initial.
            copySubnameFromName(output.firstName, strName, firstNameFloor, firstNameCeiling);
        } else if (hasNoMiddleInitial){
            output.midI = '\0';
            strcpy(output.firstName, strName + firstNameFloor);
        }
    }

    return output;
}
