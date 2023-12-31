
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FLIGHTS 100
#define MAX_BOOKINGS 100

// Complex data type for flight
typedef struct {
    char flightNumber[10];
    char origin[50];
    char destination[50];
    char date[15];
    double price;
} Flight;

typedef struct {
    int bookingID;  // Unique ID for each booking
    char passengerName[100];
    char flightNumber[10];
    bool paymentStatus; // true if payment is successful, false otherwise
} Booking;

// Payment methods enumeration
typedef enum {
    CREDIT_DEBIT_CARD,
    PAYPAL,
    APPLE_PAY
} PaymentMethod;

// Function prototypes for flight reservation system
void searchFlight(const char* origin, const char* destination, const char* date, Flight* flights, int numFlights);
bool processCreditCardPayment();
bool processCreditDebitCardPayment();
bool processPaypalPayment();
bool processApplePayPayment();
void bookFlight(const char* flightNumber, Flight* flights, int numFlights);
void processPayment(Booking* bookings, int numBookings);
void writeFlightsToFile(const char* filename, Flight* flights, int numFlights);
void readFlightsFromFile(const char* filename, Flight** flights, int* numFlights);

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // Command-line argument handling
    if (argc < 2) {
        printf("Usage: %s FlightData.txt\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Create or load flight data from a file
    Flight* flights = NULL;
    int numFlights = 0;
    readFlightsFromFile(argv[1], &flights, &numFlights);

    printf("Welcome to the Flight Reservation System!\n");

    int choice;
    do {
        printf("1. Search Flights\n");
        printf("2. Book Flight\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            char origin[50];
            char destination[50];
            char date[15];

            printf("Enter origin: ");
            scanf("%49s", origin);
            printf("Enter destination: ");
            scanf("%49s", destination);
            printf("Enter date: ");
            scanf("%14s", date);

            searchFlight(origin, destination, date, flights, numFlights);
            break;
        }

        case 2: {
            char flightNumber[10];
            printf("Enter the flight number to book: ");
            scanf("%9s", flightNumber);
            bookFlight(flightNumber, flights, numFlights);
            break;
        }

        case 3:
            writeFlightsToFile(argv[1], flights, numFlights);
            printf("Thank you for using the flight reservation system!\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 3);

    // Free dynamically allocated memory before exit
    free(flights);

    return 0;
}

void searchFlight(const char* origin, const char* destination, const char* date, Flight* flights, int numFlights) {
    int foundFlights = 0;
    printf("Available flights: Origin=%s, Destination=%s, Date=%s\n", origin, destination, date);
    if (foundFlights != 0) {
        for (int i = 0; i < numFlights; i++) {
            if (strcmp(flights[i].origin, origin) == 0 &&
                strcmp(flights[i].destination, destination) == 0 &&
                strcmp(flights[i].date, date) == 0) {
                printf("%s - %s to %s, Date: %s, Price: $%.2f\n",
                    flights[i].flightNumber, flights[i].origin, flights[i].destination,
                    flights[i].date, flights[i].price);
                foundFlights++;
            }
        }
    }
    else {
        printf("No available flights matching the criteria.\n");
    }
}
bool processCreditCardPayment() {
    // Simulate credit card payment processing
    return rand() % 2 == 0; // Randomly return true or false for success/failure
}

bool processCreditDebitCardPayment() {
    // Simulate credit/debit card payment processing
    // Add your payment processing logic here
    return processCreditCardPayment();
}

bool processPaypalPayment() {
    // Simulate PayPal payment processing
    // Add your payment processing logic here
    return rand() % 2 == 0; // Randomly return true or false for success/failure
}

bool processApplePayPayment() {
    // Simulate Apple Pay payment processing
    // Add your payment processing logic here
    return rand() % 2 == 0; // Randomly return true or false for success/failure
}

void bookFlight(const char* flightNumber, Flight* flights, int numFlights) {
    for (int i = 0; i < numFlights; i++) {
        if (strcmp(flights[i].flightNumber, flightNumber) == 0) {
            printf("Booking confirmed for Flight %s from %s to %s, Date: %s, Price: $%.2f\n",
                flights[i].flightNumber, flights[i].origin, flights[i].destination,
                flights[i].date, flights[i].price);

            // Create a booking
            Booking booking;
            booking.bookingID = rand() % 1000000 + 100000;
            printf("Enter passenger name: ");
            scanf("%99s", booking.passengerName);
            strcpy(booking.flightNumber, flightNumber);

            // Process payment
            processPayment(&booking, 1);

            return;
        }
    }

    printf("Flight not found. Please check the flight number and try again.\n");
}

void processPayment(Booking* bookings, int numBookings) {
    printf("Select payment method:\n");
    printf("1. Credit/Debit Card\n");
    printf("2. PayPal\n");
    printf("3. Apple Pay\n");
    printf("Enter your choice: ");
    int paymentChoice;
    scanf("%d", &paymentChoice);

    switch (paymentChoice) {
    case 1:
        bookings->paymentStatus = processCreditDebitCardPayment();
        break;
    case 2:
        bookings->paymentStatus = processPaypalPayment();
        break;
    case 3:
        bookings->paymentStatus = processApplePayPayment();
        break;
    default:
        printf("Invalid payment method. Payment failed.\n");
        bookings->paymentStatus = false;
        break;
    }

    printf("Payment Status: %s\n", bookings->paymentStatus ? "Successful" : "Failed");
}

void writeFlightsToFile(const char* filename, Flight* flights, int numFlights) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numFlights; i++) {
        fprintf(file, "%s,%s,%s,%s,%.2f\n", flights[i].flightNumber, flights[i].origin, flights[i].destination, flights[i].date, flights[i].price);
    }

    fclose(file);
}

void readFlightsFromFile(const char* filename, Flight** flights, int* numFlights) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        // File does not exist, create a new file
        file = fopen(filename, "w");
        if (file == NULL) {
            printf("Error creating file.\n");
            return;
        }
        fclose(file);

        *flights = NULL;
        *numFlights = 0;
        return;
    }

    int count = 0;
    char line[256];

    // Count the number of lines in the file (number of flights)
    while (fgets(line, sizeof(line), file) != NULL) {
        count++;
    }

    *flights = (Flight*)malloc(count * sizeof(Flight));
    if (*flights == NULL) {
        printf("Error allocating memory for flights.\n");
        fclose(file); 
            return;
    }

    rewind(file);

    // Read each line and parse flight data
    for (int i = 0; i < count; i++) {
        if (fgets(line, sizeof(line), file) != NULL) {
            sscanf(line, "%[^,],%[^,],%[^,],%[^,],%lf\n",
                (*flights)[i].flightNumber, (*flights)[i].origin, (*flights)[i].destination,
                (*flights)[i].date, &(*flights)[i].price);
        }
    }

    *numFlights = count;
    fclose(file);
}
