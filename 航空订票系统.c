#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Customer {
    char name[50];
    int ticketAmount;
    int classLevel;
    struct Customer *next;
} Customer;
typedef struct Flight {
    char destination[50];
    char flightNumber[10];
    char planeNumber[10];
    int weekday;
    int capacity;
    int ticketsLeft;
    Customer *bookedList;
    Customer *waitingList;
    struct Flight *next;
} Flight;

// ��������
void addFlight(Flight **head);
void bookTicket(Flight *head, char *flightNumber, Customer newCustomer, int isWaiting);
void cancelTicket(Flight *head, char *flightNumber, char *customerName);
void printFlightInfo(Flight *head, char *destination);
void printAllFlights(Flight *head);
Customer* createCustomer(char *name, int ticketAmount, int classLevel);
Flight* findFlight(Flight *head, char *flightNumber);
void handleBooking(Flight *head);
void handleCancellation(Flight *head);
void handleQuery(Flight *head);
void processWaitList(Flight *flight);
void saveFlightData(Flight *head);
void freeCustomerList(Customer *head);
void freeFlightList(Flight *head);
// ������
int main() {
    Flight *flights = NULL;

    int choice;
    do {
        printf("��ӭʹ�ñ����ն�Ʊϵͳ\n");
        printf("1. ��Ʊ\n");
        printf("2. ��Ʊ\n");
        printf("3. ��ѯ����\n");
        printf("4. ��ʾ���к���\n");
        printf("5. ��Ӻ���\n");
        printf("0. �˳�\n");
        printf("����������ѡ��: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                handleBooking(flights);
                break;
            case 2:
                handleCancellation(flights);
                break;
            case 3:
                handleQuery(flights);
                break;
            case 4:
                printAllFlights(flights);
                break;
            case 5:
                addFlight(&flights);
                break;
        }
    } while (choice != 0);

    saveFlightData(flights);
    freeFlightList(flights);
    return 0;
}
void addFlight(Flight **head) {
    Flight newFlight;
    printf("�����뺽��Ŀ�ĵأ����ڣ�: ");
    scanf("%s", newFlight.destination);
    printf("�����뺽���: ");
    scanf("%s", newFlight.flightNumber);
    printf("������ɻ���: ");
    scanf("%s", newFlight.planeNumber);
    printf("������������ڼ���1-7��: ");
    scanf("%d", &newFlight.weekday);
    printf("�������Ա����: ");
    scanf("%d", &newFlight.capacity);
    newFlight.ticketsLeft = newFlight.capacity;
    newFlight.bookedList = NULL;
    newFlight.waitingList = NULL;
    newFlight.next = NULL;

    Flight *newNode = (Flight*)malloc(sizeof(Flight));
    *newNode = newFlight;
    newNode->next = *head;
    *head = newNode;

    printf("������ӳɹ�!\n");
}

Customer* createCustomer(char *name, int ticketAmount, int classLevel) {
    Customer *newCustomer = (Customer*)malloc(sizeof(Customer));
    strcpy(newCustomer->name, name);
    newCustomer->ticketAmount = ticketAmount;
    newCustomer->classLevel = classLevel;
    newCustomer->next = NULL;
    return newCustomer;
}

Flight* findFlight(Flight *head, char *flightNumber) {
    Flight *current = head;
    while (current != NULL) {
        if (strcmp(current->flightNumber, flightNumber) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
void bookTicket(Flight *head, char *flightNumber, Customer newCustomer, int isWaiting) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("����δ�ҵ���\n");
        return;
    }

    if (isWaiting || flight->ticketsLeft < newCustomer.ticketAmount) {
        printf("��Ʊ���㣬�ѽ�������Ⱥ�������\n");
        newCustomer.next = flight->waitingList;
        flight->waitingList = createCustomer(newCustomer.name, newCustomer.ticketAmount, newCustomer.classLevel);
    } else {
        flight->ticketsLeft -= newCustomer.ticketAmount;
        newCustomer.next = flight->bookedList;
        flight->bookedList = createCustomer(newCustomer.name, newCustomer.ticketAmount, newCustomer.classLevel);
        printf("��Ʊ�ɹ���\n");
    }
}

void cancelTicket(Flight *head, char *flightNumber, char *customerName) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("����δ�ҵ���\n");
        return;
    }

    Customer **prev = &flight->bookedList;
    Customer *current = flight->bookedList;
    while (current) {
        if (strcmp(current->name, customerName) == 0) {
            *prev = current->next;
            flight->ticketsLeft += current->ticketAmount;
            free(current);
            printf("��Ʊ�ɹ���\n");
            processWaitList(flight);
            return;
        }
        prev = &current->next;
        current = current->next;
    }
    printf("δ�ҵ��ͻ���\n");
}

