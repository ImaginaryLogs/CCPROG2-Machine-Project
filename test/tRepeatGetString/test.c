#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
//CREDITS: https://patorjk.com/software/taag/#p=display&f=Graceful&t=%22%20THE%20QUICK%22%0A%22%20BROWN%20FOX%20JUMPS%20%22%0A%22%20OVER%20THE%20LAZY%20DOG%20%22%0A%22%20%5CN%20%22%20%2F%2F%20LITERALLY
int testValues(char *strInput1, char *strInput2, char *strInput3, char *strInput4){
    String15 strCond1 = "THE QUICK";
    String15 strCond2 = "BROWN FOX JUMPS";
    String15 strCond3 = "OVER THE LAZY";
    String15 strCond4 = "";
    
    int condition1 = !strcmp(strInput1, strCond1);
    int condition2 = !strcmp(strInput2, strCond2);
    int condition3 = !strcmp(strInput3, strCond3);
    int condition4 = !strcmp(strInput4, strCond4);
    int passedValues = 0;

    printf("\nReceived Values:\n");
    printf(" %d - Original:\"%s\" \n \t vs Input:\"%s\" \n", condition1, strInput1, strCond1);
    printf(" %d - Original:\"%s\" \n \t vs Input:\"%s\" \n", condition2, strInput2, strCond2);
    printf(" %d - Original:\"%s\" \n \t vs Input:\"%s\" \n", condition3, strInput3, strCond3);
    printf(" %d - Original:\"%s\" \n \t vs Input:\"%s\" \n", condition4, strInput4, strCond4);

    passedValues = condition1 + condition2 + condition3 + condition4;

    return passedValues;
}                            


int main(){
    String15 strInput1, strInput2, strInput3, strInput4;
    String15 promptMessage = "\t> Word: ";
    String31 errorMessage = "Wrong Data Type or Error.";
    ErrorInt passes = 0;
    
    printf("Please input these sentences per line before testing:\n");
    printf("THE QUICK\nBROWN FOX JUMPS\nOVER THE LAZY DOG\nOVER THE LAZY\n\n");
    printf("\nCaptcha Solver! Turn on CAPS!\n");
    printf("\n#==|[1]: Input a valid string that fits below the length\n");
    repeatGetString(strInput1, 15, "TheQuick", promptMessage, errorMessage);
    
    printf("\n#==|[2]: Input a valid string that fits at the length.\n");
    repeatGetString(strInput2, 15, "BrownFoxJumps", promptMessage, errorMessage);

    printf("\n#==|[3]: Input a string longer than the given length. Else, input \"OVER THE LAZY\".\n");
    repeatGetString(strInput3, 15, "OverTheLazy", promptMessage, errorMessage);
    
    printf("\n#==|[4]: Input an empty string. If it did nothing, input \"\\n\" literally \n");
    repeatGetString(strInput4, 15, "SlashN", promptMessage, errorMessage);
    
    passes = testValues(strInput1, strInput2, strInput3, strInput4);

    printf("Passes: %d\n", passes);
    return 0;
}