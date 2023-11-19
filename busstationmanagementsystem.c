#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_BUS_SCHEDULES
#define MAX_CAPACITY 50
#define MAX_ROUTE_LENGTH 100
#define MAX_DESTINATION_LENGTH 100
#define MAX_TICKETPRICE_LENGTH 100
#define MAX_CAPACITY_LENGTH 100
#define MAX_BUS_SCHEDULES 10
#define MAX_PASSENGER_NAME_LENGTH 100
#define MAX_SCHEDULES 100
#define MAX_SEATS 57
#define MAX_HISTORY_ENTRIES 100
#define MAX_HISTORY_ENTRIES 100
#define BUS_SCHEDULE_FILE "bus_schedules.dat"
#define TICKET_HISTORY_FILE "ticket_history.dat"
#define CANCELLATION_HISTORY_FILE "cancellation_history.dat"




typedef struct {
    char passengerName[MAX_PASSENGER_NAME_LENGTH];
    int busScheduleIndex;
    int numSeats;
    float totalTicketPrice;
} TicketHistory;

typedef struct {
    char passengerName[MAX_PASSENGER_NAME_LENGTH];
    int busScheduleIndex;
    int numSeats;
} CancellationHistory;

TicketHistory ticketHistory[MAX_HISTORY_ENTRIES];
CancellationHistory cancellationHistory[MAX_HISTORY_ENTRIES];
int numTicketHistoryEntries = 0;
int numCancellationHistoryEntries = 0;

typedef struct {
    char route[100];
    char destination[100];
    float ticketPrice;
    char depTime[100];
    int availabeSeats;
    int capacity;
    char seat;
    int scheduleId;
    char availableSeats[MAX_SEATS];  // Array to track seat availability (X for booked)
} BusSchedule;

BusSchedule schedules[MAX_BUS_SCHEDULES];
int numSchedules = 0; // Declaration for numSchedules

typedef struct {
    char name[50];
    int busScheduleIndex;
    int numSeats;
    float totalTicketPrice;
    int isTicketIssued;
    int availableseats;
} Passenger;

Passenger passenger;


// Initialize available seats for each bus schedule
void initializeSchedules() {
    for (int i = 0; i < MAX_BUS_SCHEDULES; i++) {
        for (int j = 0; j < MAX_SEATS; j++) {
            schedules[i].availableSeats[j] = 'O';
        }
    }
}




void updateAvailableSeats(int scheduleIndex, int numBookedSeats) {
    if (scheduleIndex >= 1 && scheduleIndex <= MAX_BUS_SCHEDULES) {
        schedules[scheduleIndex - 1].availabeSeats -= numBookedSeats;
        printf("Updated available seats for schedule %d: %d\n", scheduleIndex, schedules[scheduleIndex - 1].availabeSeats);
    }
}

void addBusSchedule() {
    if (numSchedules >= MAX_BUS_SCHEDULES) {
        printf("Maximum limit for bus schedules reached.\n");
        return;
    }

    printf("\t\t\t\t\t\t\t\tEnter the bus route: ");
    scanf(" %[^\n]s", schedules[numSchedules].route);

    printf("\t\t\t\t\t\t\t\tEnter the capacity of the bus: ");
    scanf("%d", &schedules[numSchedules].capacity);

    printf("\t\t\t\t\t\t\t\tEnter the ticket price: ");
    scanf("%f", &schedules[numSchedules].ticketPrice);

    printf("\t\t\t\t\t\t\t\tEnter the destination of the bus: ");
    scanf("%s", schedules[numSchedules].destination);

    printf("\t\t\t\t\t\t\t\tEnter the departure time of the bus: ");
    scanf("%s", schedules[numSchedules].depTime);

    printf("\t\t\t\t\t\t\t\tBus schedule added successfully!\n");
    numSchedules++;
}


