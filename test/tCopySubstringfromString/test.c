#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main() {
    char strBig[] = "this is a string";
    char strSub[20];

    copySubstringFromString(strSub, strBig, 3, strlen(strBig) - 3);
    printf("%s", strSub);
}