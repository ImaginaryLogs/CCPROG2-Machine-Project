
#include <stdio.h>
#include <string.h>
#include "CampoRoanV_DelacalzadaWandaR_Functions.h"
typedef char tripNo[7]; // global variable - we can define it in funcsh.h

void passwordMenu(int *isChoosingAdminCmds, int *isInputingPass, char *realPass){
    String255 inputPass;
    char main;
    String63 errorMessage = "Error, not a string.";

    
    repeatGetString(inputPass, 255, "PassMenu", "\t> Password:", errorMessage);
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

void menuAdmin2(){
    int isInputing = TRUE;
    int isChoosingAdminCmds = FALSE;
    char userInput;
    
    String255 realPass = "Admin123"; // to be changed
    String63 errorMessage = "Please input a valid admin cmd.\n";

    do {
        passwordMenu(&isChoosingAdminCmds, &isInputing, realPass);
    }
    while (isInputing && !isChoosingAdminCmds);

    

    while (isChoosingAdminCmds){
        system("cls");
        repeatGetChar(&userInput, "AdminMenu", "\t> Command: ", errorMessage);
        switch(userInput){
            case 'g':
                isChoosingAdminCmds = FALSE;
        }
    } 
    system("cls");
}

void menuPassenger()
{
    char inputPassMenu;
    char tripNo[6];
    String63 errorMessage = "Dear Passenger, Please select the following valid cmds\n";

    do {
        repeatGetChar(&inputPassMenu, "PassengerMenu", "\t> Action:", errorMessage);
        switch(inputPassMenu) {
            case 'a':
                printf("[O] Enter Trip Number: \n");
                break;

            case 'b':
                break;

            default:
                printErrorMessage(errorMessage);
                break;
        }
    } while (inputPassMenu != 'b');
    system("cls");
}

int main()
{
    char mainInput;
    printGraphics("Bus Express");
    do {
        repeatGetChar(&mainInput, "MainMenu", "\t> Choice: ", "Dear user please input again correctly.");
        printf("\n");
        system("cls");
        switch (mainInput) {
            case 'a':
                menuAdmin2();
                break;
            case 'b':
                menuPassenger();
                break;
            case 'c':
                printSingleColorText(FG_RED, "[x] Terminating...\n\tPress any button again.");
                getchar();
                break;
            default: // loop back to main menu
                printErrorMessage("Invalid command, please select again.\n");
                break;
        }
    } while (mainInput != 'c');
    return 0;
}
