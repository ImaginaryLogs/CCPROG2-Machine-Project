#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main() {
    char errorMess[] = "Invalid input";

    int rightInput = 2;
    int wrongInput = 1;
    char closingChar1 = '\n';
    char closingChar2 = ' ';

    int result1 = isInputSuccesful(rightInput, closingChar1, errorMess);
    int result2 = isInputSuccesful(wrongInput, closingChar2, errorMess);

    printf("Wrong test: %s\n", result2 ? "success" : "failure");    
    printf("Right test: %s\n", result1 ? "success" : "failure");
    
    return 0;
}