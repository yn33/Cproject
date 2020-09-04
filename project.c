#include "project.h"

int amountOfSpaces(char* input) {
    int counter = 0;
    char *current = strchr(input, ' ');
    while(current) {
        current = current + 1;
        current = strchr(current, ' ');
        counter++;
    }
    return counter;
}


int main() {

    int quit = 0;
    struct driver *database = calloc(1, sizeof(struct driver));
    int amountOfDrivers = 0;
    while(!quit) {
        char *inputString = calloc(1, 44*sizeof(char));
        fgets(inputString, 44*sizeof(char), stdin);
        char header = *inputString;
        int spaces = amountOfSpaces(inputString);
        if(header == 'A') {
            if(spaces == 2) {
                
                database = addDriver(database, inputString, amountOfDrivers);
                if(database == NULL) {
                    freeDatabase(database, amountOfDrivers);       
                    printf("%s", "Memory error");               
                    return 0;
                    
                } else {
                    amountOfDrivers++;
                }
                
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else if(header == 'U') {
            if(spaces == 4) {
                addTime(database, inputString, amountOfDrivers);
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else if(header == 'L') {
            if(spaces == 0) {
                printDrivers(database, amountOfDrivers);
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else if(header == 'W') {
            if(spaces == 1) {
                struct driver *ptr = saveToFile(database, amountOfDrivers, inputString);
                if(ptr == NULL) {
                    printf("%s", "Opening file failed.\n");
                } 
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else if(header == 'O') {
            if(spaces == 1) {

                struct driver *newDatabase = calloc(1, sizeof(struct driver));
                struct driver **newDatabasePointer = &newDatabase;
                if(newDatabase != NULL) {
                    int newLength = readFromFile(newDatabasePointer, inputString);
                    if(newLength < 0) {
                        printf("%s", "Opening file failed.\n");
                        free(newDatabase);
                    } else {
                        freeDatabase(database, amountOfDrivers);
                        database = *newDatabasePointer;
                        amountOfDrivers = newLength;
                    }
                } else {
                    printf("%s", "Memory error.\n");
                }
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else if(header == 'Q') {
            if(spaces == 0) {
                freeDatabase(database, amountOfDrivers);
                quit = 1;
            } else {
                printf("%s", "Wrong format.\n");
            }
        } else {
            printf("%s", "Unknown command.\n");
        }

        free(inputString);
    }

    return 0;
}