void displayBusSchedules() {
    printf("\t\t\t\t\t\t\t\t================================================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t********BUS SCHEDULES********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s%-20s%-20s%-20s\n", "|Schedule ID", "Route", "Destination", "Capacity","Dept Time", "Ticket Price|");
    printf("\t\t\t\t\t\t\t\t=================================================================================================================\n");
    for (int i = 0; i < numSchedules; i++) {
        printf("\t\t\t\t\t\t\t\t|%-20d%-20s%-20s%-20d%-20s%-20.2f\n", i + 1, schedules[i].route, schedules[i].destination, schedules[i].capacity,schedules[i].depTime, schedules[i].ticketPrice);
        printf("\t\t\t\t\t\t\t\t================================================================================================================\n");
    }
}

void deleteBusSchedule(int scheduleIndex) {
    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index. Please choose a valid schedule.\n");
        return;
    }

    // Move all schedules after the deleted schedule to fill the gap
    for (int i = scheduleIndex - 1; i < numSchedules - 1; i++) {
        schedules[i] = schedules[i + 1];
    }

    numSchedules--;

    printf("Bus schedule at index %d deleted successfully.\n", scheduleIndex);
}

void markSeatAsBooked(BusSchedule *schedule, int seatNumber) {
    if (schedule && seatNumber >= 1 && seatNumber <= 57) {
        // Calculate the index of the selected seat in the availableSeats array
        int seatIndex = seatNumber - 1;
        schedule->availableSeats[seatIndex] = 'X';  // Mark the seat as booked
    }
}

char getSeatStatus(BusSchedule *schedule, int seatNumber) {
    if (schedule && seatNumber >= 1 && seatNumber <= 57) {
        // Calculate the index of the selected seat in the availableSeats array
        int seatIndex = seatNumber - 1;
        return schedule->availableSeats[seatIndex];
    }
    return ' '; // Return a space if the seat is out of range or the schedule is invalid
}

int scheduleIndex;
char selectedSeatType;
// Function to book a seat for a specific bus schedule

void bookTicketForSchedule(int scheduleIndex) {
    if (passenger.isTicketIssued) {
        printf("You have already booked a ticket.\n");
        return;
    }

    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index.\n");
        return;
    }

    BusSchedule *schedule = &schedules[scheduleIndex - 1];

    // Display available seats for the selected bus schedule
    viewAvailableSeatsForSchedule(schedule);

    int selectedSeat;
    char selectedSeatType;

    printf("Enter the seat you want to book (e.g., 1 for A1, 12 for W12): ");
    scanf("%d", &selectedSeat);

    if (selectedSeat < 1 || selectedSeat > MAX_SEATS) {
        printf("Invalid seat number. Please choose a valid seat.\n");
        return;
    }

    selectedSeat--;  // Adjust to 0-based index

    if (selectedSeat < 0 || selectedSeat >= MAX_SEATS) {
        printf("Invalid seat number. Please choose a valid seat.\n");
        return;
    }

    if (schedule->availableSeats[selectedSeat] == 'X') {
        printf("Selected seat is already booked. Please choose another seat.\n");
    } else {
        // Update the total ticket price and issue the ticket
        passenger.numSeats = selectedSeat + 1;
        passenger.totalTicketPrice = schedule->ticketPrice;
        printf("Ticket booked successfully for %s on schedule %d for seat %c%d.\n",
               passenger.name, scheduleIndex, selectedSeatType, selectedSeat + 1);
        printf("Total Ticket Price: %.2f\n", passenger.totalTicketPrice);
        passenger.isTicketIssued = 1;

        // Update the availableSeats array to mark the seat as booked
        schedule->availableSeats[selectedSeat] = 'X';
    }
}
// Declaration of the function
void updateAvailableSeats(int scheduleIndex, int numBookedSeats);

// ...

