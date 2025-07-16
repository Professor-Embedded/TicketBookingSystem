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


//---------------Structure initialization-------------//
Movie movies[NO_OF_SCREENS] = { 
				{1, "Badass Ravi Kumar"},
				{2, "Aegon The conqueror"}
			     };

Screen screens[NO_OF_SCREENS] = {
					{1, 1, 280, {"14:00", "19:00","23:00"}, {{{0}}}},
					{2, 2, 500, {"15:00","20:00","23:30"}, {{{0}}}}
				};

Tickets tickets[MAX_TICKETS_IDS];

//--------Function declaration----------//
void BookTicket();
void CancelTicket();
void Record();
void PrintTicket(int,int,int,int,int);
void PrintAvailableSeats(int,int);
int generateTicketID(void);


//---------Function Defination-----------//
int generateTicketID(void){
	FILE *file = fopen("ticket_ids.txt", "a+");
	
	if(file == NULL){
		printf("Opening file attempt fail!\n");
		return 1;
	}
	
	int id, found;
	srand(time(NULL));

	do{
	   	found = 0;
		id = rand()%900000 + 100000;
		rewind(file);

		int existingID;
		while(fscanf(file, "%d", &existingID) != EOF){
			
			if(existingID == id){
				found = 1;
				break;
			}
		}

	
	}while(found);
	
	fprintf(file, "%d\n", id);
	fclose(file);
	return id;


}
void PrintAvailableSeats(int sel_movie, int sel_time){
	for(int i=0; i<ROWS; i++){
		printf("\n");
		for(int j=0; j<COLS; j++){
			if(screens[sel_movie].seats[sel_time][i][j] == 0){
				printf("%c%d ",rowchar[i],j+1 );
			}

			else{
				printf("  ");
			}
		}
		
	}
}


void commitBookings() {
    FILE *file = fopen("bookings.txt", "w+");  
    if (file == NULL) {
        printf("File opening attempt failed\n");
        return;
    }
    fwrite(screens, sizeof(screens), 1, file);  
    fclose(file);
	//printf("Hi i got opened");
}

void AddBookingRecord(int ticketid, int screenInd, int showInd, int row, int col, char date[10], int price ){
	FILE* file = fopen("booking-record.dat","a+");
	fprintf(file,"Ticket No:%d, ", ticketid);
	fprintf(file,"Screen:%d, ",screens[screenInd].id);
	fprintf(file,"%s, ",movies[screens[screenInd].movieId -1].name);
	
	char rw = rowchar[row];

	fprintf(file,"Seat:%c%d, ",rw,col+1);
	fprintf(file, "Date:%s, ",date);
	fprintf(file,"Time:%s, ",screens[screenInd].showTimes[showInd]);
	fprintf(file, "Price:%d, ",price);
	fprintf(file,"\n\n\n");
	fclose(file);
}
void AddCancellationRecord(int ticketid, int screenInd, int showInd, int row, int col,char date[11], int price){
	FILE* file = fopen("cancellation-record.dat","a+");
	fprintf(file,"Ticket No:%d, ", ticketid);
	fprintf(file,"Screen:%d, ",screens[screenInd].id);
	fprintf(file,"%s, ",movies[screens[screenInd].movieId -1].name);
	
	char rw = rowchar[row];

	fprintf(file,"Seat:%c%d, ",rw,col+1);
	fprintf(file,"Date:%s, ",date);
	fprintf(file,"Time:%s, ",screens[screenInd].showTimes[showInd]);
	fprintf(file, "Price:%d, ",price);
	fprintf(file,"\n\n\n");
	fclose(file);
}


