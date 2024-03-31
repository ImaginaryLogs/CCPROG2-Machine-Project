#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    int certain = 0;
    int valid = 0;
    char errorMess[] = "invalid input";

    validateUserInput(&certain, &valid, errorMess);
    printf("isChoiceCertain: %d, isChoiceValidByUser: %d\n", certain, valid);
    validateUserInput(&certain, &valid, errorMess);
    printf("isChoiceCertain: %d, isChoiceValidByUser: %d\n", certain, valid);
    validateUserInput(&certain, &valid, errorMess);
    printf("isChoiceCertain: %d, isChoiceValidByUser: %d\n", certain, valid);
}