void viewAvailableSeatsForSchedule(BusSchedule *schedule) {
    if (schedule == NULL) {
        printf("Invalid schedule.\n");
        return;
    }

    printf("Available Seats for Schedule %d (%s to %s):\n", schedule->scheduleId, schedule->route, schedule->destination);
    printf("Legend: [W] Window Seat, [A] Aisle Seat, [X] Booked Seat\n");

    for (int row = 0; row < MAX_SEATS / 4; row++) {
        for (int col = 0; col < 4; col++) {
            int seatNumber = 4 * row + col;
            char seatType;

            if (col == 0 || col == 3) {
                seatType = 'W';  // Window seats
            } else {
                seatType = 'A';  // Aisle seats
            }

            char status = schedule->availableSeats[seatNumber];

            if (status == 'X') {
                printf("[X] ");  // Booked seat
            } else {
                printf("[%c%d] ", seatType, seatNumber + 1);
            }
        }
        printf("\n");
    }
}

char selectedSeatType;

void issueTicketForSchedule(int scheduleIndex) {
    if (!passenger.isTicketIssued) {
        printf("You have not booked a ticket yet.\n");
        return;
    }

    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index.\n");
        return;
    }

    BusSchedule *schedule = &schedules[scheduleIndex - 1];

    // Display available seats for the selected bus schedule
    viewAvailableSeatsForSchedule(schedule);

    // Check if a ticket is issued
    if (passenger.isTicketIssued) {
        printf("Receipt for Ticket Issued:\n");
        printf("Passenger Name: %s\n", passenger.name);
        printf("Bus Schedule Index: %d\n", scheduleIndex);
        printf("Seat: %c%d\n", selectedSeatType, passenger.numSeats);
        printf("Departure Time: %s\n", schedule->depTime);
        printf("Ticket Price: %.2f\n", schedule->ticketPrice);
        printf("Route: %s\n", schedule->route);
        printf("Destination: %s\n", schedule->destination);
    } else {
        printf("You have not booked a ticket yet.\n");
    }
}



void bookTicket() {
    if (passenger.isTicketIssued) {
        printf("You have already booked a ticket.\n");
        return;
    }

    printf("Enter your name: ");
    scanf(" %[^\n]s", passenger.name);

    printf("Enter the bus schedule index you want to book: ");
    scanf("%d", &passenger.busScheduleIndex);

    int scheduleIndex = passenger.busScheduleIndex;

    // Check if the selected schedule exists
    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index.\n");
        return;
    }

    // Book a ticket within the selected bus schedule
    bookTicketForSchedule(scheduleIndex);
}


void issueTicket() {
    if (!passenger.isTicketIssued) {
        printf("You have not booked a ticket yet.\n");
        return;
    }

    int scheduleIndex = passenger.busScheduleIndex;
    int seatNumber = passenger.numSeats;

    generateReceipt(scheduleIndex, seatNumber);

    // Add issued ticket to the history
    strcpy(ticketHistory[numTicketHistoryEntries].passengerName, passenger.name);
    ticketHistory[numTicketHistoryEntries].busScheduleIndex = scheduleIndex;
    ticketHistory[numTicketHistoryEntries].numSeats = seatNumber;
    ticketHistory[numTicketHistoryEntries].totalTicketPrice = passenger.totalTicketPrice;
    numTicketHistoryEntries++;

    printf("\t\t\t\t\t\t\t\tTicket issued successfully!\n");

    passenger.isTicketIssued = 0;  // Mark the ticket as not issued
}

void generateReceipt(int scheduleIndex, int seatNumber) {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Ticket Receipt********\n");
    printf("\t\t\t\t\t\t\t\tBus Schedule Index: %d\n", scheduleIndex);
    printf("\t\t\t\t\t\t\t\tPassenger Name: %s\n", passenger.name);
    printf("\t\t\t\t\t\t\t\tDeparture Time: %s\n", schedules[scheduleIndex - 1].depTime);
    printf("\t\t\t\t\t\t\t\tRoute: %s\n", schedules[scheduleIndex - 1].route);
    printf("\t\t\t\t\t\t\t\tDestination: %s\n", schedules[scheduleIndex - 1].destination);
    printf("\t\t\t\t\t\t\t\tSeat Number: %d\n", seatNumber);
    printf("\t\t\t\t\t\t\t\tTotal Ticket Price: %.2f\n", passenger.totalTicketPrice);
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
}