void BookTicket(){
	char date[11];
	printf("Enter date (dd/mm/yyyy):");
	scanf("%s",date);

	int selected_movie=-1;
	printf("\nSelect Movie:\n0:%s\n1:%s",movies[screens[0].movieId-1].name, movies[screens[1].movieId -1].name);
	scanf("%d",&selected_movie);

	if(selected_movie != 0 && selected_movie !=1){
		printf("Wrong input value\n");
		printf("Redirecting to the main menu...\n");
		return;
	}

	int selected_time=-1;
	printf("\nSelect show time:\n0:%s\n1:%s\n2:%s",screens[selected_movie].showTimes[0],
						       screens[selected_movie].showTimes[1],
						       screens[selected_movie].showTimes[2]);
	scanf("%d",&selected_time);
	
	if(selected_time != 0 && selected_time !=1 &&  selected_time != 2){
		printf("Wrong input value\n");
		printf("Redirecting to the main menu...\n");
		return;
	}
	
	
 	PrintAvailableSeats(selected_movie,selected_time);
	printf("\n");
	int noofseats = 1;
	int valid = 1;

	while(valid){
		printf("Enter number of seats you want to book:");
		scanf("%d",&noofseats);
		if(noofseats > 0 && noofseats<10)
			valid = 0;
		else
		printf("No of seats must be between 1 to 9..\n");
	}

	printf("Total Amount: %d\n", noofseats*screens[selected_movie].price);
	printf("Press 1 to Continue else 0 to Exit: ");
	int confirmFlag = -1;
	scanf("%d",&confirmFlag);

	if(confirmFlag != 1){
		printf("Redirecting to main menu....\n");
		return;
	}

	while(noofseats){
		printf("\n");
 		PrintAvailableSeats(selected_movie,selected_time);
		printf("Select your seats:");
		char selected_seat[3];
		scanf("%s",selected_seat);
	
		if(!( (selected_seat[0] >= 'A' && selected_seat[0] <= 'J') &&
			(selected_seat[1] >= '1' && selected_seat[1] <= '5') )){
			printf("Not a valid input\nDo a proper selection...\n");
			continue;
		}
		
		int sel_row = selected_seat[0]-'A';
		int sel_col = selected_seat[1]- '1';
		printf("\n%d,%d\n",sel_row,sel_col);
		if(screens[selected_movie].seats[selected_time][sel_row][sel_col] == 0){
			screens[selected_movie].seats[selected_time][sel_row][sel_col] = 1;
			commitBookings();
			printf("Bookings committed\n");
			int ticketid = generateTicketID();
			printf("Ticket Id generated\n");
			AddBookingRecord(ticketid, selected_movie, selected_time, sel_row, sel_col,date,screens[selected_movie].price);
			printf("Booking Record Added\n");
			Tickets temp;
			temp.ticketid = ticketid;
			temp.screenInd = selected_movie;
			temp.showInd = selected_time;
			strncpy(temp.date,date,11);
			temp.row = sel_row;
			temp.col = sel_col;
			temp.price = screens[selected_movie].price;
			tickets[ticketid] = temp;
			printf("tickets Array updated\n");
			PrintTicket(ticketid, selected_movie, selected_time, sel_row, sel_col);
			noofseats--;
			printf("\n");
		}

		else{
			printf("Try another!!\n");
		}

	}

}

void CancelTicket(){
	int ticketid,confirm;
	printf("Enter you ticket no:");
	scanf("%d", &ticketid);
	printf("Your Ticket No: %d\n", ticketid);

	FILE *file = fopen("ticket_ids.txt","a+");
	rewind(file);
	int id;
	int count = 0;
	int found = 0;
	while(fscanf(file, "%d", &id) != EOF) {
		if(id == ticketid){
			found = 1;
			printf("Ticket found!\n");
		}
		count++;

	}

	rewind(file);
	int ticketids[count];
	int ind = 0;

	while(fscanf(file, "%d", &id) != EOF){
		if(id != ticketid){
			ticketids[ind++] = id;
		}
	}
		

	fclose(file);
	


	printf("Press 1 to confirm your cancellation else press 0: ");
	scanf("%d",&confirm);

	if(confirm == 1 && found == 1){	
			Tickets temp = tickets[ticketid];
	FILE *file1 = fopen("ticket_ids.txt","w+");
			screens[temp.screenInd].seats[temp.showInd][temp.row][temp.col] = 0;
			commitBookings();

			for(int i=0; i<count; i++){
				fprintf(file1, "%d\n",ticketids[i]);
			}
			//fwrite(&ticketids, sizeof(int), 1, file1);
			fclose(file1);
			
			AddCancellationRecord(ticketid, temp.screenInd, temp.showInd, temp.row, temp.col,temp.date,temp.price );

			printf("Ticket Canceled Successfully\n");
		}
		

	else if(found == 0)
		printf("Ticket Not found!\n");
	

	else
		printf("Exit without cancellation...\n");
	

}

