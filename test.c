#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef char String15[16];

struct Student{
    struct NameField{
        String15 lastname;
        String15 firstname;
    } Name;
};

struct Student *
initializeNames(struct Student input[], int size){
    int i;
    String15 name1 = "Jimmy";
    String15 a;
    String15 name2 = "Neutron";
    for(i = 0; i < size; i++){
        name1[strlen(name1) + (int) strlen(itoa(i, a, 10)) - 1] = '\0';
        strcat(name1, itoa(i, a, 10));
        name2[strlen(name2) + (int) strlen(itoa(i, a, 10)) - 1] = '\0';
        strcat(name2, itoa(i, a, 10));
    	strcpy((*(input + i)).Name.firstname, name1);
    	strcpy((*(input + i)).Name.lastname, name2);	
	}

    return input;
}

void
printNames(struct Student input[], int size){
    int i;
    for(i = 0; i < size; i++){
        printf("%02d) %s %s\n", i, (*(input + i)).Name.firstname, (*(input + i)).Name.lastname);
    }
}


int main(){
	struct Student Class[3];
	initializeNames(Class, 3);
	printNames(Class, 3);

    return 0;
}
