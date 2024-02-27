#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
int testValues(char cInput1, char *strInput2, char *strInput3, char cInput4, char cInput5){
    int hasPassedConditions = 0;

    printf("\nReceived Values:\n");
    printf(" %d - %c \n", cInput1 == 'U', cInput1);
    printf(" %d - %s \n", !strcmp(strInput2, "Tres"), strInput2);
    printf(" %d - %s \n", !strcmp(strInput3, "Cinco"), strInput3);
    printf(" %d - %c \n", cInput4 == 'S', cInput4);
    printf(" %d - %c \n", cInput5 == 'N', cInput5);

    hasPassedConditions =   cInput1 == 'U' &&
                            !strcmp(strInput2, "Tres") &&
                            !strcmp(strInput3, "Cinco") &&
                            cInput4 == 'S' &&
                            cInput5 == 'N';

    printf("#==// ");
    if (hasPassedConditions)
        printf("Passed\n\n");
    else
        printf("Failed\n\n");

    return hasPassedConditions;
}

int main(){
    char cInput1, cInput4, cInput5;
    ErrorInt errors = 0;
    String31 strInput2 = "";
    String31 strInput3 = "";
    printf("Please input these sentences per line before testing:\n");
    printf("Uno Dos\nTres Quatro\nCinco Seis\nSiete Ocho\nNueve\n");
    printf("#==\\\\ [1]: With the function\n");
    printf("Insert the given names: \n");
    scanf("%c", &cInput1);
    clearInput();
    scanf("%s", strInput2);
    clearInput();
    scanf("%s", strInput3);
    clearInput();
    scanf("%c", &cInput4);
    clearInput();
    scanf("%c", &cInput5);
    clearInput();

    errors = !testValues(cInput1, strInput2, strInput3, cInput4, cInput5);

    printf("#==\\\\ [2]: Without the function\n");
    printf("Insert the given names: \n");
    scanf("%c", &cInput1);
    scanf("%s", strInput2);
    scanf("%s", strInput3);
    scanf("%c", &cInput4);
    scanf("%c", &cInput5);

    errors = testValues(cInput1, strInput2, strInput3, cInput4, cInput5);
    printf("If failed, it is as expected.\n");

    printf("Errors: %d\n", errors);
    return 0;
}