void printFlightInfo(Flight *head, char *destination) {
    Flight *current = head;
    while (current) {
        if (strcmp(current->destination, destination) == 0) {
            printf("�����: %s, �ɻ���: %s, ����: %d, ����: %d, ��Ʊ: %d\n",
                   current->flightNumber, current->planeNumber, current->weekday, current->capacity, current->ticketsLeft);
        }
        current = current->next;
    }
}

void printAllFlights(Flight *head) {
    Flight *current = head;
    while (current) {
        printf("Ŀ�ĵ�: %s, �����: %s, �ɻ���: %s, ����: %d, ����: %d, ��Ʊ: %d\n",
               current->destination, current->flightNumber, current->planeNumber, current->weekday, current->capacity, current->ticketsLeft);
        current = current->next;
    }
}
void handleBooking(Flight *head) {
    char flightNumber[10];
    Customer newCustomer;

    printf("�����뺽���: ");
    scanf("%s", flightNumber);

    printf("��������������: ");
    scanf("%s", newCustomer.name);

    printf("�����붩Ʊ����: ");
    scanf("%d", &newCustomer.ticketAmount);

    printf("�������λ�ȼ� (1, 2, �� 3): ");
    scanf("%d", &newCustomer.classLevel);

    bookTicket(head, flightNumber, newCustomer, 0);
}

void handleCancellation(Flight *head) {
    char flightNumber[10];
    char customerName[50];

    printf("�����뺽���: ");
    scanf("%s", flightNumber);

    printf("��������������: ");
    scanf("%s", customerName);

    cancelTicket(head, flightNumber, customerName);
}

void handleQuery(Flight *head) {
    char destination[50];

    printf("������Ŀ�ĵ�: ");
    scanf("%s", destination);

    printFlightInfo(head, destination);
}

void processWaitList(Flight *flight) {
    while (flight->waitingList && flight->ticketsLeft > 0) {
        Customer *waitListedCustomer = flight->waitingList;
        if (flight->ticketsLeft >= waitListedCustomer->ticketAmount) {
            flight->waitingList = waitListedCustomer->next;
            flight->ticketsLeft -= waitListedCustomer->ticketAmount;
            printf("�Ⱥ������ͻ� %s �ѳɹ���Ʊ��\n", waitListedCustomer->name);
            free(waitListedCustomer);
        } else {
            break;
        }
    }
}

void saveFlightData(Flight *head) {
    FILE *file = fopen("flights.txt", "w");
    if (!file) {
        printf("�޷����ļ��Ա������ݡ�\n");
        return;
    }

    Flight *current = head;
    while (current) {
        // ���溽����Ϣ
        fprintf(file, "%s %s %s %d %d %d\n",
                current->destination, current->flightNumber, current->planeNumber,
                current->weekday, current->capacity, current->ticketsLeft);

        // �����Ѷ�Ʊ�ͻ���Ϣ
        Customer *bookedCustomer = current->bookedList;
        while (bookedCustomer) {
            fprintf(file, "B %s %d %d\n", bookedCustomer->name, bookedCustomer->ticketAmount, bookedCustomer->classLevel);
            bookedCustomer = bookedCustomer->next;
        }

        // ����Ⱥ������ͻ���Ϣ
        Customer *waitingCustomer = current->waitingList;
        while (waitingCustomer) {
            fprintf(file, "W %s %d %d\n", waitingCustomer->name, waitingCustomer->ticketAmount, waitingCustomer->classLevel);
            waitingCustomer = waitingCustomer->next;
        }

        fprintf(file, "EndFlight\n");
        current = current->next;
    }

    fclose(file);
    printf("�����ѱ��浽�ļ���\n");
}
void freeCustomerList(Customer *head) {
    Customer *current = head;
    Customer *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

void freeFlightList(Flight *head) {
    Flight *current = head;
    Flight *temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        freeCustomerList(temp->bookedList);
        freeCustomerList(temp->waitingList);
        free(temp);
    }
}
