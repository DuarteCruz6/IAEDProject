#include "parking.h"

//main function that receives the commands
int main() {
    char input[BUFSIZ];
    char command, *rest_command;
    while (1) {
        fgets(input, BUFSIZ, stdin);
        input[strcspn(input, "\n")] = 0;
        command = input[0];
        rest_command = (char *)malloc(strlen(input)+1);
        strcpy(rest_command, input + 1);
        switch (command) {
            case 'q':
                free_all_parks();
                free_all_cars();
                return 0;
            case 'p':
                create_park(rest_command);
                break;
            case 'e':
                new_vehicle(rest_command);
                break;
            case 's':
                old_vehicle(rest_command);
                break;
            case 'v':
                check_vehicle(rest_command);
                break;
            case 'f':
                bill_day(rest_command);
                break;
            case 'r':
                remove_park(rest_command);
                break;
            default:    
                printf("Invalid command!\n");
        }
    free(rest_command);    
    }
    return 0;
}