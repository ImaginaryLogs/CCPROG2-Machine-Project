#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// |===| Define and Typedef |===================================|

#define BG_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define BG_BLACK (0x0)
#define BG_RED BACKGROUND_RED
#define FG_RED FOREGROUND_RED
#define FG_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FG_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)


typedef int ErrorInt;

typedef char String255[256];
typedef char String63[64];
typedef char String31[32];
typedef char String15[16];
typedef char TripNo[7];

struct Passenger {
    String255 dropOffPoint;
    String63 name;
    time_t currentDate;
    int idNumber;
    int timeOfTrip;
};
typedef struct Passenger Bus13[13];
typedef struct Passenger Bus16[13];


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
    printf("\n[!] ERROR:\n");
    SetConsoleTextAttribute(hConsoleOutput, FG_RED);
    printf("\t%s\n", errorMessage);
    SetConsoleTextAttribute(hConsoleOutput, FG_WHITE | BG_BLACK);
    printf("\n");
}

/**
 * @brief Prints ASCII graphics found in ASCII_Art.txt
 * 
 * @param graphicsID Name of the graphic to render.
 * @return 0 if No error, 1 if File not Found, 2 if EOF, 3 - Misalignment
 * Pre-condition: Given graphicsID must exist in ASCII_Art.txt.
 */
ErrorInt 
printGraphics(char *graphicsID){
    String255 graphicsData;
    String15 stringGraphicHeight;
    String15 scannedGraphicId;
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
        printf("File not found\n");
        haveNotFoundGraphic = 0;
        errorCode = 1;
    }

    while(haveNotFoundGraphic){
        //Format: "ID:<graphic id>, <height of graphic>;"
        idScanned = fscanf(fileGraphics, "ID:%15[^,], %[^;];\n", scannedGraphicId, &stringGraphicHeight);

        // printf("Scanned Id: %s, Height: %s\n", scannedGraphicId, stringGraphicHeight);
        
        graphicHeight = atoi(stringGraphicHeight); // converts strings into integer when possible.

        // Conditions unpacked for better reading
        isEndOfFile = idScanned == EOF;
        isPreviousMetadataSame = strcmp(prevGraphicHeight, stringGraphicHeight) == 0 && 
                                strcmp(prevGraphicId, scannedGraphicId) == 0;
        isSameGraphicId = strcmp(scannedGraphicId, graphicsID) == 0;
        
        haveNotFoundGraphic = 0; // Assume a closing statement like an error or found the graphics
       
        if (isEndOfFile) {
            printErrorMessage("Program Error: EOF. Graphics Not found.\n");
            errorCode = 2;
        } else if (isPreviousMetadataSame) {
            printErrorMessage("Program Error: Misalignment. Next Graphics Metadata not found.\n");
            printf("Height:  %s %s\n", prevGraphicHeight, stringGraphicHeight);
            printf("Graphic: %s %s\n", prevGraphicId, scannedGraphicId);
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

        for (int gLine = 0; gLine < graphicHeight; gLine++) 
            fgets(graphicsData, 256, fileGraphics);
        
        strcpy(prevGraphicHeight, stringGraphicHeight);
        strcpy(prevGraphicId, scannedGraphicId);
    }
    
    fclose(fileGraphics);
    return errorCode;
}

/**
 * @brief Promts the user for a string. If the given does not fit into given size of the string, send error and repromt again.
 * 
 * @param pInput Pointer to the string to be assigned. 
 * @param size Maximum size of the string.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
 * Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 */
void 
repeatGetString(char *pInput, int size, char choiceMenuGraphicsCode[], char promtMessage[], char errorMessage[]){
    int isIncorrectInput = 1;
    int dataTypes;
    char closingChar;
    char stringFormat[13] = " %";
    char stringSize[5] = {""};

    itoa(size, stringSize, 10);
    strcat(stringFormat, stringSize);
    strcat(stringFormat, "[^\n]%c");

    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        dataTypes = scanf(stringFormat, pInput, &closingChar);
        if (dataTypes != 2 || closingChar != '\n') {
            system("cls");
            clearInput();
            printErrorMessage(errorMessage);
        }
        else
            isIncorrectInput = 0;
    }
    while(isIncorrectInput);
}

