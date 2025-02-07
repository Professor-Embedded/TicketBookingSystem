#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define NO_OF_SCREENS 2
#define NO_OF_SEATS 50
#define ROWS 10
#define COLS 5
#define MAX_TICKETS_IDS 9999999
#define SHOWS_PER_DAY 3
#define THEATER "AP MULTIPLEX"

char rowchar[10] = {'A', 'B','C','D','E','F','G','H','I','J'};
//-----------Structure declaration--------------//
typedef struct{
        int id;
        char name[50];
}Movie;

typedef struct{
        int id;
        int movieId;
        int price;
        char showTimes[SHOWS_PER_DAY][10];
        int seats[SHOWS_PER_DAY][ROWS][COLS];
}Screen;

typedef struct{
        int ticketid;
        int screenInd;
        int showInd;
        int row;
        int col;
        int price;
        char date[11];
}Tickets;
