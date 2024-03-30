/*
 * REFERENCES:
 * [1] itoa():  https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-itoa-convert-int-into-string
 * [2] atoi():  https://www.ibm.com/docs/en/i/7.4?topic=functions-atoi-convert-character-string-integer
 * [3] SetConsoleTextAttribute():   https://learn.microsoft.com/en-us/windows/console/setconsoletextattribute
 * [4] time and localtime of <time.h>
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
#define FG_INTEN (FOREGROUND_INTENSITY)

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
typedef char String3[4];
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
    String63    lastName;
    String63    firstName;
    char midI;
};

struct dropOffPointList {
    String127   dropOffs[4];
    String31    route;
    int size;
};

struct Passenger {
    struct NameField passengerName;
    String255   dropOffPoint;
    String127   embarkationPoint;
    String31    route;
    TripNo      tripNumber;
    int idNumber;
    int priorityNumber;
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
    String127 embarkationPoint;
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
 * @brief Clears the input for anything not a new line.
 */
void 
clearInput(){
    while (getchar() != '\n'); // Get character while its not a new line and do nothing.
}


/**
 * @brief Swaps integers
 * @note Pre-condition: both integers have non-garbage values.
 * @param *nSRCE: int source you want to swap from
 * @param *nDEST: int destination you want to swap to
 * @return void
 */
void
swapInt(int *nDEST, int *nSRCE){
    int temp;
    printf("1");
    temp = *nSRCE;
    *nSRCE = *nDEST;
    *nDEST = temp;
    printf("2");
}

/**
 * @brief Swaps the content of string
 * @note Pre-condition: both strings have enough size to fit each other's size.
 * @param *nSRCE: string source you want to swap from
 * @param *nDEST: string destination you want to swap to
 * @return void
 */
