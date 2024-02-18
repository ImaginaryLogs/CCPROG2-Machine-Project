/*
hi roan, please check if this is alright na with you,
corrections are very much welcomed
*/

//menu screen for all users
//menu for each user [to be checked post function]
//admin and passenger

#include <stdio.h>
#include <string.h>
typedef char input;

int main ()
{
    //started on Feb 10, 2024

    
    input main, user, admin;
    input tripNo[6];
    input pass[256];
    
    do {
        printf("--- MAIN MENU ---\n");          //AYUSIN KO TOH DAPAT MAS MAGING AESTHETIC #ASCII (lol paturo pls)
        printf("a. Admin\n");
        printf("b. Passenger\n");
        printf("c. Exit\n");
        scanf("%c", &main);
        
        
        switch (main) {
            case 'a':
                do {
                    printf("Admin Menu\n");
                    printf("a. Proceed to Admin Options\n");
                    printf("b. Go Back\n");
                    scanf("%c", &admin);
                    
                    switch (admin) {
                        case 'a':
                            printf("Enter Password: ");
                            scanf("%s", pass);
                            
                           /*
                            temp code for admin menu
                            
                            if (strcmp(pass, realPass) == 0) {
                                printf("Admin Menu\n");
                                printf("a. View Passengers\n");
                                printf("b. View Passenger Count at Stops\n");
                                printf("c. View Passenger Information\n");
                                printf("d. Load Passenger Information\n");
                                printf("e. Search Passenger\n");
                                printf("f. Inform User\n");
                                printf("Go Back\n");
                                scanf("%c", &admin);
                            
                                do {
                                    switch (admin) {
                                        case 'a':
                                                1. It will ask the user for the Trip Number.
                                                2. This will display the current count for a specific trip.
                                                3. Display will be as seen above.
                                                4. All taken seats will be marked with an “X” while open/available seats will be marked with an “O”.

                                            printf("Enter Trip Number: ");
                                            scanf("%s", tripNo);
                                            displayTripCount
                            
                                            break;
                            
                                        case 'b':
                                                1. Ask the user for the Trip Number.
                                                2. This will display a list of the stops that the passengers picked and show how many will be getting off at those stops.
                            
                                            printf("Enter Trip Number: ");
                                            scanf("%s", tripNo);
                                            displayTripStop;

                                            break;

                                        case 'c':
                                                1. This feature will ask for the Trip Number.
                                                2. It will then display a list of the names, ID Numbers and priority numbers of the passengers of the specific trip sorted by priority number(highest to lowest).
                            
                                            printf("Enter Trip Number: ");
                                            scanf("%s", tripNo);
                                            displayPassengerArr;
                            
                                            break;

                                        case 'd':
                                                1. This will be an alternative to manually inputting passenger information.
                                                2. It will read the trip number as well as the passenger info from a text file.
                                        
                                            //not sure how to do this pa sorry

                                            break;

                                        case 'e':
                                                1. This feature will allow the user to search for passengers by their last name.
                                                2. Automatically save current information to a text file when the program is terminated.
                                                3. View any recent trip files via a file load feature.
                                                4. Using this feature will display all information of the loaded trip file.
                                            
                                            input lastName[256];
                                            printf("Enter Last Name of Passenger: ");
                                            scanf("%s" lastName);
                            
                                            still unsure of how to do 2-4

                                            break;

                                        case 'f':
                                                1. When the user picks a specific trip, if the trip is full, he/she will be notified via display message.
                            
                                            printf("Enter Trip Number: ");
                                            scanf("%s", tripNo);
                                            isTripFull(tripNo);
                            
                                            break;
                                        
                                        case 'g':
                                            break;

                                        default:
                                            printf("Inavlid input, please select again\n");
                                            break;

                            } while (admin != 'g')
                            */

                            break;
                            
                        case 'b':
                            break;
                            
                        default:
                            printf("Invalid input, please select again.\n");
                            break;
                    }
                } while (admin != 'b');
                break;
                
            case 'b':
                do {
                    printf("Passnger Menu\n");
                    printf("a. Search for Trip\n");
                    printf("b. Go Back\n"); //go back to main manu
                    scanf("%c", &user);
                    
                    switch (user) {
                        case 'a':
                            printf("Enter Trip Number: ");
                            scanf("%s", tripNo);
                            
                            /*
                             temp code: to be replaced
                             
                             tripNotFull = isTripFull(tripNo);
                             
                             if (tripFull) {
                             assignTrip(tripNo);
                             printf ("You are assigned to trip no: %s", tripNo);
                             } else {
                             printf("Trip is full\n");
                             removeLowPrio();
                             }
                             */
                            
                            break;
                            
                        case 'b':
                            break;
                            
                        default:
                            printf("Invalid input, please select again.\n");
                            break;
                    }
                } while (user != 'b');
                
                break;
                
                
                
                
            case 'c':
                printf("Exiting...\n");
                return 0;
                break;
                
            default: //loop back to main menu
                printf("Invalid input, please select again.\n");
                break;
        }
    } while (main != 'c');
}
