#include <stdio.h>
#include <string.h>

void printSeats13(int passCount/*, int tripNo*/) {
    char seats13[5][3];
    if (passCount == 0) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                seats13[i][j] = 'O';
            }
        }
        
        seats13[4][1] = '\0';
        seats13[4][2] = '\0';
        
        printf("13 - Seater Bus: All Vacant\n");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                printf("%c  ", seats13[i][j]);
            }
            printf("\n");
        }
    }
}

void printSeats13(int passCount/*, int tripNo*/) { 
    char seats16[4][4];
    
    if (passCount == 0) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                seats16[i][j] = 'O';
            }
        }

        printf("\n16 - Seater Bus: All Vacant\n");
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("\n");
        }
    }
}