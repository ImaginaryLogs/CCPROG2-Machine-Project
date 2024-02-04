#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef char String255[256];
typedef char String63[64];
typedef char String15[16];

typedef int ErrorInt;


/**
 * @brief Clears the input for anything not a new line.
 * 
 */
void 
clearInput(){
    while (getchar() != '\n'); // Get character while its not a new line and do nothing.
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

    do {
        switch(size){
            case 255:
                dataTypes = scanf("%255[^\n]%c", pInput, &closingChar);
                break;
            case 63:
                dataTypes = scanf("%63[^\n]%c", pInput, &closingChar);
                break;
            case 15:
                dataTypes = scanf("%15[^\n]%c", pInput, &closingChar);
                break;
        }

        if(dataTypes != 2 || closingChar != '\n'){
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
 * @brief 
 * 
 * @param requiredGraphicId Name of the graphic to render.
 * @return ErrorInt 0: No error, 1: EOF error, 2: Misalignment error.
 */
ErrorInt 
printGraphics(String15 requiredGraphicId){
    ErrorInt errorCode = 0;
    String15 stringGraphicHeight;
    String15 scannedGraphicId;
    String15 prevGraphicHeight;
    String15 prevGraphicId;
    String255 graphicsData;
    FILE *graphicsFile;

    char idScanned;
    int graphicHeight = 0;
    int haveNotFoundGraphic = 1;
    int gLine;
    // 
    int isEndOfFile = 0;
    int isPreviousMetadataSame = 0;
    int isSameGraphicId = 0;

    graphicsFile = fopen("graphicsFile.txt", "r");
    do {
        //Format: "ID:<graphic id>, <height of graphic>;"
        idScanned = fscanf(graphicsFile, "ID:%15[^,], %[^;];\n", scannedGraphicId, &stringGraphicHeight);

        // printf("Scanned Id: %s, Height: %s\n", scannedGraphicId, stringGraphicHeight);
        
        isEndOfFile = idScanned == EOF;
        isPreviousMetadataSame = strcmp(prevGraphicHeight, stringGraphicHeight) == 0 && 
                                 strcmp(prevGraphicId,scannedGraphicId) == 0;
        isSameGraphicId = strcmp(scannedGraphicId, requiredGraphicId) == 0;
        graphicHeight = atoi(stringGraphicHeight); // converts strings into integer when possible.

        haveNotFoundGraphic = 0; // assume its an error
        if (isEndOfFile) {
            printf("Program Error: EOF. Graphics Not found.\n");
            errorCode = 1;
        } else if (isPreviousMetadataSame) {
            printf("Program Error: Misalignment. Next Graphics Metadata not found.");
            errorCode = 2;
        } else if (isSameGraphicId) {
            for (gLine = 0; gLine < graphicHeight; gLine++) {
                fgets(graphicsData, 256, graphicsFile);
                printf("%s", graphicsData);
            }
        } else {
            haveNotFoundGraphic = 1;
        }

        for (int gLine = 0; gLine < graphicHeight; gLine++) 
            fgets(graphicsData, 256, graphicsFile);
        
        strcpy(prevGraphicHeight, stringGraphicHeight);
        strcpy(prevGraphicId,scannedGraphicId);
    }
    while(haveNotFoundGraphic);
    fclose(graphicsFile);
    return errorCode;
}


void userMenu(){
    String15 userChoice;
    String63 wrongChoice = "Please type recognizable inputs!";
    repeatGetString(userChoice, 15, wrongChoice);
}

void passengerMenu(){

}

void adminMenu(){

}