// ... (Previous code)

void displayBookingHistory() {
    if (passenger.isTicketIssued) {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t********Booking History********\n");
        printf("\t\t\t\t\t\t\t\tPassenger: %s\n", passenger.name);
        printf("\t\t\t\t\t\t\t\tBus Schedule Index: %d\n", passenger.busScheduleIndex);
        printf("\t\t\t\t\t\t\t\tNumber of Seats: %d\n", passenger.numSeats);
        printf("\t\t\t\t\t\t\t\tTotal Ticket Price: %.2f\n", passenger.totalTicketPrice);
        printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    } else {
        printf("\t\t\t\t\t\t\t\tNo booking history found.\n");
    }
}

void viewBookedSeatsForSchedule(int scheduleIndex) {
    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index. Please choose a valid schedule.\n");
        return;
    }

    BusSchedule *schedule = &schedules[scheduleIndex - 1];

    printf("Booked Seats for Schedule %d (%s to %s):\n", schedule->scheduleId, schedule->route, schedule->destination);
    printf("Legend: [W] Window Seat, [A] Aisle Seat, [X] Booked Seat\n");

    for (int row = 0; row < MAX_SEATS / 4; row++) {
        for (int col = 0; col < 4; col++) {
            int seatNumber = 4 * row + col;
            char seatType;

            if (col == 0 || col == 3) {
                seatType = 'W';  // Window seats
            } else {
                seatType = 'A';  // Aisle seats
            }

            char status = schedule->availableSeats[seatNumber];

            if (status == 'X') {
                printf("[X] ");  // Booked seat
            } else {
                printf("[%c%d] ", seatType, seatNumber + 1);  // Available seat with seat number
            }
        }
        printf("\n");
    }
}

void displayTicketHistory() {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Ticket Issuing History********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s%-20s\n", "Passenger", "Schedule ID", "seat number", "Total Ticket Price");
    for (int i = 0; i < numTicketHistoryEntries; i++) {
        printf("\t\t\t\t\t\t\t\t%-20s%-20d%-20d%-20.2f\n", ticketHistory[i].passengerName, ticketHistory[i].busScheduleIndex,
               ticketHistory[i].numSeats, ticketHistory[i].totalTicketPrice);
    }
    printf("\t\t\t\t\t\t\t\t=============================================================================================\n");
}


char selectedSeatType;



// Function to cancel a booking for a specific bus schedule
void cancelBookingForSchedule(int scheduleIndex) {
    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index. Please choose a valid schedule.\n");
        return;
    }

    BusSchedule *schedule = &schedules[scheduleIndex - 1];

    if (!passenger.isTicketIssued) {
        printf("You have not booked a ticket yet.\n");
        return;
    }

    viewAvailableSeatsForSchedule(schedule);

    int selectedSeat;
    printf("Enter the seat number you want to cancel (e.g., 1 for A1, 12 for W12): ");
    scanf("%d", &selectedSeat);

    if (selectedSeat < 1 || selectedSeat > schedule->capacity) {
        printf("Invalid seat number. Please choose a valid seat.\n");
        return;
    }

    // Adjust the selected seat to a 0-based index
    selectedSeat--;

    if (schedule->availableSeats[selectedSeat] == 0) {
        printf("The selected seat is not booked. Please choose a booked seat to cancel.\n");
    } else {
        schedule->availableSeats[selectedSeat] = 0;  // Mark the seat as available
        printf("Booking for seat %d has been canceled.\n", selectedSeat + 1);
        passenger.isTicketIssued = 0;  // Mark the ticket as not issued
    }
}