void Record(){
	printf("\n---------------: BOOKING DATA :----------------\n");
	FILE* file =  fopen("booking-record.dat", "r");
	char ch;
	
	if(file == NULL){
		printf("Booking Record not found!\n");
		return;
	}
	while( (ch = fgetc(file)) != EOF ){
		putchar(ch);
	}
	
	fclose(file);

	printf("\n---------------: CANCELLATION DATA :----------------\n");
	FILE* file1 =  fopen("cancellation-record.dat", "r");
	if(file1 == NULL){
		printf("cancellation record not found\n");
		return;
	}

	while( (ch = fgetc(file1)) != EOF ){
		putchar(ch);
	}
	
	fclose(file);

	
}

	

void PrintTicket(int ticketid, int screenInd, int showInd, int row, int col ){
	
	printf("\n=========================================\n");
	printf("%s\n",THEATER);
	printf("=========================================\n");
	printf("Ticket No:%d", ticketid);
	printf("							Movie ticket\n");
	printf("=========================================\n");
	printf("Screen:%d",screens[screenInd].id);
	printf("							%s\n",movies[screens[screenInd].movieId -1].name);
	
	char rw = rowchar[row];

	printf("Seat:%c%d\n",rw,col+1);
	printf("Time:%s\n",screens[screenInd].showTimes[showInd]);
	printf("Date:%s\n",tickets[ticketid].date);
	printf("						Price:%d\n",tickets[ticketid].price);
	printf("=========================================\n");	

}


void ShowAvailibility(void){
	
	char date[11];
	printf("Enter date (dd/mm/yyyy):");
	scanf("%s",date);

	int selected_movie=-1;
	printf("\nSelect Movie:\n0:%s\n1:%s",movies[screens[0].movieId-1].name, movies[screens[1].movieId -1].name);
	scanf("%d",&selected_movie);

	if(selected_movie != 0 && selected_movie !=1){
		printf("Wrong input value\n");
		printf("Redirecting to the main menu...\n");
		return;
	}

	int selected_time=-1;
	printf("\nSelect show time:\n0:%s\n1:%s\n2:%s",screens[selected_movie].showTimes[0],
						       screens[selected_movie].showTimes[1],
						       screens[selected_movie].showTimes[2]);
	scanf("%d",&selected_time);
	
	if(selected_time != 0 && selected_time !=1 &&  selected_time != 2){
		printf("Wrong input value\n");
		printf("Redirecting to the main menu...\n");
		return;
	}
	
	
 	PrintAvailableSeats(selected_movie,selected_time);
	printf("\n");

}

/*
void loadBookings(){
	FILE *file = fopen("bookings.txt", "r");
	if(file != NULL){
		fread(screens, sizeof(screens), 1, file);
		fclose(file);
	}
}
*/

//--------------Driver Code-------------//
int main(){
	//loadBookings();
	int menu = -1;
	printf("\n-----------------------------------\n");
	printf("Welocome to the ticket booking system");
	printf("\n-----------------------------------\n");
	
	while(menu != 5){
		printf("\n\nBook ticket : 1\nCancel ticket : 2\nView Record : 3\nShow Availability: 4\nExit : 5\nPlease select appropriate option from the above: ");
		scanf("%d",&menu);
		
		switch(menu){

			case 1: BookTicket();
				break;

			case 2: CancelTicket();
				break;

			case 3: Record();
				break;

			case 4: ShowAvailibility();
				break;

			case 5: printf("Exiting!!\n");
				break;

			default: printf("Invalid input, Try again\n");
		}

	}
	

	return 0;
}
// adding one line commment here
