#ifndef AdjacencyListElement_h
#define AdjacencyListElement_h

typedef struct AdjacencyListElement {
    List* nodeA;
    List* nodeB;
    int weight = 0;
    AdjacencyListElement* next;
} AdListEl;

#endif /* AdjacencyListElement_h */
