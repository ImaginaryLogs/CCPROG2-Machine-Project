#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int testValues(int nInput1, int nInput2, int nInput3, int nInput4){
    int passedValues = 0;

    printf("\nReceived Values:\n");
    printf(" %d - %d \n", nInput1 == 1, nInput1);
    printf(" %d - %d \n", nInput2 == 2, nInput2);
    printf(" %d - %d \n", nInput3 == 3, nInput3);
    printf(" %d - %d \n", nInput4 == 4, nInput4);

    passedValues = (nInput1 == 1) + (nInput2 == 2) + (nInput3 == 3) + (nInput4 == 4);

    return passedValues;
}                            


int main(){
    int nInput1, nInput2, nInput3, nInput4;
    String15 promptMessage = "\t> Number: ";
    String31 errorMessage = "Wrong Data Type.";
    ErrorInt passes = 0;
    
    printf("Please input these sentences per line before testing:\n");
    printf("1\n3147483648\n2\n123.2\n3\n");
    printf("#==|[1]: Input integer\n");
    repeatGetInteger(&nInput1, "One", promptMessage, errorMessage);
    
    printf("\n#==|[2]: Input number beyond the range of integer\n");
    repeatGetInteger(&nInput2, "Two", promptMessage, errorMessage);
    
    printf("\n#==|[3]: Input not integer but number\n");
    repeatGetInteger(&nInput3, "Three", promptMessage, errorMessage);

    printf("\n#==|[3]: Input not intger\n");
    repeatGetInteger(&nInput4, "Four", promptMessage, errorMessage);

    passes = testValues(nInput1, nInput2, nInput3, nInput4);

    printf("Passes: %d\n", passes);
    return 0;
}