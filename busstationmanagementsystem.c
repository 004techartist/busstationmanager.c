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
    char availableSeats[57]; // A: Available, X: Occupied
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


void initializeSchedules() {
    for (int i = 0; i < MAX_BUS_SCHEDULES; i++) {
        for (int j = 0; j < 57; j++) {
            schedules[i].availableSeats[j] = 'A'; // Mark all seats as available
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
// Declaration of the function
void updateAvailableSeats(int scheduleIndex, int numBookedSeats);

// ...



void viewAvailableSeats(int scheduleIndex) {
    if (scheduleIndex < 1 || scheduleIndex > numSchedules) {
        printf("Invalid schedule index.\n");
        return;
    }

    BusSchedule *schedule = &schedules[scheduleIndex - 1];
    printf("Available Seats for Schedule %d (%s to %s):\n", scheduleIndex, schedule->route, schedule->destination);
    printf("Legend: [W] Window Seat, [A] Aisle Seat, [X] Booked Seat\n");

    for (int row = 0; row < 57 / 4; row++) {
        for (int col = 0; col < 4; col++) {
            int seatNumber = 4 * row + col;
            char seatType;

            if (col == 0 || col == 3) {
                seatType = 'W';  // Window seats
            } else {
                seatType = 'A';  // Aisle seats
            }

            int seatIndex = 4 * row + col;
            char status = schedule->availableSeats[seatIndex];

            if (status == 'X') {
                printf("[X] ");  // Booked seat
            } else {
                printf("[%c%d] ", seatType, seatNumber + 1);
            }
        }
        printf("\n");
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

    // Display available seats for the selected bus schedule
    viewAvailableSeats(passenger.busScheduleIndex);

    char selectedSeat[3]; // A string to store the seat input, e.g., "A1" or "W2"
    printf("Enter the seat you want to book (e.g., A1, W2): ");
    scanf("%s", selectedSeat);

    // Convert the seat input to numeric seat number
    int selectedSeatNumber = -1;
    if (selectedSeat[0] == 'A' || selectedSeat[0] == 'W') {
        selectedSeatNumber = atoi(selectedSeat + 1);
    }

    if (selectedSeatNumber < 1 || selectedSeatNumber > 57) {
        printf("Invalid seat number. Please choose a valid seat.\n");
        return;
    }

    // Check if the selected seat is available
    int isSeatAvailable = 1; // Assuming the seat is available
    for (int i = 0; i < numTicketHistoryEntries; i++) {
        if (ticketHistory[i].busScheduleIndex == passenger.busScheduleIndex &&
            ticketHistory[i].numSeats == selectedSeatNumber) {
            isSeatAvailable = 0; // Seat is already booked
            break;
        }
    }

    if (!isSeatAvailable) {
        printf("Selected seat is already booked. Please choose another seat.\n");
    } else {
        // Update the total ticket price and issue the ticket
        passenger.numSeats = selectedSeatNumber;
        passenger.totalTicketPrice = schedules[passenger.busScheduleIndex - 1].ticketPrice;
        printf("Ticket booked successfully for %s on schedule %d for seat %s.\n",
               passenger.name, passenger.busScheduleIndex, selectedSeat);
        printf("Total Ticket Price: %.2f\n", passenger.totalTicketPrice);
        passenger.isTicketIssued = 1;
        updateAvailableSeats(passenger.busScheduleIndex, 1); // Mark the selected seat as booked
    }
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


void displayTicketHistory() {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Ticket Issuing History********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s%-20s\n", "Passenger", "Schedule ID", "Number of Seats", "Total Ticket Price");
    for (int i = 0; i < numTicketHistoryEntries; i++) {
        printf("\t\t\t\t\t\t\t\t%-20s%-20d%-20d%-20.2f\n", ticketHistory[i].passengerName, ticketHistory[i].busScheduleIndex,
               ticketHistory[i].numSeats, ticketHistory[i].totalTicketPrice);
    }
    printf("\t\t\t\t\t\t\t\t=============================================================================================\n");
}

void displayCancellationHistory() {
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Booking Cancellation History********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s\n", "Passenger", "Schedule ID", "Number of Seats");
    for (int i = 0; i < numCancellationHistoryEntries; i++) {
        printf("\t\t\t\t\t\t\t\t%-20s%-20d%-20d\n", cancellationHistory[i].passengerName, cancellationHistory[i].busScheduleIndex,
               cancellationHistory[i].numSeats);
    printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    }
}

void cancelBooking() {
    if (!passenger.isTicketIssued) {
        printf("You have not booked a ticket yet.\n");
        return;
    }

    int scheduleIndex = passenger.busScheduleIndex;
    char seatType;
    int seatNumber;

    printf("Enter the seat you want to cancel (e.g., A1, W2): ");
    char selectedSeat[3]; // A string to store the seat input, e.g., "A1" or "W2"
    scanf("%s", selectedSeat);

    if ((selectedSeat[0] != 'A' && selectedSeat[0] != 'W') || !isdigit(selectedSeat[1])) {
        printf("Invalid seat format. Please use the format A1 or W2.\n");
        return;
    }

    seatType = selectedSeat[0];
    seatNumber = selectedSeat[1] - '0';

    // Calculate the index of the selected seat in the availableSeats array
    int seatIndex = -1;
    if (seatType == 'A') {
        seatIndex = (seatNumber - 1) + ((seatNumber - 1) / 4);
    } else if (seatType == 'W') {
        seatIndex = (seatNumber - 1) * 2;
    }

    if (seatIndex >= 0 && seatIndex < 57) {
        // Check if the selected seat is already booked
        if (schedules[scheduleIndex - 1].availableSeats[seatIndex] == 'X') {
            // Mark the seat as available
            schedules[scheduleIndex - 1].availableSeats[seatIndex] = 'A';

            // Remove the canceled seat from ticket history
            for (int i = 0; i < numTicketHistoryEntries; i++) {
                if (strcmp(ticketHistory[i].passengerName, passenger.name) == 0 &&
                    ticketHistory[i].busScheduleIndex == scheduleIndex &&
                    ticketHistory[i].numSeats == seatNumber) {
                    // Mark the ticket as canceled in ticket history
                    ticketHistory[i].busScheduleIndex = -1;
                    break;
                }
            }

            printf("Booking for seat %c%d on schedule %d is canceled.\n", seatType, seatNumber, scheduleIndex);
            updateAvailableSeats(scheduleIndex, -1);
        } else {
            printf("The selected seat is not booked. No action taken.\n");
        }
    } else {
        printf("Invalid seat number. Please choose a valid seat.\n");
    }
}





void issueTicket() {
    if (!passenger.isTicketIssued) {
        printf("\t\t\t\t\t\t\t\tYou have not booked a ticket yet.\n");

    }
    // Add issued ticket to the history
    strcpy(ticketHistory[numTicketHistoryEntries].passengerName, passenger.name);
    ticketHistory[numTicketHistoryEntries].busScheduleIndex = passenger.busScheduleIndex;
    ticketHistory[numTicketHistoryEntries].numSeats = passenger.numSeats;
    ticketHistory[numTicketHistoryEntries].totalTicketPrice = passenger.totalTicketPrice;
    numTicketHistoryEntries++;


        printf("\t\t\t\t\t\t\t\t============================================================================================\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t********Ticket Issued successfully********\n");
    printf("\t\t\t\t\t\t\t\t%-20s%-20s%-20s%-20s\n", "Passenger", "Schedule ID", "Number of Seats", "Total Ticket Price");
    for (int i = 0; i < numTicketHistoryEntries; i++) {
        printf("\t\t\t\t\t\t\t\t%-20s%-20d%-20d%-20.2f\n", ticketHistory[i].passengerName, ticketHistory[i].busScheduleIndex,
               ticketHistory[i].numSeats, ticketHistory[i].totalTicketPrice);
    }
    printf("\t\t\t\t\t\t\t\t=============================================================================================\n");
    passenger.isTicketIssued = 0;
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
    printf("\t\t\t\t\t\t\t\tPassword:\t");
    scanf("%s",pword);
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
                        printf("\t\t\t\t\t\t\t\t9. View available seats\n");

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

                                break;



                            case 0:
                                printf("\t\t\t\t\t\t\t\tGoodbye!\n");
                                return 0;
                            default:
                                printf("\t\t\t\t\t\t\t\tInvalid choice. Please try again.\n");
                        }

                        printf("\t\t\t\t\t\t\t\tDo you want to continue with another operation? (y/n): \n\n");
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

        printf("\t\t\t\t\t\t\t\tDo you want to perform another operation? (y/n): ");
        scanf(" %c", &cont);
    } while (cont == 'y' || cont == 'Y');

    atexit(saveBusSchedulesToFile);
    atexit(saveTicketHistoryToFile);
    atexit(saveCancellationHistoryToFile);

    return 0;
}