void cancelBooking() {
    if (passenger.isTicketIssued) {
        int scheduleIndex = passenger.busScheduleIndex;

        printf("Enter the seat number you want to cancel (e.g., 1 or 14): ");
        int selectedSeat;
        scanf("%d", &selectedSeat);

        if (selectedSeat < 1 || selectedSeat > 57) {
            printf("Invalid seat number. Please choose a valid seat number between 1 and 57.\n");
            return;
        }

        // Check if the selected seat is already booked
        if (schedules[scheduleIndex - 1].availableSeats[selectedSeat - 1] == 'X') {
            // Add the canceled ticket to the cancellation history
            strcpy(cancellationHistory[numCancellationHistoryEntries].passengerName, passenger.name);
            cancellationHistory[numCancellationHistoryEntries].busScheduleIndex = scheduleIndex;
            cancellationHistory[numCancellationHistoryEntries].numSeats = selectedSeat;
            numCancellationHistoryEntries++;

            // Mark the seat as available
            schedules[scheduleIndex - 1].availableSeats[selectedSeat - 1] = 'A';

            // Remove the canceled seat from ticket history
            for (int i = 0; i < numTicketHistoryEntries; i++) {
                if (strcmp(ticketHistory[i].passengerName, passenger.name) == 0 &&
                    ticketHistory[i].busScheduleIndex == scheduleIndex &&
                    ticketHistory[i].numSeats == selectedSeat) {
                    // Mark the ticket as canceled in ticket history
                    ticketHistory[i].busScheduleIndex = -1;
                    break;
                }
            }

            printf("Booking for seat %d on schedule %d is canceled.\n", selectedSeat, scheduleIndex);
            updateAvailableSeats(scheduleIndex, -1);
            passenger.isTicketIssued = 0;  // Mark the ticket as not issued
        } else {
            printf("The selected seat is not booked. No action taken.\n");
        }
    } else {
        printf("You have not booked a ticket yet. Please book a ticket before canceling.\n");
    }
}



void displayCancellationHistory() {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Booking Cancellation History********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s\n", "Passenger", "Schedule ID", "seat no:");
    for (int i = 0; i < numCancellationHistoryEntries; i++) {
        printf("\t\t\t\t\t\t\t\t%-20s%-20d%-20d\n", cancellationHistory[i].passengerName, cancellationHistory[i].busScheduleIndex,
               cancellationHistory[i].numSeats);
    }
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
}






void saveBusSchedules() {
    FILE *file = fopen("bus_schedules.dat", "wb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tCould not save bus schedules.\n");
        return;
    }

    fwrite(schedules, sizeof(BusSchedule), numSchedules, file);
    fclose(file);
}

void loadBusSchedules() {
    FILE *file = fopen("bus_schedules.dat", "rb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tNo saved bus schedules found.\n");
        return;
    }

    numSchedules = fread(schedules, sizeof(BusSchedule), MAX_SCHEDULES, file);
    fclose(file);
}

void saveTicketHistory() {
    FILE *file = fopen("ticket_history.dat", "wb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tCould not save ticket history.\n");
        return;
    }

    fwrite(ticketHistory, sizeof(TicketHistory), numTicketHistoryEntries, file);
    fclose(file);
}

void loadTicketHistory() {
    FILE *file = fopen("ticket_history.dat", "rb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tNo saved ticket history found.\n");
        return;
    }

    numTicketHistoryEntries = fread(ticketHistory, sizeof(TicketHistory), MAX_HISTORY_ENTRIES, file);
    fclose(file);
}

void saveBusSchedulesToFile() {
    FILE *file = fopen(BUS_SCHEDULE_FILE, "wb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tCould not save bus schedules to file.\n");
        return;
    }

    fwrite(schedules, sizeof(BusSchedule), numSchedules, file);
    fclose(file);
}

void loadBusSchedulesFromFile() {
    FILE *file = fopen(BUS_SCHEDULE_FILE, "rb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tNo saved bus schedules found.\n");
        return;
    }

    numSchedules = fread(schedules, sizeof(BusSchedule), MAX_BUS_SCHEDULES, file);
    fclose(file);
}

