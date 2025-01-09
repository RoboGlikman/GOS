#include "node.h"
#include "../../../MemoryManager/mm.h"
#include "../../../stdint.h"
#include "../../stdio/stdio.h"
Node *createNode(int data) {
    Node *newNode = (Node *)kmalloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    
    return newNode;
}


void insertAfter(Node* head, int key, int newData) {
    Node* curr = head;

    while (curr != NULL) {
        if (curr->data == key)
            break;
        curr = curr->next;
    }

    if (curr == NULL){
        printf("couldnt insert after, kery not found\n");
        return;
    }

    Node* newNode = createNode(newData);
    newNode->next = curr->next;
    curr->next = newNode;
}

Node *searchNode(Node *head, int value){
    Node *temp = head;
    while(temp != NULL){
        if (temp->data == value)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

void insertNodeAtEnd(Node *end, int data){
    Node* new_node = createNode(data);
    end->next = new_node;
}

void removeLastNode(Node* head){
    if (head == NULL) 
        return;

    if (head->next == NULL) {
        kfree(head);
        return;
    }

    Node* second_last = head;
    while (second_last->next->next != NULL) 
        second_last = second_last->next;
    
    kfree(second_last->next);
    second_last->next = NULL;
}

void clearList(Node **head){
    Node *temp = *head;
    while (temp != NULL) {
        Node* next = temp->next;
        kfree(temp);
        temp = next;
        
    }
    *head = NULL;
}

void printList(Node *head){
    Node* temp = head;
    while (temp != NULL){
        printf("%d --> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}


