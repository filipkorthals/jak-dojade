#include <iostream>
#include <ctype.h>
#include <limits.h>
#include "structures/Coords.h"
#include "structures/SingleLinkedList.h"
#include "structures/AdjacencyListElement.h"
#include "structures/DijkstraElement.h"

using namespace std;

Coords findStar(char* map, int i, int j, Coords size);
List* initList();
AdListEl* initAdListEl();
DijkstraElement* initDijkstraElement();
List* checkIfNodeExists(List* nodesHead, Coords coords);
void createNewNode(List* nodesHead, char* value, Coords coords);
bool isNode(char* map, int i, int j, Coords size);
List* findNodeA(char* map, List* nodesHead, Coords coords, Coords size);
List* findNodeB(char* map, List* nodesHead, Coords coords, Coords size);
AdListEl* createGraphElements(List* cityA, List* cityB, int weight);
void addElementToGraph(AdListEl* graphHead, List* cityA, List* cityB, int weight);
void checkNodesAround(List* nodesHead, List** cityA, List** cityB, int* weight, int i, int j, Coords size, int k);
List* findCity(List* nodesHead, char* cityName);
void getFlights(AdListEl* graphHead, List* nodesHead);
bool areNodesLeft(DijkstraElement* dijkstraHead);
DijkstraElement* getSmallestElement(DijkstraElement* dijkstraHead);
void deleteDijkstraElements(DijkstraElement** dijkstraHead);
void printCityName(char* name);
void dijkstra(AdListEl *graphHead, List* nodesHead, List* cityA, List* cityB, bool addRoute);
void getQueries(AdListEl* graphHead, List* nodesHead);
void deleteNodes(List** nodesHead);
void deleteGraph(AdListEl** graphHead);

int main(int argc, const char * argv[]) {
    Coords size;
    bool isSign = false;
    List* nodesHead = NULL;
    AdListEl* graphHead = NULL;
    scanf("%d", &size.x);
    scanf("%d", &size.y);
    char* map = (char*)malloc(sizeof(char) * (size.x * size.y));
    char ch = getchar();
    
    for (int i = 0; i < size.y; i++) {
        for (int j = 0; j < size.x; j++) {
            ch = getchar();
            if (ch == '\n')
                ch = getchar();
            map[i * size.x + j] = ch;
        }
        ch = getchar();
    }
    
    Coords tmpCoords = {NULL, NULL};
    char tmpName[TEXT];
    memset(tmpName, '\0', TEXT);
    int index = 0;
    
    for (int i = 0; i < size.y; i++) {
        for (int j = 0; j < size.x; j++) {
            ch = map[i * size.x + j];
            if (isalpha(ch) || isdigit(ch)) {
                if (!isSign) {
                    isSign = true;
                    tmpCoords = findStar(map, i, j, size);
                }
                tmpName[index] = ch;
                index++;
            }
            else {
                if (isSign) {
                    isSign = false;
                    index = 0;
                    if (tmpCoords.x == NULL && tmpCoords.y == NULL)
                        tmpCoords = findStar(map, i, j - 1, size);
                    if (nodesHead == NULL) {
                        nodesHead = initList();
                        strcpy(nodesHead->value, tmpName);
                        nodesHead->coords = tmpCoords;
                    }
                    else
                        createNewNode(nodesHead, tmpName, tmpCoords);
                    memset(tmpName, '\0', TEXT);
                    tmpCoords = {NULL, NULL};
                }
                if (ch == '#' && isNode(map, i, j, size)) {
                    memset(tmpName, '\0', TEXT);
                    tmpName[0] = '#';
                    if (nodesHead == NULL) {
                        nodesHead = initList();
                        strcpy(nodesHead->value, tmpName);
                        nodesHead->coords = {j, i};
                    }
                    else
                        createNewNode(nodesHead, tmpName, {j, i});
                }
            }
        }
    }
    
    for (int i = 0; i < size.y; i++) {
        for (int j = 0; j < size.x; j++) {
            ch = map[i * size.x + j];
            if (ch == '#') {
                List *cityA = NULL;
                List *cityB = NULL;
                int weight = 0;
                
                if (checkIfNodeExists(nodesHead, {j, i}) == NULL) {
                    cityA = findNodeA(map, nodesHead, {j, i}, size);
                    cityB = findNodeB(map, nodesHead, {j, i}, size);

                    if (cityA->coords.x == cityB->coords.x)
                        weight = cityB->coords.y - cityA->coords.y;
                    else if (cityA->coords.y == cityB->coords.y)
                        weight = cityB->coords.x - cityA->coords.x;
                    
                    if (cityA != NULL) {
                        if (graphHead == NULL) {
                            graphHead = createGraphElements(cityA, cityB, weight);
                        }
                        else
                            addElementToGraph(graphHead, cityA, cityB, weight);
                    }
                }
                else {
                    for (int k = 0; k < 4; k++) {
                        checkNodesAround(nodesHead, &cityA, &cityB, &weight, i, j, size, k);
                        
                        if (cityA != NULL && cityB != NULL) {
                            if (graphHead == NULL) {
                                graphHead = createGraphElements(cityA, cityB, weight);
                            }
                            else
                                addElementToGraph(graphHead, cityA, cityB, weight);
                        }
                    }
                }
            }
            else if (ch == '*') {
                List *cityA = NULL;
                List *cityB = NULL;
                int weight = 0;
                
                cityA = checkIfNodeExists(nodesHead, {j, i});
                if (graphHead == NULL) {
                    graphHead = createGraphElements(cityA, cityA, 0);
                }
                else
                    addElementToGraph(graphHead, cityA, cityA, 0);
                
                for (int k = 0; k < 4; k++) {
                    checkNodesAround(nodesHead, &cityA, &cityB, &weight, i, j, size, k);
                    
                    if (cityA != NULL && cityB != NULL) {
                        if (graphHead == NULL) {
                            graphHead = createGraphElements(cityA, cityB, weight);
                        }
                        else
                            addElementToGraph(graphHead, cityA, cityB, weight);
                    }
                }
            }
        }
    }

    free(map);
    
    int k;
    scanf("%d", &k);
    
    for (int i = 0; i < k; i++)
        getFlights(graphHead, nodesHead);
    
    int q;
    scanf("%d", &q);

    for (int i = 0; i < q; i++)
        getQueries(graphHead, nodesHead);
    
    deleteNodes(&nodesHead);
    deleteGraph(&graphHead);
    
    return 0;
}

