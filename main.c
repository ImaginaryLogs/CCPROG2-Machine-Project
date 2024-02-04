#include <stdio.h>
#include "funcs.h"

int 
main(){
	int isInUserMenu = 1;
	int isUsingApplication = 1;
	int isAdmin = 0;
	ErrorInt isFound = 0;

	while(isUsingApplication){
		ErrorInt isFound = printGraphics("Bus Express");
		printf("\nErrors: %d\n", isFound);
		getchar();
	}

	return 0;
}
