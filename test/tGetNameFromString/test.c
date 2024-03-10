#include "../../src/CampoRoanV_DelacalzadaWandaR_Functions.h"


int main(){
    struct NameField outputName1;
    String63 Name = "";
    String63 strLastName = "";
    String63 strFirstName = "";
    char middleI;
    
    printf("Input last name, first name, and middle initial:\n");
    scanf("%s %s %c", strLastName, strFirstName, &middleI);
    clearInput();
    strcat(Name, removeNewline(strLastName));
    strcat(Name, ", ");
    strcat(Name, removeNewline(strFirstName));
    strcat(Name, " ");
    Name[strlen(Name)] = middleI;
    Name[strlen(Name)] = '\0';
    strcat(Name, ".");
    

    outputName1 = GetNameFromString(Name);
    printf("Received: \"%s\", \"%s\" \'%c\'.\n", strLastName, strFirstName, middleI);
    printf("Translation: \"%s\"\n", Name);
    printf("Result: \"%s\", \"%s\" \'%c\'.\n", outputName1.lastName, outputName1.firstName, outputName1.midI == '\0' ? ' ' : outputName1.midI);

    // Erase name
    strcpy(Name, "");
    strcpy(outputName1.lastName, "");
    strcpy(outputName1.firstName, "");
    outputName1.midI = '\0';

    printf("Input last name and first name only:\n");
    scanf("%s %s", strLastName, strFirstName);
    clearInput();
    strcat(Name, removeNewline(strLastName));
    strcat(Name, ", ");
    strcat(Name, removeNewline(strFirstName));

    outputName1 = GetNameFromString(Name);
    printf("Received: \"%s\", \"%s\" .\n", strLastName, strFirstName);
    printf("Translation: \"%s\"\n", Name);
    printf("Result: \"%s\", \"%s\" \'%c\'.\n", outputName1.lastName, outputName1.firstName, outputName1.midI == '\0' ? ' ' : outputName1.midI);
    
    getchar();
    return 0;

}