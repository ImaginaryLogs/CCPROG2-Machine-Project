#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"
#include <stdio.h>

int main (){
    int a = 0;
    int b = 1;
    swapInt(&a, &b);
    printf("%d %d", a, b);

    char str1 = "bus";
    char str2 = "express";
    swapStr(&str1, &str2);
    printf("%s %s", str1, str2);
}