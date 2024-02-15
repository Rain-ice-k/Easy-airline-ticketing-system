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

// 函数声明
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
// 主函数
int main() {
    Flight *flights = NULL;

    int choice;
    do {
        printf("欢迎使用本航空订票系统\n");
        printf("1. 订票\n");
        printf("2. 退票\n");
        printf("3. 查询航班\n");
        printf("4. 显示所有航班\n");
        printf("5. 添加航班\n");
        printf("0. 退出\n");
        printf("请输入您的选择: ");
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
    printf("请输入航班目的地（国内）: ");
    scanf("%s", newFlight.destination);
    printf("请输入航班号: ");
    scanf("%s", newFlight.flightNumber);
    printf("请输入飞机号: ");
    scanf("%s", newFlight.planeNumber);
    printf("请输入飞行星期几（1-7）: ");
    scanf("%d", &newFlight.weekday);
    printf("请输入乘员定额: ");
    scanf("%d", &newFlight.capacity);
    newFlight.ticketsLeft = newFlight.capacity;
    newFlight.bookedList = NULL;
    newFlight.waitingList = NULL;
    newFlight.next = NULL;

    Flight *newNode = (Flight*)malloc(sizeof(Flight));
    *newNode = newFlight;
    newNode->next = *head;
    *head = newNode;

    printf("航班添加成功!\n");
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
        printf("航班未找到。\n");
        return;
    }

    if (isWaiting || flight->ticketsLeft < newCustomer.ticketAmount) {
        printf("余票不足，已将您加入等候名单。\n");
        newCustomer.next = flight->waitingList;
        flight->waitingList = createCustomer(newCustomer.name, newCustomer.ticketAmount, newCustomer.classLevel);
    } else {
        flight->ticketsLeft -= newCustomer.ticketAmount;
        newCustomer.next = flight->bookedList;
        flight->bookedList = createCustomer(newCustomer.name, newCustomer.ticketAmount, newCustomer.classLevel);
        printf("订票成功。\n");
    }
}

void cancelTicket(Flight *head, char *flightNumber, char *customerName) {
    Flight *flight = findFlight(head, flightNumber);
    if (!flight) {
        printf("航班未找到。\n");
        return;
    }

    Customer **prev = &flight->bookedList;
    Customer *current = flight->bookedList;
    while (current) {
        if (strcmp(current->name, customerName) == 0) {
            *prev = current->next;
            flight->ticketsLeft += current->ticketAmount;
            free(current);
            printf("退票成功。\n");
            processWaitList(flight);
            return;
        }
        prev = &current->next;
        current = current->next;
    }
    printf("未找到客户。\n");
}

void printFlightInfo(Flight *head, char *destination) {
    Flight *current = head;
    while (current) {
        if (strcmp(current->destination, destination) == 0) {
            printf("航班号: %s, 飞机号: %s, 星期: %d, 定额: %d, 余票: %d\n",
                   current->flightNumber, current->planeNumber, current->weekday, current->capacity, current->ticketsLeft);
        }
        current = current->next;
    }
}

void printAllFlights(Flight *head) {
    Flight *current = head;
    while (current) {
        printf("目的地: %s, 航班号: %s, 飞机号: %s, 星期: %d, 定额: %d, 余票: %d\n",
               current->destination, current->flightNumber, current->planeNumber, current->weekday, current->capacity, current->ticketsLeft);
        current = current->next;
    }
}
void handleBooking(Flight *head) {
    char flightNumber[10];
    Customer newCustomer;

    printf("请输入航班号: ");
    scanf("%s", flightNumber);

    printf("请输入您的姓名: ");
    scanf("%s", newCustomer.name);

    printf("请输入订票数量: ");
    scanf("%d", &newCustomer.ticketAmount);

    printf("请输入仓位等级 (1, 2, 或 3): ");
    scanf("%d", &newCustomer.classLevel);

    bookTicket(head, flightNumber, newCustomer, 0);
}

void handleCancellation(Flight *head) {
    char flightNumber[10];
    char customerName[50];

    printf("请输入航班号: ");
    scanf("%s", flightNumber);

    printf("请输入您的姓名: ");
    scanf("%s", customerName);

    cancelTicket(head, flightNumber, customerName);
}

void handleQuery(Flight *head) {
    char destination[50];

    printf("请输入目的地: ");
    scanf("%s", destination);

    printFlightInfo(head, destination);
}

void processWaitList(Flight *flight) {
    while (flight->waitingList && flight->ticketsLeft > 0) {
        Customer *waitListedCustomer = flight->waitingList;
        if (flight->ticketsLeft >= waitListedCustomer->ticketAmount) {
            flight->waitingList = waitListedCustomer->next;
            flight->ticketsLeft -= waitListedCustomer->ticketAmount;
            printf("等候名单客户 %s 已成功订票。\n", waitListedCustomer->name);
            free(waitListedCustomer);
        } else {
            break;
        }
    }
}

void saveFlightData(Flight *head) {
    FILE *file = fopen("flights.txt", "w");
    if (!file) {
        printf("无法打开文件以保存数据。\n");
        return;
    }

    Flight *current = head;
    while (current) {
        // 保存航班信息
        fprintf(file, "%s %s %s %d %d %d\n",
                current->destination, current->flightNumber, current->planeNumber,
                current->weekday, current->capacity, current->ticketsLeft);

        // 保存已订票客户信息
        Customer *bookedCustomer = current->bookedList;
        while (bookedCustomer) {
            fprintf(file, "B %s %d %d\n", bookedCustomer->name, bookedCustomer->ticketAmount, bookedCustomer->classLevel);
            bookedCustomer = bookedCustomer->next;
        }

        // 保存等候名单客户信息
        Customer *waitingCustomer = current->waitingList;
        while (waitingCustomer) {
            fprintf(file, "W %s %d %d\n", waitingCustomer->name, waitingCustomer->ticketAmount, waitingCustomer->classLevel);
            waitingCustomer = waitingCustomer->next;
        }

        fprintf(file, "EndFlight\n");
        current = current->next;
    }

    fclose(file);
    printf("数据已保存到文件。\n");
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
