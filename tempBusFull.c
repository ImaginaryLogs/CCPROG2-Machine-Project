#include <stdio.h>
#include <string.h>

void printSeats13(/*int passCount, int tripNo*/) {
    char seats13[5][3];
    int passCount;
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
    } 
    //  need to make sure that seats13[4][1] seats13[4][2] are null always
    else if (passCount != 0) {
        printf("13 - Seater Bus: %d Passenger/s\n", passCount);

        for (i = 0; i < passCount; i++) {
            printf("X   ");
        }

        for (j = 0; j < 3 - passCount; j++) {
            printf("O   ");
        }
        printf("\n");
        
        for (i = 1; i < 5; i++) {
            for (j = 0; j < 3; j++) {
                printf("O   ");
            }
            printf("\n");
        }
    }
}

void printSeats16(/*int passCount, int tripNo*/) { 
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
    }
}