/**
 * @brief Promts the user for an integer. If the given is not an integer, send error and repromt again.
 * 
 * @param pInput Pointer to the integer to be assigned.
 * @param choiceMenuGraphicsCode[]: String pointer of the Graphics Code of the corresponding Graphics to print in ASCII_Art.txt.
 * @param promtMessage[]: String pointer of the prompt that tells the user what to input.
 * @param errorMessage[]: String pointer of the Error message to be given to the console.
* Pre-condition: ASCII_Art.txt file exist and its metadata aligned, choiceMenuGraphicsCode given exist in ASCII_Art.txt, and promtMessage and errorMessage have been declared and initialized.
 */
void
repeatGetInteger(int *pInput, char choiceMenuGraphicsCode[], char promtMessage[], char errorMessage[]){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int isIncorrectInput = 1;
    char closingChar;
    long long int pInput2 = 0;
    do {
        printGraphics(choiceMenuGraphicsCode);
        printf("%s", promtMessage);
        if (scanf("%lld%c", &pInput2, &closingChar) != 2 || closingChar != '\n'){
            system("cls");
            clearInput();
            printErrorMessage(errorMessage);
        } else if (pInput2 < -2147483648 || pInput2 > 2147483647){
            system("cls");
            printErrorMessage(errorMessage);
        } else
            isIncorrectInput = 0;
    }
    while(isIncorrectInput);
    *pInput = pInput2;
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
repeatGetChar(char *pInput, char *choiceMenuGraphicsCode, char promtMessage[], char errorMessage[]){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int isIncorrectInput = 1;
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


// |===| Essential Functions Section |=====================|

void passwordMenu(int *isChoosingAdminCmds, int *isInputingPass, char *realPass){
    String255 inputPass;
    String63 errorMessage = "Error, not a string.";
    char main;

    repeatGetString(inputPass, 255, "PassMenu", "\t> Password: ", errorMessage);
    printf("\n");

    if (strcmp(inputPass, realPass) == 0) {
        *isChoosingAdminCmds = TRUE;
    } else if (strcmp(inputPass, "quit") == 0){
        *isInputingPass = FALSE;
    } else {
        system("cls");
        printErrorMessage("Wrong Input. Try again.");
    }
}

// |===| PASSENGER CMD SECTION |=====================|

void userEmbarkation(){
    printf("[O] Enter Trip Number: \n");
    printSingleColorText(BACKGROUND_GREEN, "User creates an embarkation trip.");
}

// |===| ADMIN CMD SECTION ==========================|

void adminNoOfPassenger(){
    printSingleColorText( FG_YELLOW, "Admin views a Trip's no. of Passenger");
}

void adminCountPassengerDropOff(){
    printSingleColorText( FG_YELLOW, "Admin counts number of Passenger in a drop-off.");
}

void adminViewPassengerInfo(){
    printSingleColorText( FG_YELLOW, "Admin views the passenger info.");
}

void adminSearchPassenger(){
    printSingleColorText( FG_YELLOW, "Admin searches the passenger in a trip.");
}

void adminEmbarkation(){
    printSingleColorText( FG_YELLOW, "Admin creates an embarkation trip.");
}

// |===| MENU SECTION |=============================|

/**
 * TODO fill it functions accessible to a passenger based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 * 
 */
void menuPassenger()
{
    char inputPassMenu;
    char tripNo[6];
    String63 errorMessage = "Dear Passenger, Please select the following valid cmds\n";

    do {
        repeatGetChar(&inputPassMenu, "PassengerMenu", "\t> Action: ", errorMessage);
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
    int isInputing = TRUE;
    int isChoosingAdminCmds = FALSE;
    char userInput;
    
    
    while (isInputing && !isChoosingAdminCmds)
        passwordMenu(&isChoosingAdminCmds, &isInputing, realPass);

    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, "AdminMenu", "\t> Command: ", errorMessage);
        switch(userInput){
            case 'a':
                adminNoOfPassenger();
                break;
            case 'b':
                adminCountPassengerDropOff();
                break;
            case 'c':
                adminViewPassengerInfo();
                break;
            case 'd':
                adminSearchPassenger();
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