void
swapStr(char *nDEST, char *nSRCE){
    String255 temp = "";
    printf("3");
    strcpy(temp, nDEST);
    strcpy(nDEST, nSRCE);
    strcpy(nSRCE, temp);
    printf("4");
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
    printf("\n\t%s\t\n", errorMessage);
    SetConsoleTextAttribute(hConsoleOutput, FG_RED);
    printf("\n#>---------------------------------------------------------------<#\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

/**
 * @brief Prints a pop-up message. 
 * @param *headerString: String of the top header of the message. 
 * @param headerColor: Color of the message. 
 * @param *bodyMessage: Body of the message.
 */
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
 * @brief Prints the date to console.
 * @param *date: a DateDMY struct you want to print.
 */
void
printDate(struct DateDMY *date){
    String3 strMonths[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("%d, %s %d", date->day, strMonths[date->month - 1], date->year);
};

/**
 * @brief Prints ASCII graphics found in ASCII_Art.txt
 * 
 * @param graphicsID Name of the graphic to render.
 * @return 0 if No error, 1 if File not Found, 2 if EOF, 3 if Misalignment
 * @note Pre-condition: Given graphicsID must exist in ASCII_Art.txt.
 */
ErrorInt 
printGraphics(char *graphicsID){
    String255 graphicsData = "";

    String63 strErrorEndOfFile      = "PG Error: EOF. \n\tGraphics Not found.\n";
    String63 strErrorMisalign       = "PG Error: Misalignment. \n\tNext Graphics Metadata not found.\n";
    String63 strErrorFileNotFound   = "PG Error: File not found. \n\t\"ASCII_Art.txt\" not found.\n";

    String31 strMetadataFormat      = "ID:%s %s\n";
    String15 stringGraphicHeight    = "";
    String15 scannedGraphicId       = "";
    String15 prevGraphicHeight      = "";
    String15 prevGraphicId          = "";

    FILE *fileGraphics;
    
    // Int variables
    ErrorInt errorCode  = 0;
    int graphicHeight   = 0;
    int gLine           = 0;    // Graphics line

    // Boolean definitions
    int haveNotFoundGraphic     = TRUE;
    int isEndOfFile             = FALSE;
    int isPreviousMetadataSame  = FALSE;
    int isSameGraphicId         = FALSE;

    fileGraphics = fopen("ASCII_Art.txt", "r");
    
    if (fileGraphics == NULL) {
        printErrorMessage(strErrorFileNotFound);
        haveNotFoundGraphic = FALSE;
        errorCode = EROR_FILE_NOT_FOUND;
        return errorCode;
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

/**
 * @brief Checks if the month has 31 days. 
 * @param Month integer representation of a month.
 * @returns Boolean if is the month is a Day 31 Month 
 */
int 
checkValidDate(int Year, int Month, int Day){
    int isMonthAtDay31;
    int isLeapYear;
    // A Leap Year should be divisible by four but not by 100, but it can be bypassed if its divisible by 400.
    isLeapYear = ((Year % 4 == 0) && (Year % 100 != 0)) || (Year % 400 == 0);
    /*  Requirements for Months with 31 days:
    *   odd between January (Month 1) to July (Month 7), or
    *   even between August (Month 8) to December (Month 12)
    * */
    isMonthAtDay31 = ((Month > 0) && (Month < 8) && (Month % 2 == 1)) || 
                     ((Month > 7) && (Month <= 12) && (Month % 2 == 0));
    /**
     * February is special as Leap Years are the only days they <= 29, else its <= 28.
     * Material Implication A -> B, !A || B
    */
    return  Day > 0 &&
            (!(Month == 2) || ((isLeapYear) ? Day <= 29 : Day <= 28)) &&
            (isMonthAtDay31 ? Day <= 31 : Day <= 30);
}

/**
 * @brief Converts a DateDMY struct into a String. Optionally, it can be turned into a filename.
 * @param *dateString: char buffer to store the string you want to convert TO.
 * @param *date: the DateDMY struct you want to convert FROM.
 * @param isFilename: boolean condition if filename is needed.
 */
void
StringfromDateDMY(char *dateString, struct DateDMY *date, int isFilename){
    String15 fileName       = "Trip-";
    String15 fileExtension  = ".txt";
    String15 strYear    = "";
    String15 strMonth   = "";
    String15 strDay     = "";
    
    // Uses itoa, converts int into a string based on the given base REF: [01]. 
    itoa(date->year, strYear, 10);
    itoa(date->month, strMonth, 10);
    itoa(date->day, strDay, 10);

    if (isFilename)
        strcat(dateString, fileName);

    strcat(dateString, strDay);
    dateString[strlen(dateString)] = '-';
    strcat(dateString, strMonth);
    dateString[strlen(dateString)] = '-';
    strcat(dateString, strYear);

    if (isFilename)
        strcat(dateString, fileExtension);
}

/**
 * @brief Returns a TimeHM struct from a string with the time 
 * @param  *timeString: string of the time you want to extract FROM.
 * Pre-condition: Assumes that the string is properly formatted in a "HH:MM" or "H:MM".
 * @return a struct TimeHM of the time.
 */
struct TimeHM 
TimeHMfromString(char *timeString){
    struct TimeHM time;
    String7 hour = "";
    int separator;

    separator = timeString[1] == ':' ? 1 : 2;

    strcpy(hour, timeString);
    hour[separator] = '\0';

    time.hour = atoi(hour);
    time.minute = atoi(hour + separator + 1);

    return time;
}

/**
 * @brief Checks the time today and returns a DateDMY struct.
 * @returns a DateDMY struct of today.
 */
struct DateDMY 
GetDateToday(){
    struct DateDMY  today;
    struct tm       Time;

    time_t t = time(NULL);
    Time = *localtime(&t);

    today.year = Time.tm_year + 1900;
    today.month = Time.tm_mon + 1;
    today.day = Time.tm_mday;

    return today;
}

/**
 * @brief Gets the time right now and returns a TimeHM struct.
 * @returns a TimeHM struct of at the current hour and minute.
 */
struct TimeHM 
GetTimeHmToday(){
    struct TimeHM   timeNow;
    struct tm       Time;

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

/**
 * @brief Copies the substring from a given range of index.
 * @param *strSub: String buffer TO substring output.
 * @param *strBigger: String pointing to the bigger string to grab the substr FROM. 
 * @param subnameStartingIndex: Starting index of
 * @param subnameLastIndex: 
 * @returns Char pointer to the substring
 */
char *
copySubstringFromString(char *strSub, char *strBigger, int subnameStartingIndex, int subnameLength){
    int charIndex = 0;

    for(charIndex = 0; charIndex < subnameLength; charIndex++)
        strSub[charIndex] = strBigger[charIndex + subnameStartingIndex];

    strSub[subnameLength] = '\0';
    return strSub;
}

/**
 * @brief Checks if the substring is within the bigger string.
 * @param *substring: the substring
 * @param *biggerString: the string you want to check.
 * @return Boolean if the substring is present within the string.
 * @retval TRUE if substring is present
 * @retval FALSE if not present.
 */
int
isSubString(char *substring, char *biggerString){
    int nSameCharacters = 0;
    int strlenSub       = 0;
    int strlenBigger    = 0;
    int nBig    = 0;
    int nSub    = 0;

    int isFound = FALSE;

    strlenSub   = strlen(substring);
    strlenBigger = strlen(biggerString);

    for(nBig = 0; nBig < strlenBigger - strlenSub + 1 && !isFound; nBig++){
        nSameCharacters = 0;

        for(nSub = 0; nSub < strlenSub; nSub++){
            nSameCharacters += biggerString[nBig + nSub] == substring[nSub];
        }

        if (nSameCharacters == strlenSub)
            isFound = TRUE;
    }
    
    return isFound;
}

/**
 * @brief Gets the string version of a Namefield struct.
 * @param *strName: String buffer that holds the output name.
 * @param name: Namefield struct you want to get the String FROM
 * @returns The pointer to strName with content.
 */
char *
GetStringFromNameField(char *strName, struct NameField name){
    strcpy(strName, name.lastName);

    if (strlen(name.lastName) > 0)
        strcat(strName, ", ");

    strcat(strName, name.firstName);

    if (name.midI != '\0') {
        strcat(strName, " ");
        strName[strlen(strName) + 1] = '\0';
        strName[strlen(strName)] = name.midI;
        strcat(strName, ".");
    }

    return strName;
}

/**
 * @brief Breaksdown a full name into its individual components. 
 * @param *strName: The string you want to breakdown. 
 * @return A struct Namefield from the converted string.
 * @note Pre-condition: strName is in the format of "lastname, firstName M." or "lastname, firstname"
 */
struct NameField
GetNameFieldFromString(char *strName){
    struct NameField output;
    int hasSeenFirstName        = FALSE;
    int hasSeenMiddileInitial   = FALSE;
    int hasNoMiddleInitial      = FALSE;
    int charIndex       = 0;
    int firstNameCeil   = 0;
    int firstNameFlor   = 0;

    for(charIndex = 0; charIndex < (int) strlen(strName) ; charIndex++){
        // Ceiling of last name section
        hasSeenFirstName = strName[charIndex] == ','; 
        // Ceiling of middle initial
        hasSeenMiddileInitial = strName[charIndex] == '.'; 
        /* 
         * Remember that the index starts at 0. So the index's ranges from 0 to strlen - 1, 
         * so Offset by 1 to make it from 1 to strlen.
        */
        hasNoMiddleInitial = charIndex + 1 == (int) strlen(strName) && 
                             strName[charIndex] != '.'; 
            
        if (hasSeenFirstName) {
            firstNameFlor = charIndex + 2; // Floor / first char of first name is two chars away from ',' 
            copySubstringFromString(output.lastName, strName, 0, charIndex);
        } else if (hasSeenMiddileInitial){
            output.midI = strName[charIndex - 1];
            firstNameCeil = charIndex - 2; // Ceiling / last char of first name is two chars away from the middle initial.
            copySubstringFromString(output.firstName, strName, firstNameFlor, firstNameCeil);
        } else if (hasNoMiddleInitial){
            output.midI = '\0';
            strcpy(output.firstName, strName + firstNameFlor);
        }
    }

    return output;
}

/**
 * @brief Checks if given is a TripNo.
 * @param *inputString: String to test if its an input;
 * @return Boolean whether or not it is a TripNo.
 * @retval TRUE if it is a recognizable TripNo.
 * @retval FALSE if its not.
 */
int
isStringTripNo(char *inputString){
    String3 tripNumberPart   = "";
    String3 tripCodePart     = "";

    int isTripNo            = FALSE;
    int isFirstCapitalized  = TRUE;
    int isSecondNumber      = TRUE;
    int i = 0;
    
    if (strlen(inputString) > 6 || strlen(inputString) < 5 || (inputString[5] != '\n' && inputString[5] != '\0'))
        return isTripNo;
    
    copySubstringFromString(tripCodePart, inputString, 0, 2);
    copySubstringFromString(tripNumberPart, inputString, 2, 3);

    for (i = 0; i < 2; i++) {
        if (tripCodePart[i] < 65 || tripCodePart[i] > 90) {
            isFirstCapitalized = FALSE;
        }
    }

    for (i = 0; i < 3; i++) {
        if (tripNumberPart[i] < 48 || tripNumberPart[i] > 57) {
            isSecondNumber = FALSE;
        }
    }

    isTripNo = isFirstCapitalized && isSecondNumber;

    return isTripNo;
}

/**
 * @brief Checks if scanf is successful.
 * @param inputs: mumber of inputs.
 * @param closingChar: character at the end of the input.
 * @param *errorMessage: errorMessage to pop up
 * @retval TRUE if its the given fits the requirements of a scanf
 * @retval FALSE if it fails one of the conditions.
 */
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

/**
 * @brief Promts the user for a string. If the given does not fit into given length of the string, send error and repromt again.
 * 
 * @param pInput Pointer to the string to be assigned. 
 * @param length Length of the string format.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
 * @note Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
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
        // fgets is more safer.
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
 * @param *pInput Pointer to the integer to be assigned.
 * @param *choiceMenuGraphicsCode: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param *promtMessage: String pointer of the prompt that tells the user what to input.
 * @param *errorMessage: String pointer of the Error message to be given to the console.
 * @note Pre-condition: Scanned Input is within the range of an Integer defined by the Computer. ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
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
 * @param *pInput: Pointer to the char to be assigned.
 * @param *choiceMenuGraphicsCode: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param *promtMessage: String pointer of the prompt that tells the user what to input.
 * @param *errorMessage: String pointer of the Error message to be given to the console.
 * @note Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 * */
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
 * @brief validates the user if their input is correct.
 * @param *isChoiceCertain: pointer to a bool condition of a inner while loop asking if input is final.
 * @param *isChoiceValidByUser: pointer a bool condition of the outer while loop questioning what is the input.
 * @param *validationErrorMessage: error if the choice is not in the given
 */
void
validateUserInput(int *isChoiceCertain, int *isChoiceValidByUser, char *validationErrorMessage){
	String15 strGraphics 			= "None";
	String15 strPrompt 				= "\t> [Y/N]: ";
	String31 strConfirmationError 	= "Enter a valid char.";
    char userValidation;
    repeatGetChar(&userValidation, strGraphics, strPrompt, strConfirmationError);
    switch(userValidation){
        case 'Y':
        case 'y':
            *isChoiceValidByUser = !*isChoiceValidByUser;
        case 'N':
        case 'n':
            *isChoiceCertain = !*isChoiceCertain;
            system("cls");
            break;
        default:
            printErrorMessage(validationErrorMessage);
    }
}

/**
 * @brief Promts the user for a date (YYYY-MM-DD). If the given is not a date, send error and repromt again.
 * @param *pInput: a pointer to the DateDMY struct output.
 */
void
repeatGetDateDMY(struct DateDMY *pInput){
    // Error Messages
    String63 typeErrMessage     = "Please input the correct format.";
    String63 validErrMessage    = "Please enter either Y or N only.";
    String63 promptYear 		= "Enter a recent year after 2000.";
	String63 promptMonth		= "Enter a valid month.";
	String63 promptDay			= "Enter a valid day.";
	String15 graphicCode 		= "EnterDate";
	
    // Boolean Conditions
    int isFindingDate   = TRUE;
    int isValidDay      = FALSE;
    int isValidMonth    = FALSE;
    int isValidYear     = FALSE;
    int isConfirming    = FALSE;
    
    int successfulInputs    = 0;
    int returnedInputs      = 0;

    // User inputs
    char closingChar;

    while (isFindingDate) {
        printGraphics(graphicCode);

        successfulInputs = 0;
        while (successfulInputs < 3) {
            switch (successfulInputs) {
                case 0: // Year Section
                    printf("\t> Enter Year: ");
                    returnedInputs = scanf("%d%c", &pInput->year, &closingChar);

                    if (pInput->year < 2000) 
                        printErrorMessage(promptYear);
                    isValidYear = pInput->year >= 2000;
                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, typeErrMessage) &&
                                        isValidYear;
                    break;
                case 1: // Month Section
                    printf("\t> Enter Month: ");
                    returnedInputs = scanf("%d%c", &pInput->month, &closingChar);
                    isValidMonth = pInput->month >= 1 && pInput->month < 13;

                    if (!isValidMonth) 
                        printErrorMessage(promptMonth);

                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, typeErrMessage) &&
                                        isValidMonth;
                    break;
                case 2: // Day Section
                    printf("\t> Enter Day: ");
                    returnedInputs = scanf("%d%c", &pInput->day, &closingChar);
                    isValidDay = checkValidDate(pInput->year, pInput->month, pInput->day);

                    if (!isValidDay) 
                        printErrorMessage(promptDay);

                    successfulInputs += isInputSuccesful(returnedInputs, closingChar, typeErrMessage) &&
                                        isValidDay;
                    break;
            }
        }

        isConfirming = FALSE;
        while (!isConfirming) {
            printf("Should the date entered be: ");
            printDate(pInput);
            printf("?\n");
            validateUserInput(&isConfirming, &isFindingDate, validErrMessage);
        }
    } 
}

/**
 * @brief Promts the user for a trip number. If the given is not a date, send error and repromt again.
 * @param *pInput: Pointer to the string TripNo to be assigned.
 * @param *choiceMenuGraphicsCode: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param *promtMessage: String pointer of the prompt that tells the user what to input.
 * @param *errorMessage: String pointer of the Error message to be given to the console.
 * @note Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 * @returns none
 */
void
repeatGetTripNo(char *pInput, char choiceMenuGraphicsCode[], char *promtMessage, char *errorMessage){ 
    // String buffers
    String7 strTripNumber   = "";
    String7 TripCodePart    = "";
    String7 TripNumberPart  = "";
    
    int isWithinRange   = FALSE;
    int isValidTripCode = FALSE;
    int TripNumber      = 0;

    do {
        repeatGetString(strTripNumber, 6, choiceMenuGraphicsCode, promtMessage, errorMessage);
        copySubstringFromString(TripCodePart, strTripNumber, 0, 2);
        copySubstringFromString(TripNumberPart, strTripNumber, 2, 6);
        
        TripNumber = atoi(TripNumberPart);

        isWithinRange = (TripNumber >= 101 && TripNumber <= 109) || 
                        (TripNumber >= 150 && TripNumber <= 160);

        isValidTripCode =   ((strcmp(TripCodePart, "AE") == 0 && isWithinRange) || 
                            (strcmp(TripCodePart, "SP") == 0 && (TripNumber == 101 || TripNumber == 150))) &&
                            strlen(TripNumberPart) > 0;


        if (isValidTripCode)
            strcpy(pInput, strTripNumber);

    } while(!isValidTripCode);
}


