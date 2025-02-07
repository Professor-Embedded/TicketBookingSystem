COMPILER=gcc

build:
	$(COMPILER) main.c -o main.out
clean:
	rm -rf main.out
	rm -rf *.o
	rm -rf booking-record.dat
	rm -rf cancellation-record.dat
	rm -rf bookings.txt
	rm -rf ticket_ids.txt
