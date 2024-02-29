#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
// CREDITS: https://patorjk.com/software/taag/#p=display&f=Big%20Money-sw&t=1%0A-%202%0A3

int testValues(int nInput1, int nInput2, int nInput3){
    int passedValues = 0;
    int condition1 = nInput1 == 1;
    int condition2 = nInput2 == -2;
    int condition3 = nInput3 == 3;
    
    printf("\nReceived Values:\n");
    printf(" %d - %d \n", condition1, nInput1);
    printf(" %d - %d \n", condition2, nInput2);
    printf(" %d - %d \n", condition3, nInput3);

    passedValues = condition1 + condition2 + condition3;
    return passedValues;
}                            


int main(){
    int nInput1, nInput2, nInput3, nInput4;
    String15 promptMessage = "\t> Number: ";
    String31 errorMessage = "Wrong Data Type.";
    ErrorInt passes = 0;
    
    printf("Please input these text per line before testing:\n");
    printf("1\n123.2\n-2\nThree\n3\n");
    printf("\n#==|[1]: Input the integer on the screen\n");
    String15 graphicCode1 = "One";
    repeatGetInteger(&nInput1, graphicCode1, promptMessage, errorMessage);
    
    printf("\n#==|[2]: Input a float then -2\n");
    String15 graphicCode2 = "Two";
    repeatGetInteger(&nInput2, graphicCode2, promptMessage, errorMessage);

    printf("\n#==|[3]: Input not intger then 3\n");
    String15 graphicCode3 = "Three";
    repeatGetInteger(&nInput3, graphicCode3, promptMessage, errorMessage);

    passes = testValues(nInput1, nInput2, nInput3);

    printf("Passes: %d\n", passes);
    return 0;
}