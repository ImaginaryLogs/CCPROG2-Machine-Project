
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// |===| Define and Typedef |===================================|

#define BACKGROUND_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)
#define BACKGROUND_BLACK (0x0)
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define FOREGROUND_BLACK (0x0)

typedef int ErrorInt;

typedef char String255[256];
typedef char String63[64];
typedef char String31[32];
typedef char String15[16];

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
 * Precondition: Given nColor is defined by MACRO
 */
void
printSingleColorText(int nColor, char *strInput){
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleOutput, nColor);
    printf(strInput);
    SetConsoleTextAttribute(hConsoleOutput, FOREGROUND_WHITE | BACKGROUND_BLACK);
    printf("\n");
}

/**
 * @brief Promts the user for a string. If the given does not fit into given size of the string, send error and repromt again.
 * 
 * @param pInput Pointer to the string to be assigned. 
 * @param size Maximum size of the string.
 * @param errorMessage Pointer to the Error message to be given to the console.
 */
void 
repeatGetString(char *pInput, int size, char *errorMessage){
    int isIncorrectInput = 1;
    int dataTypes;
    char closingChar;
    char stringFormat[12] = "%";
    char stringSize[5] = {""};

    itoa(size, stringSize, 10);
    strcat(stringFormat, stringSize);
    strcat(stringFormat, "[^\n]%c");

    do {
        dataTypes = scanf(stringFormat, pInput, &closingChar);
        if (dataTypes != 2 || closingChar != '\n') {
            clearInput();
            printf("%s\n", errorMessage);
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
 * @param errorMessage Pointer to the Error message to be given to the console.
 */
void
repeatGetInteger(int *pInput, String63 errorMessage){
    int isIncorrectInput = 1;
    char closingChar;

    do {
        if(scanf("%d%c", pInput, &closingChar) != 2 || closingChar != '\n'){
            clearInput();
            printf("%s\n", errorMessage);
        }
        else
            isIncorrectInput = 0;
    }
    while(isIncorrectInput);
}

/**
 * @brief Prints ASCII graphics found in ASCII_Art.txt
 * 
 * @param graphicsID Name of the graphic to render.
 * @return 0 if No error, 1 if File not Found, 2 if EOF, 3 - Misalignment
 * Pre-condition: Given graphicsID must be a valid one.
 */
ErrorInt 
printGraphics(String15 graphicsID){
    String255 graphicsData;
    String15 stringGraphicHeight;
    String15 scannedGraphicId;
    String15 prevGraphicHeight;
    String15 prevGraphicId;
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
        isPreviousMetadataSame = strcmp(prevGraphicHeight, stringGraphicHeight) == 0 && strcmp(prevGraphicId, scannedGraphicId) == 0;
        isSameGraphicId = strcmp(scannedGraphicId, graphicsID) == 0;
        
        haveNotFoundGraphic = 0; // Assume a closing statement like an error or found the graphics
       
        if (isEndOfFile) {
            printf("Program Error: EOF. Graphics Not found.\n");
            errorCode = 2;
        } else if (isPreviousMetadataSame) {
            printf("Program Error: Misalignment. Next Graphics Metadata not found.");
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

// |===| Essential Functions Section |=====================|


// |===| PASSENGER CMD SECTION |=====================|

void userEmbarkation(){
    printSingleColorText(BACKGROUND_GREEN, "User creates an embarkation trip.");
}

// |===| ADMIN CMD SECTION ==========================|

void adminNoOfPassenger(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Admin views a Trip's no. of Passenger");
}

void adminCountPassengerDropOff(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Admin counts number of Passenger in a drop-off.");
}

void adminViewPassengerInfo(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Admin views the passenger info.");
}

void adminSearchPassenger(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Admin searches the passenger in a trip.");
}

void adminEmbarkation(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Admin creates an embarkation trip.");
}

// |===| MENU SECTION |=============================|

/**
 * TODO fill it functions accessible to a passenger based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 * 
 */
void passengerMenu(){
    printSingleColorText( BACKGROUND_GREEN, "Entered Passenger Space.");
}

/**
 * TODO fill it functions accessible to an admin based on the diagram.
 * @brief Contains the functions accessible to a regular user 
 * 
 */
void adminMenu(){
    printSingleColorText( BACKGROUND_GREEN | BACKGROUND_RED, "Entered Admin Space.");
    printf("\n");
}

/********************************************************************************************************* 
 * This is to certify that this project is our own work, based on our personal efforts in studying and 
 * applying the concepts learned. We have constructed the functions and their respective algorithms and 
 * corresponding code by ourselves. The program was run, tested, and debugged by our own efforts. 
 * 
 * We further certify that we have not copied in part or whole or otherwise plagiarized the work of 
 * other students and/or persons. 
 *                                                      ROAN CEDRIC V. CAMPO,           DLSU ID# 12305936
 *                                                      WANDA JUDE R. DE LA CALZADA,    DLSU ID# 
*********************************************************************************************************/