void saveTicketHistoryToFile() {
    FILE *file = fopen(TICKET_HISTORY_FILE, "wb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tCould not save ticket history to file.\n");
        return;
    }

    fwrite(ticketHistory, sizeof(TicketHistory), numTicketHistoryEntries, file);
    fclose(file);
}

void loadTicketHistoryFromFile() {
    FILE *file = fopen(TICKET_HISTORY_FILE, "rb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tNo saved ticket history found.\n");
        return;
    }

    numTicketHistoryEntries = fread(ticketHistory, sizeof(TicketHistory), MAX_HISTORY_ENTRIES, file);
    fclose(file);
}

void saveCancellationHistoryToFile() {
    FILE *file = fopen(CANCELLATION_HISTORY_FILE, "wb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tCould not save cancellation history to file.\n");
        return;
    }

    fwrite(cancellationHistory, sizeof(CancellationHistory), numCancellationHistoryEntries, file);
    fclose(file);
}

void loadCancellationHistoryFromFile() {
    FILE *file = fopen(CANCELLATION_HISTORY_FILE, "rb");
    if (file == NULL) {
        printf("\t\t\t\t\t\t\t\tNo saved cancellation history found.\n");
        return;
    }

    numCancellationHistoryEntries = fread(cancellationHistory, sizeof(CancellationHistory), MAX_HISTORY_ENTRIES, file);
    fclose(file);
}



struct user{
    char username[50];
    char password[50];
    char phone[50];
};

