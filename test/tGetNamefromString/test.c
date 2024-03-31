#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int main() {
    char strName[] = "DLC, Wanda R.";
    struct NameField name = GetNameFieldFromString(strName);

    printf("%s %c %s", name.firstName, name.midI, name.lastName);
}