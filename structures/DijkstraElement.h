#ifndef DijkstraElement_h
#define DijkstraElement_h

typedef struct DijkstraElement {
    List* node;
    int weight;
    List* prevNode;
    bool isChecked;
    DijkstraElement* next;
} DijkstraElement;

#endif /* DijkstraElement_h */
