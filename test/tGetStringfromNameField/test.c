#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main(){
    struct NameField name = {"DLC", "Wanda", 'R'};
    char strName[100];

    printf("%s",GetStringFromNameField(strName, name));
}
