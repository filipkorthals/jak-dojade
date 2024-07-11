#ifndef SingleLinkedList_h
#define SingleLinkedList_h

#define TEXT 10

typedef struct SingleLinkedList {
    char value[TEXT];
    Coords coords;
    SingleLinkedList* next;
} List;

#endif /* SingleLinkedList_h */
