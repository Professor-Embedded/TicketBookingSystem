#include <stdio.h>
#include "myheader.h"

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



//--------------Driver Code-------------//
int main(){
	//loadBookings();
	int menu = -1;
	printf("\n-----------------------------------\n");
	printf("Welocome to the ticket booking system");
	printf("\n-----------------------------------\n");
	
	while(menu != 4){
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

// Adding comment to check pr format
