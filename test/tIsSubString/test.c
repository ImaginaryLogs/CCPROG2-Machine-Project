#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"

int test(String15 Name[], String15 Nickname[], int size){
    int i, passes = 0;
    for(i = 0; i < size; i++){
        printf("%c) Is \"%s\" inside \"%s\"?\n", 'A'+ i, Nickname[i], Name[i]);
        printf("\tResult: %d\n", isSubString(Nickname[i], Name[i]));
        passes += isSubString(Nickname[i], Name[i]);

    }
    return passes;
};


int main(){
    int passes;
    String15 Name[5] = {"Monkey D. Luffy", "Roronoa Zoro", "Nami", "Usopp\t", "Vinsmoke Sanji"};
    String15 Nickname[5] = {"Luffy", "Zorojuro", "a", "\t", ""};

    passes = test(Name, Nickname, 5);
    printf("Passes %d\n", passes);

    return 0;
}