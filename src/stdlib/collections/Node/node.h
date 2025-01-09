#pragma once
#include "../../../stdint.h"

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *createNode(int data);
Node *searchNode(Node *head, int value);
void removeLastNode(Node* head);
void clearList(Node **head);
void insertAfter(Node* head, int key, int newData);
void insertNodeAtEnd(Node *end, int data);
void printList(Node *head);