int main() {
    struct user usr;
    FILE *fp;
    char filename[50], phone[50], pword[50];
    int choice;
    char cont = 'y';

    loadBusSchedulesFromFile();
    loadTicketHistoryFromFile();
    loadCancellationHistoryFromFile();


printf("\n\t\t\t\t\t\t\t\t==================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\tWELCOME TO ULTIMATE BUS STATION BOOKING SERVICES\n");
printf("\t\t\t\t\t\t\t\t==================================================================================\n");
    do {
        printf("\t\t\t\t\t\t\t\t\t\t\t=========Hello user!=========\n");
        printf("\t\t\t\t\t\t\t\tWhich of the options do you want to transact:\n");
        printf("\t\t\t\t\t\t\t\t1. Sign up\n");
        printf("\t\t\t\t\t\t\t\t2. Sign in\n");
        printf("\t\t\t\t\t\t\t\t0. Exit\n");
        printf("\n\t\t\t\t\t\t\t\t==================================================================================\n");

        printf("\t\t\t\t\t\t\t\t\tYour choice: ");
        scanf("%d", &choice);


        switch (choice) {
            case 1:
                // Sign up logic
                // ...
     system("cls");
        printf("\t\t\t\t\t\t\t\tEnter your name:");
        scanf("%s",usr.username);
        printf("\n\n\t\t\t\t\t\t\t\tEnter your phone number:");
        scanf("%s",usr.phone);
        printf("\n\n\t\t\t\t\t\t\t\tEnter your new password:");
        scanf("%s",usr.password);
        strcpy(filename,usr.phone);
        fp = fopen(strcat(filename,".dat"),"w");
        fwrite(&usr,sizeof(struct user),1,fp);
        if(fwrite >0)
            {
            printf("\n\n\t\t\t\t\t\t\t\tAccount succesfully registered");
}
          else{
                printf("\n\n\t\t\t\t\t\t\t\tsomething went wrong please try again");
            }

            fclose (fp);
                break;
            case 2:
                // Sign in logic
                // ...
system("cls");
    printf("\n\t\t\t\t\t\t\t\tPhone number:\t");
    scanf("%s",phone);
   printf("\n\t\t\t\t\t\t\t\tPassword:\t");
char c;
int i = 0;

// Read characters until Enter is pressed
while (1) {
    c = _getch(); // Use _getch() to read a character without echoing to the console

    if (c == '\r') { // Check if Enter is pressed
        pword[i] = '\0'; // Null-terminate the string
        break;
    } else if (c == '\b' && i > 0) { // Check if Backspace is pressed
        printf("\b \b"); // Move the cursor back, print a space, move the cursor back again
        i--;
    } else if (isprint(c)) { // Check if the character is printable
        pword[i++] = c; // Add the character to the password and move to the next position
        printf("*"); // Print '*' instead of the actual character
    }
}

    strcpy(filename,phone);
    fp = fopen(strcat(filename,".dat"),"r");
    if(fp == NULL){
            printf("\n\t\t\t\t\t\t\t\tUser not registered");
    }
    fread(&usr,sizeof(struct user),1,fp);
    fclose(fp);
    if(!strcmp(pword,usr.password)){
    }
passenger.isTicketIssued = 0;

                if (!strcmp(pword, usr.password)) {
                    while (cont == 'y' || cont == 'Y') {
                        system("cls");
                        printf("\t\t\t\t\t\t\t\t==== Bus Station Management ====\n");
                        printf("\t\t\t\t\t\t\t\t1. Add Bus Schedule\n");
                        printf("\t\t\t\t\t\t\t\t2. View Bus Schedules\n");
                        printf("\t\t\t\t\t\t\t\t3. Book a Ticket\n");
                        printf("\t\t\t\t\t\t\t\t4. Issue Ticket\n");
                        printf("\t\t\t\t\t\t\t\t5. Cancel Booking\n");
                        printf("\t\t\t\t\t\t\t\t6. Display Booking History\n");
                        printf("\t\t\t\t\t\t\t\t7. Display ticket issuing history\n");
                        printf("\t\t\t\t\t\t\t\t8. Display all cancelled tickets\n");
                        printf("\t\t\t\t\t\t\t\t9. Delete a bus schedule detail\n");
                        printf("\t\t\t\t\t\t\t\t10. Display Booked seats\n");


                        printf("\t\t\t\t\t\t\t\t0. Exit\n");
                        printf("\t\t\t\t\t\t\t\t\t\tEnter your choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1:
                                addBusSchedule();
                                break;
                            case 2:
                                displayBusSchedules();
                                break;
                            case 3:
                                bookTicket();
                                break;
                            case 4:
                                issueTicket();
                                break;
                            case 5:
                                cancelBooking();
                                break;
                            case 6:
                                displayBookingHistory();
                                break;
                            case 7:
                                // Display ticket issuing history
                                // ...
                                displayTicketHistory();

                                break;
                            case 8:
                                displayCancellationHistory();
                                break;
                            case 9:
    // Delete Bus Schedule
    printf("Enter the schedule index you want to delete: ");
    scanf("%d", &scheduleIndex);
    deleteBusSchedule(scheduleIndex);
    break;

case 10: // Assuming 10 is the option to view booked seats
    printf("Enter the bus schedule index to view booked seats: ");
    scanf("%d", &scheduleIndex);
    viewBookedSeatsForSchedule(scheduleIndex);
    break;


                            case 0:
                                printf("\t\t\t\t\t\t\t\tGoodbye!\n");
                                return 0;
                            default:
                                printf("\t\t\t\t\t\t\t\tInvalid choice. Please try again.\n");
                        }

                        printf("\n\n\t\t\tDo you want to continue with another operation? (y/n): ");
                        scanf(" %c", &cont);
                    }
                }
                break;
            case 0:
                printf("\t\t\t\t\t\t\t\tGoodbye!\n");
                return 0;
            default:
                printf("\t\t\t\t\t\t\t\tInvalid choice. Please try again.\n");
        }

        printf("\n\n\t\t\tDo you want to perform another operation? (y/n): ");
        scanf(" %c", &cont);
    } while (cont == 'y' || cont == 'Y');

    atexit(saveBusSchedulesToFile);
    atexit(saveTicketHistoryToFile);
    atexit(saveCancellationHistoryToFile);

    return 0;
}


