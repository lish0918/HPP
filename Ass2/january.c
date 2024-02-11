/*Using global variable instead of call by reference, overly complicated and unnecessary checks in add and delete node functions.*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int day;
    float minTemp;
    float maxTemp;
    struct Node* next;
} Node;

Node* head = NULL;

Node* createNode(int day, float min, float max) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->day = day;
    newNode->minTemp = min;
    newNode->maxTemp = max;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node* newNode) {
    if(head == NULL || head->day > newNode->day) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* current = head;
        while(current->next != NULL && current->next->day < newNode->day) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

void deleteNode(int day) {
    Node* temp = head, *prev;

    if(temp != NULL && temp->day == day) {
        head = temp->next;
        free(temp);
        return;
    }

    while(temp != NULL && temp->day != day) {
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

void printList() {
    Node* node = head;
    printf("day min max\n");
    while(node != NULL) {
        printf("%d %f %f\n", node->day, node->minTemp, node->maxTemp);
        node = node->next;
    }
}

int searchNode(int day) {
    Node* node = head;
    while(node != NULL) {
        if(node->day == day) return 1;
        node = node->next;
    }
    return 0;
}

int main() {
    char command;
    int day;
    float min, max;

    while(1) {
        printf("Enter command: ");
        scanf(" %c", &command);
        switch(command) {
            case 'A':
                scanf("%d %f %f", &day, &min, &max);
                if (day < 1 || day > 31 || min > max) {
                    printf("Invalid Command!\n");
                    break;
                }
                if (searchNode(day) != 0) {
                    printf("Duplicate Day!\n");
                    break;
                }
                insertNode(createNode(day, min, max));
                break;
            case 'D':
                scanf("%d", &day);
                deleteNode(day);
                break;
            case 'P':
                printList();
                break;
            case 'Q':
                while(head != NULL) {
                    Node* temp = head;
                    head = head->next;
                    free(temp);
                }
                return 0;
            default:
                printf("Invalid Command!\n");
        }
    }
}