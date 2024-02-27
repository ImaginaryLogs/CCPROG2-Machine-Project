#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int testValues(char cInput1, char cInput2, char cInput3){
    int passedValues = 0;

    printf("\nReceived Values:\n");
    printf(" %d - %c \n", cInput1 == 'S', cInput1);
    printf(" %d - %c \n", cInput2 == 'C', cInput2);
    printf(" %d - %c \n", cInput3 == 'T', cInput3);

    passedValues = cInput1 == 'S' + cInput2 == 'C' + cInput3 == 'T';

    return passedValues;
}                            


int main(){
    char cInput1, cInput2, cInput3;
    String15 promptMessage = "\t> Shape: ";
    String31 errorMessage = "Wrong Data Type.";
    ErrorInt errors = 0;
    
    printf("Please input these sentences per line before testing:\n");
    printf("S\nCircle\nC\n123\nT\n");
    printf("#==|[1]: Input char\n");
    repeatGetChar(&cInput1, "Square", promptMessage, errorMessage);
    
    printf("\n#==|[2]: Input string\n");
    repeatGetChar(&cInput2, "Circle", promptMessage, errorMessage);
    
    printf("\n#==|[3]: Input not char nor string\n");
    repeatGetChar(&cInput3, "Triangle", promptMessage, errorMessage);

    errors = testValues(cInput1, cInput2, cInput3);

    printf("Passes: %d\n", errors);
    return 0;
}