Coords findStar(char* map, int i, int j, Coords size) {
    if (map[i * size.x + j] == '*')
        return {j, i};
    if (i > 0 && map[(i - 1) * size.x + j] == '*')  // gora
        return {j, i - 1};
    if (i > 0 && j < size.x - 1 && map[(i - 1) * size.x + j + 1] == '*')    // prawy gorny rog
        return {j + 1, i - 1};
    if (j < size.x - 1 && map[i * size.x + j + 1] == '*')   // prawo
        return {j + 1, i};
    if (i < size.y - 1 && j < size.x - 1 && map[(i + 1) * size.x + j + 1] == '*')   // prawy dolny rog
        return {j + 1, i + 1};
    if (i < size.y - 1 && map[(i + 1) * size.x + j] == '*') // dol
        return {j, i + 1};
    if (i < size.y - 1 && j > 0 && map[(i + 1) * size.x + j - 1] == '*')    // lewy dolny rog
        return {j - 1, i + 1};
    if (j > 0 && map[i * size.x + j - 1] == '*')    // lewo
        return {j - 1, i};
    if (i > 0 && j > 0 && map[(i - 1) * size.x + j - 1] == '*') // lewy gorny rog
        return {j - 1, i - 1};
    return {NULL, NULL};
}

List* initList() {
    List* tmp = new List;
    tmp->coords = {NULL, NULL};
    memset(tmp->value, '\0', TEXT);
    tmp->next = NULL;
    return tmp;
}

AdListEl* initAdListEl() {
    AdListEl* tmp = new AdListEl;
    tmp->nodeA = NULL;
    tmp->nodeB = NULL;
    tmp->next = NULL;
    return tmp;
}

DijkstraElement* initDijkstraElement() {
    DijkstraElement *tmp = new DijkstraElement;
    tmp->node = NULL;
    tmp->weight = INT_MAX;
    tmp->prevNode = NULL;
    tmp->isChecked = false;
    tmp->next = NULL;
    return tmp;
}

