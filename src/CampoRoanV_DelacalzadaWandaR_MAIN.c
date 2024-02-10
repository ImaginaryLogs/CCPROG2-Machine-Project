#include "CampoRoanV_DelacalzadaWandaR_Functions.h"

/**
 * TODO Fill it functions accessible to a general user based on the diagram.
 * TODO Password for admin.
 * TODO Use admin and passenger menu.
 * TODO Sign the certify agreement at the end
 * @brief Contains the functions accessible to a regular user 
 * 
 */
int 
main(){
	HANDLE hConsoleScreen = GetStdHandle(STD_OUTPUT_HANDLE);
	
	// Replace all of this after if you want
	int isInUserMenu = 1;
	int isUsingApplication = 1;
	int isAdmin = 0;
	String15 input;
	String31 errorMessage = "\nError! try again.\n";
	ErrorInt isFound = 0;

	while(isUsingApplication){
		printf("Test");
		isFound = printGraphics("Bus Express");
		printf("\nErrors: %d\n", isFound);
		printf("Input String: ");
		repeatGetString(input, 15, errorMessage);
		SetConsoleTextAttribute(hConsoleScreen, FOREGROUND_INTENSITY | BACKGROUND_RED);

		printf("Output String: %s", input);
		SetConsoleTextAttribute(hConsoleScreen, FOREGROUND_WHITE | BACKGROUND_BLACK);
		printf("\n");

		getchar();
	}

	return 0;
}
