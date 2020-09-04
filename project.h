#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct driver 
{
    char *name;
    char *team;
    int hours;
    int minutes;
    int seconds;
};

struct driver *addDriver(struct driver *database, char* input, int length) {


    char *newString = calloc(44, sizeof(char));
    char *ptr = newString;

    if(newString == NULL) {
        return NULL;
    }
    //starts from name
    strcpy(newString, input + 2);

    char *newName = calloc(21, sizeof(char));
    if(newName == NULL) {
        free(newString);
        return NULL;
    }

    char *newTeam = calloc(21, sizeof(char));
    if(newName == NULL) {
        free(newString);
        free(newName);
        return NULL;
    }

    struct driver *newDriver = calloc(1, sizeof(struct driver));
    if(newDriver == NULL) {
        free(newString);
        free(newName);
        free(newTeam);
        return NULL;
    }


    unsigned int i = 0;

    //copy name to newName
    while(newString[i] != ' ') {
        newName[i] = newString[i];
        i++;
    }
    newName[i] = 0;
    newString = newString + i + 1;
    i = 0;
    while(newString[i] != 0 && newString[i] != '\n') {
        newTeam[i] = newString[i];
        i++;
    }
    newTeam[i] = 0;


    //copy to newDriver
    newDriver->name = newName;
    newDriver->team = newTeam;
    newDriver->hours = 0;
    newDriver->minutes = 0;
    newDriver->seconds = 0;


    database = realloc(database, (length + 1)*sizeof(struct driver));

    if(database == NULL) {
        free(newString);
        free(newName);
        free(newTeam);
        free(newDriver);
        return NULL;
    }

    database[length] = *newDriver;

    free(newDriver);
    free(ptr);

    return database;
}

struct driver *addTime(struct driver *database, char *input, int length) {

    char *newString = calloc(44, sizeof(char));
    if(newString == NULL) {
        return NULL;
    }
    strcpy(newString, input + 2);

    char *newName = calloc(20, sizeof(char*));
    if(newName == NULL) {
        free(newString);
        return NULL;
    }


    int i = 0;

    //copy name to newName
    while(newString[i] != ' ') {
        newName[i] = newString[i];
        i++;
    }

    char *newTime = newString + i + 1;

    i = 0;

    //find correct driver, return null if not found
    while((i < (length - 1)) && (strcmp(database[i].name, newName) != 0)) {
        i++;
    }


    if(length != 0) {
        if(strcmp(database[i].name, newName) != 0) {
            printf("Driver not found.\n");
            free(newName);
            free(newString);
            return NULL;
        }
    } else {
        printf("%s", "No drivers have been added.\n");
        free(newName);
        free(newString);
        return NULL;
    }


    // add times
    database[i].hours = database[i].hours + (newTime[0] - '0');
    database[i].minutes = database[i].minutes + ((newTime[2] - '0') * 10) + (newTime[3] - '0');
    database[i].seconds = database[i].seconds + ((newTime[5] - '0') * 10) + (newTime[6] - '0');

    // make sure time is formatted correctly
    while(database[i].seconds > 60) {
        database[i].seconds = database[i].seconds - 60;
        database[i].minutes = database[i].minutes + 1;
    }
    while(database[i].minutes > 60) {
        database[i].minutes = database[i].minutes - 60;
        database[i].hours = database[i].hours + 1;
    }

    free(newName);
    free(newString);

    return database;
}

void printDriver(struct driver printThis) {
    printf("%s ", printThis.name);
    printf("%s ", printThis.team);
    printf("%d ", printThis.hours);
    printf("%d ", printThis.minutes);
    printf("%d", printThis.seconds);
    printf("%c", '\n');
}

int compareDrivers(const void *first, const void *second) {

    const struct driver *driver1 = first;
    const struct driver *driver2 = second;

    //compare total seconds
    int totalSeconds1 = (driver1->hours * 60 * 60) + (driver1->minutes * 60) + (driver1->seconds);
    int totalSeconds2 = (driver2->hours * 60 * 60) + (driver2->minutes * 60) + (driver2->seconds);

    //if driver1 faster, return negative, if driver2 faster, return positive
    return (totalSeconds1 - totalSeconds2);

}

void sortDatabase(struct driver *database, int length) {
    qsort(database, length, sizeof(struct driver), compareDrivers);
}

void printDrivers(struct driver *database, int length) {

    sortDatabase(database, length);
    
    for(int i = 0; i < length; i++) {
        printDriver(database[i]);
    }
}

struct driver *saveToFile(struct driver *database, int length, char *input) {

    char *filename = input + 2;
    FILE *file = fopen(filename, "w");
    if (!file) {
        return NULL;
    }
    for(int i = 0; i < length; i++) {
        fprintf(file, "%s %s %d %d %d\n", database[i].name, database[i].team, database[i].hours, database[i].minutes, database[i].seconds);
    }
    fclose(file);
    return database;
}

void freeDatabase(struct driver *database, int length) {
    for(int i = 0; i < length; i++) {
        free(database[i].name);
        free(database[i].team);
    }
    free(database);
}

int readFromFile(struct driver **database, char* input) {

    struct driver *newDatabase = *database;
    char *filename = input + 2;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("%s", "Opening file failed.");
        return -1;
    }

    char *newName = calloc(20, sizeof(char));
    char *newTeam = calloc(20, sizeof(char));
    int *hours = calloc(1, sizeof(int));
    int *minutes = calloc(1, sizeof(int));
    int *seconds = calloc(1, sizeof(int));

    int i = 0;
    
    while(fscanf(file, "%s %s %d %d %d", newName, newTeam, hours, minutes, seconds) == 5) {
        newDatabase = realloc(newDatabase, (i + 1)*sizeof(struct driver));
        if(newDatabase == NULL) {
            free(newName);
            free(newTeam);
            free(hours);
            free(minutes);
            free(seconds);
            free(newTeam);
            return -1;
        }
        struct driver *newDriver = calloc(1, sizeof(struct driver));
        if(newDriver == NULL) {
            free(newName);
            free(newTeam);
            free(hours);
            free(minutes);
            free(seconds);
            free(newTeam);
            return -1;
        }
        char *driverName = calloc(20, sizeof(char));
        if(driverName == NULL) {
            free(newName);
            free(newTeam);
            free(hours);
            free(minutes);
            free(seconds);
            free(newTeam);
            free(newDriver);
            return -1;
        }
        char *driverTeam = calloc(20, sizeof(char));
        if(driverName == NULL) {
            free(newName);
            free(newTeam);
            free(hours);
            free(minutes);
            free(seconds);
            free(newTeam);
            free(newDriver);
            free(driverName);
            return -1;
        }

        strcpy(driverName, newName);
        strcpy(driverTeam, newTeam);
        newDriver->name = driverName;
        newDriver->team = driverTeam;
        newDriver->hours = *hours;
        newDriver->minutes = *minutes;
        newDriver->seconds = *seconds;

        newDatabase[i] = *newDriver;
        free(newDriver); 
        i++;
    }
    free(newName);
    free(newTeam);
    free(hours);
    free(minutes);
    free(seconds);
    fclose(file);
    *database = newDatabase;
    return i;
}