List* checkIfNodeExists(List* nodesHead, Coords coords) {
    List* tmp = nodesHead;
    while (tmp != NULL) {
        if(tmp->coords.x == coords.x && tmp->coords.y == coords.y)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void createNewNode(List* nodesHead, char* value, Coords coords) {
    List* tmp = initList();
    tmp->coords = coords;
    strcpy(tmp->value, value);
    
    List* n = nodesHead;
    while (n->next != NULL)
        n = n->next;
    n->next = tmp;
}

bool isNode(char* map, int i, int j, Coords size) {
    int roadsCounter = 0;
    if (i > 0 && (map[(i - 1) * size.x + j] == '#' || map[(i - 1) * size.x + j] == '*'))
        roadsCounter++;
    if (j < size.x - 1 && (map[i * size.x + j + 1] == '#' || map[i * size.x + j + 1] == '*'))
        roadsCounter++;
    if (i < size.y - 1 && (map[(i + 1) * size.x + j] == '#' || map[(i + 1) * size.x + j] == '*'))
        roadsCounter++;
    if (j > 0 && (map[i * size.x + j - 1] == '#' || map[i * size.x + j - 1] == '*'))
        roadsCounter++;
    
    if(roadsCounter > 2)
        return true;
    
    if (i > 0 && j < size.x - 1 &&
        (map[(i - 1) * size.x + j] == '#' || map[(i - 1) * size.x + j] == '*') &&
        (map[i * size.x + j + 1] == '#' || map[i * size.x + j + 1] == '*'))
        return true;
    if (j < size.x - 1 && i < size.y - 1 &&
        (map[i * size.x + j + 1] == '#' || map[i * size.x + j + 1] == '*') &&
        (map[(i + 1) * size.x + j] == '#' || map[(i + 1) * size.x + j] == '*'))
        return true;
    if (j > 0 && i < size.y - 1 &&
        (map[(i + 1) * size.x + j] == '#' || map[(i + 1) * size.x + j] == '*') &&
        (map[i * size.x + j - 1] == '#' || map[i * size.x + j - 1] == '*'))
        return true;
    if (j > 0 && i > 0 &&
        (map[i * size.x + j - 1] == '#' || map[i * size.x + j - 1] == '*') &&
        (map[(i - 1) * size.x + j] == '#' || map[(i - 1) * size.x + j] == '*'))
        return true;
    
    return false;
}

List* findNodeA(char* map, List* nodesHead, Coords coords, Coords size) {
    int tmpI;
    int tmpJ;
    bool isNodeFound = false;
    
    tmpI = coords.y;
    tmpJ = coords.x - 1;
    while(tmpJ >= 0 && (map[tmpI * size.x + tmpJ] == '#' || map[tmpI * size.x + tmpJ] == '*')) {
        if (checkIfNodeExists(nodesHead, {tmpJ, tmpI}) != NULL) {
            isNodeFound = true;
            break;
        }
        tmpJ--;
    }
    if (isNodeFound)
        return checkIfNodeExists(nodesHead, {tmpJ, tmpI});
    
    tmpI = coords.y - 1;
    tmpJ = coords.x;
    while (tmpI >= 0 && (map[tmpI * size.x + tmpJ] == '#' || map[tmpI * size.x + tmpJ] == '*')) {
        if (checkIfNodeExists(nodesHead, {tmpJ, tmpI}) != NULL) {
            isNodeFound = true;
            break;
        }
        tmpI--;
    }
    if (isNodeFound)
        return checkIfNodeExists(nodesHead, {tmpJ, tmpI});
    
    return NULL;
}

List* findNodeB(char* map, List* nodesHead, Coords coords, Coords size) {
    int tmpI;
    int tmpJ;
    bool isNodeFound = false;
    
    tmpI = coords.y;
    tmpJ = coords.x + 1;
    while(tmpJ < size.x && (map[tmpI * size.x + tmpJ] == '#' || map[tmpI * size.x + tmpJ] == '*')) {
        if (checkIfNodeExists(nodesHead, {tmpJ, tmpI}) != NULL) {
            isNodeFound = true;
            break;
        }
        tmpJ++;
    }
    if (isNodeFound)
        return checkIfNodeExists(nodesHead, {tmpJ, tmpI});
    
    tmpI = coords.y + 1;
    tmpJ = coords.x;
    while (tmpI < size.y && (map[tmpI * size.x + tmpJ] == '#' || map[tmpI * size.x + tmpJ] == '*')) {
        if (checkIfNodeExists(nodesHead, {tmpJ, tmpI}) != NULL) {
            isNodeFound = true;
            break;
        }
        tmpI++;
    }
    if (isNodeFound)
        return checkIfNodeExists(nodesHead, {tmpJ, tmpI});
    
    return NULL;
}

AdListEl* createGraphElements(List* cityA, List* cityB, int weight) {
    AdListEl *tmp = initAdListEl();
    tmp->next = weight == 0 ? NULL : initAdListEl();
    tmp->nodeA = cityA;
    tmp->nodeB = cityB;
    tmp->weight = weight;
    if (weight != 0) {
        tmp->next->nodeA = cityB;
        tmp->next->nodeB = cityA;
        tmp->next->weight = weight;
        tmp->next->next = NULL;
    }
    return tmp;
}

void addElementToGraph(AdListEl* graphHead, List* cityA, List* cityB, int weight) {
    AdListEl *tmp = graphHead;
    AdListEl *firstEl = createGraphElements(cityA, cityB, weight);
    
    while (tmp->next != NULL) {
        if ((tmp->nodeA == cityA && tmp->nodeB == cityB && tmp->weight == weight) ||
            (tmp->nodeA == cityB && tmp->nodeB == cityA && tmp->weight == weight))
            return;
        tmp = tmp->next;
    }
    
    if ((tmp->nodeA == cityA && tmp->nodeB == cityB && tmp->weight == weight) ||
        (tmp->nodeA == cityB && tmp->nodeB == cityA && tmp->weight == weight))
        return;
    
    tmp->next = firstEl;
}

void checkNodesAround(List* nodesHead, List** cityA, List** cityB, int* weight, int i, int j, Coords size, int k) {
    switch (k) {
        case 0:
            if (j < size.x - 1 && checkIfNodeExists(nodesHead, {j + 1, i}) != NULL) {
                *cityA = checkIfNodeExists(nodesHead, {j, i});
                *cityB = checkIfNodeExists(nodesHead, {j + 1, i});
                *weight = 1;
            }
            break;
        case 1:
            if (i < size.y - 1 && checkIfNodeExists(nodesHead, {j, i + 1}) != NULL) {
                *cityA = checkIfNodeExists(nodesHead, {j, i});
                *cityB = checkIfNodeExists(nodesHead, {j, i + 1});
                *weight = 1;
            }
            break;
        case 2:
            if (i > 0 && checkIfNodeExists(nodesHead, {j, i - 1}) != NULL) {
                *cityA = checkIfNodeExists(nodesHead, {j, i});
                *cityB = checkIfNodeExists(nodesHead, {j, i - 1});
                *weight = 1;
            }
            break;
        case 3:
            if (j > 0 && checkIfNodeExists(nodesHead, {j - 1, i}) != NULL) {
                *cityA = checkIfNodeExists(nodesHead, {j, i});
                *cityB = checkIfNodeExists(nodesHead, {j - 1, i});
                *weight = 1;
            }
            break;
        default:
            break;
    }
}

List* findCity(List* nodesHead, char* cityName) {
    List* tmp = nodesHead;
    
    while(tmp != NULL) {
        if (strcmp(tmp->value, cityName) == 0)
            return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void getFlights(AdListEl* graphHead, List* nodesHead) {
    char ch = '\n', tmpName[TEXT];
    memset(tmpName, '\0', TEXT);
    int weight, index = 0;
    AdListEl* tmp = graphHead;
    List *cityA, *cityB;
    
    while (tmp->next != NULL)
        tmp = tmp->next;
    
    tmp->next = initAdListEl();
    
    while (ch == '\n')
        ch = getchar();
    
    while (ch != ' ') {
        tmpName[index] = ch;
        ch = getchar();
        index++;
    }
    
    cityA = findCity(nodesHead, tmpName);
    index = 0;
    memset(tmpName, '\0', TEXT);
    
    ch = getchar();
    while (ch != ' ') {
        tmpName[index] = ch;
        ch = getchar();
        index++;
    }
    
    cityB = findCity(nodesHead, tmpName);
    index = 0;
    memset(tmpName, '\0', TEXT);

    scanf("%d", &weight);
    
    tmp->next->nodeA = cityA;
    tmp->next->nodeB = cityB;
    tmp->next->weight = weight;
}

bool areNodesLeft(DijkstraElement* dijkstraHead) {
    DijkstraElement* tmp = dijkstraHead;
    
    while (tmp != NULL) {
        if (!tmp->isChecked)
            return true;
        tmp = tmp->next;
    }
    return false;
}

DijkstraElement* getSmallestElement(DijkstraElement* dijkstraHead) {
    DijkstraElement* tmp = dijkstraHead;
    DijkstraElement* selectedElement = NULL;
    int tmpWeight = INT_MAX;
    
    while (tmp != NULL) {
        if (tmp->weight <= tmpWeight && tmp->isChecked == false) {
            tmpWeight = tmp->weight;
            selectedElement = tmp;
        }
        tmp = tmp->next;
    }
    return selectedElement;
}

void deleteDijkstraElements(DijkstraElement** dijkstraHead) {
    DijkstraElement* tmp = *dijkstraHead;
    while(tmp != NULL) {
        *dijkstraHead = (*dijkstraHead)->next;
        delete tmp;
        tmp = *dijkstraHead;
    }
}

void printCityName(char* name) {
    int i = 0;
    printf(" ");
    while (name[i] != '\0') {
        printf("%c", name[i]);
        i++;
    }
}

void dijkstra(AdListEl *graphHead, List* nodesHead, List* cityA, List* cityB, bool addRoute) {
    if (cityA == cityB) {
        printf("0\n");
        return;
    }
    
    int totalTime = 0;
    DijkstraElement* dijkstraHead = initDijkstraElement();
    dijkstraHead->node = cityA;
    dijkstraHead->weight = 0;
    DijkstraElement *tmpDijkstra = dijkstraHead;
    
    List* tmpNode = nodesHead;
    while(tmpNode != NULL) {
        if (tmpNode != cityA) {
            tmpDijkstra->next = initDijkstraElement();
            tmpDijkstra->next->node = tmpNode;
            tmpDijkstra = tmpDijkstra->next;
        }
        tmpNode = tmpNode->next;
    }
    
    while (areNodesLeft(dijkstraHead)) {
        DijkstraElement* currentEl = getSmallestElement(dijkstraHead);
        currentEl->isChecked = true;
        
        AdListEl* tmpAdList = graphHead;
        while(tmpAdList != NULL) {
            if (tmpAdList->nodeA == currentEl->node) {
                tmpDijkstra = dijkstraHead;
                while (tmpDijkstra != NULL) {
                    if (tmpDijkstra->node == tmpAdList->nodeB && tmpDijkstra->weight > currentEl->weight + tmpAdList->weight) {
                        tmpDijkstra->weight = currentEl->weight + tmpAdList->weight;
                        tmpDijkstra->prevNode = currentEl->node;
                        break;
                    }
                    tmpDijkstra = tmpDijkstra->next;
                }
            }
            tmpAdList = tmpAdList->next;
        }
    }
    
    tmpDijkstra = dijkstraHead;
    while (tmpDijkstra != NULL) {
        if (tmpDijkstra->node == cityB) {
            totalTime = tmpDijkstra->weight;
            break;
        }
        tmpDijkstra = tmpDijkstra->next;
    }
    
    printf("%d", totalTime);
    
    if (addRoute) {
        List* lastNode = cityA;
        while (lastNode != tmpDijkstra->prevNode) {
            DijkstraElement* routeFindingEl = tmpDijkstra;
            DijkstraElement* tmpHead = dijkstraHead;
            while (routeFindingEl->prevNode != lastNode) {
                if (tmpHead->node == routeFindingEl->prevNode) {
                    routeFindingEl = tmpHead;
                    tmpHead = dijkstraHead;
                    continue;
                }
                tmpHead = tmpHead->next;
            }
            if (routeFindingEl->node->value[0] != '#')
                printCityName(routeFindingEl->node->value);
            lastNode = routeFindingEl->node;
        }
    }
    
    printf("\n");
    deleteDijkstraElements(&dijkstraHead);
}

void getQueries(AdListEl* graphHead, List* nodesHead) {
    char ch = '\n', tmpName[TEXT];
    memset(tmpName, '\0', TEXT);
    int index = 0;
    AdListEl* tmp = graphHead;
    List *cityA, *cityB;
    
    while (tmp->next != NULL)
        tmp = tmp->next;
    
    tmp->next = initAdListEl();
    
    while (ch == '\n')
        ch = getchar();
    
    while (ch != ' ') {
        tmpName[index] = ch;
        ch = getchar();
        index++;
    }
    
    cityA = findCity(nodesHead, tmpName);
    index = 0;
    memset(tmpName, '\0', TEXT);
    
    ch = getchar();
    while (ch != ' ') {
        tmpName[index] = ch;
        ch = getchar();
        index++;
    }
    
    cityB = findCity(nodesHead, tmpName);
    index = 0;
    memset(tmpName, '\0', TEXT);
    
    ch = getchar();
    
    if (ch == '0') {
        dijkstra(graphHead, nodesHead, cityA, cityB, false);
    }
    else
        dijkstra(graphHead, nodesHead, cityA, cityB, true);
}

void deleteNodes(List** nodesHead) {
    List* tmp = *nodesHead;
    while(tmp != NULL) {
        *nodesHead = (*nodesHead)->next;
        delete tmp;
        tmp = *nodesHead;
    }
}

void deleteGraph(AdListEl** graphHead) {
    AdListEl* tmp = *graphHead;
    while(tmp != NULL) {
        *graphHead = (*graphHead)->next;
        delete tmp;
        tmp = *graphHead;
    }
}
