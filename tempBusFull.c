#include <stdio.h>
#include <string.h>
#include "src/CampoRoanV_DelacalzadaWandaR_Functions.h"

void printSeats13(int passCount) {  
    char seats13[5][3];
    int i, j;

    if (passCount == 0) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                seats13[i][j] = 'O';
            }
        }

        seats13[4][1] = '\0';
        seats13[4][2] = '\0';

        printf("13 - Seater Bus: All Vacant\n");
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                printf("%c  ", seats13[i][j]);
            }
            printf("\n");
        }
    } else if (passCount > 0 && passCount < 14) {
        printf("13 - Seater Bus: %d Passenger/s\n", passCount);

        for (i = 1; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                seats13[i][j] = 'O';
            }
        }
        
        for (i = 0; i < passCount; i++) {
            seats13[0][i] = 'X';
        }
        
        seats13[4][1] = '\0';
        seats13[4][2] = '\0';
        
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                printf("%c  ", seats13[i][j]);
            }
            printf("\n");
        }
    } else if (passCount != 0 || passCount > 13)
        repeatGetChar(&passCount);
}

void printSeats16(int passCount) { 
    char seats16[4][4];
    int passCount;
    int i, j;

    if (passCount == 0) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                seats16[i][j] = 'O';
            }
        }

        printf("\n16 - Seater Bus: All Vacant\n");
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("\n");
        }
    } else if (passCount > 0 && passCount < 17) {
        printf("16 - Seater Bus: %d Passenger/s\n", passCount);

        for (i = 1; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                seats16[i][j] = 'O';
            }
        }
        
        for (i = 0; i < passCount; i++) {
            seats16[0][i] = 'X';
        }
        
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                printf("%c  ", seats16[i][j]);
            }
            printf("\n");
        }
    } else if (passCount != 0 || passCount > 16)
        repeatGetChar(&passCount);
}