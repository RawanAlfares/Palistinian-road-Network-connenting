#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>
#define MAX_CITIES 200


double pCost=0;
double kCost=0;
double PTime=0;
double KTime=0;

//define the structure for neighbors to the city
typedef struct NeighborCity* Neighbor;
struct NeighborCity {
    double distance;
    Neighbor nextCity;
    int cityIndex;
};
//define the structure of the city.
typedef struct city* City;
struct city {
    char name[20];
    Neighbor headOfCities;
    bool visited;
};

typedef struct MinheapNode *MinHeapNode;
 struct MinheapNode{
    int cityIndex;
    double key;
};
typedef struct minheap *MinHeap;
struct minheap{
    MinHeapNode *array;
    int *pos;
    int capacity;
    int size;
};


//function that balance the heap sift (sift down)
void heapify(MinHeap heap , int idx) {
    int smallest=idx;
    int left=2*idx+1; //left child
    int right=2*idx+2;  //right child

    if (left<heap->size && heap->array[left]->key < heap->array[smallest]->key) {
        smallest=left;
    }
    if (right<heap->size && heap->array[right]->key < heap->array[smallest]->key) {
        smallest=right;
    }
    if (smallest!=idx) {
        MinHeapNode smallestNode=heap->array[smallest];
        MinHeapNode idxNode=heap->array[idx];

        //swap between the two positions
        heap->pos[smallestNode->cityIndex] = idx;
        heap->pos[idxNode->cityIndex] = smallest;

        MinHeapNode temp=heap->array[smallest];
        heap->array[smallest]=heap->array[idx];
        heap->array[idx]=temp;

        heapify(heap,smallest);
    }
}
//function to extract the minimum heap
MinHeapNode extractMin(MinHeap heap) {
    MinHeapNode deleteItem;
    //check if the heap is empty or not
    if (heap->size==0) {
        printf("\nHeap is empty\n");
        return NULL;
    }
    //store the node in deleteItem that is to be deleted
    deleteItem=heap->array[0];

    //Replace the deleted node with the last node
    heap->array[0]=heap->array[heap->size-1];
    heap->size--;
    heapify(heap,0);
    return deleteItem;
}
//function that creat the array of the heap
MinHeap createMinHeap(int Capacity , double key[]) {

    MinHeap minHeap = (MinHeap) malloc(sizeof(struct minheap));
    if (minHeap == NULL) {
        printf("Memory allocation failed for heap\n");
        return NULL;
    }

    minHeap->size = Capacity;
    minHeap->capacity = Capacity;

    minHeap->array = (MinHeapNode *)malloc(Capacity * sizeof(MinHeapNode));
    if (minHeap->array == NULL) {
        printf("Memory allocation failed for array\n");
        return NULL;
    }

    minHeap->pos = (int *)malloc(Capacity * sizeof(int));
    if (minHeap->pos == NULL) {
        printf("Memory allocation failed for pos\n");
        return NULL;
    }

    for (int i = 0; i < Capacity; i++) {
        minHeap->array[i] = (MinHeapNode)malloc(sizeof(struct MinheapNode));
        if (minHeap->array[i] == NULL) {
            printf("Memory allocation failed for node %d\n", i);
            return NULL;
        }

        minHeap->array[i]->cityIndex = i;
        minHeap->array[i]->key = key[i];
        minHeap->pos[i] = i;
    }

    for (int i = (Capacity - 1) / 2; i >= 0; i--) {
        heapify(minHeap, i);
    }

    return minHeap;
}

//function that checks if the heap is empty
int isEmpty(MinHeap heap) {
    return heap->size == 0;
}
//function that balance the heap sift (sift up)
void decreaseKey(MinHeap heap, int cityIndex,double newKey) {
    int i=heap->pos[cityIndex];
    heap->array[i]->key=newKey;
    while (i&& heap->array[i]->key<heap->array[(i-1)/2]->key) {
        //swap current node with its parent
        MinHeapNode currentNode=heap->array[i];
        MinHeapNode parentNode=heap->array[(i-1)/2];

        heap->array[i]=parentNode;
        heap->array[(i-1)/2]=currentNode;

        heap->pos[currentNode->cityIndex]=(i-1)/2;
        heap->pos[parentNode->cityIndex]=i;

        i=(i-1)/2;

    }
}



int citiesIndecies=0;
City cities[MAX_CITIES];

//function that search for a city if its exist return its index . if not it add it to the array of cities
int searchCity(char cityName []){
    for (int i=0;i<citiesIndecies;i++) {
        if (strcmp(cities[i]->name,cityName)==0) {
            return i;
        }
    }
    City newCity = (City)malloc(sizeof(struct city));
    if (newCity==NULL) {
        printf("Memory allocation error\n");
        return -1;
    }
    strcpy(newCity->name,cityName);
    newCity->headOfCities=NULL;
    newCity->visited=false;
    cities[citiesIndecies]=newCity;
    return citiesIndecies++;;
}
//function that connect between two verticies (cities)
void addEdges(int Indexcity1,int Indexcity2, int dis) {
    Neighbor newNeighbor1=(Neighbor)malloc(sizeof(struct NeighborCity));
    if (newNeighbor1==NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNeighbor1->distance=dis;
    newNeighbor1->nextCity=cities[Indexcity1]->headOfCities;
    cities[Indexcity1]->headOfCities=newNeighbor1;
    newNeighbor1->cityIndex=Indexcity2;

    Neighbor newNeighbor2=(Neighbor)malloc(sizeof(struct NeighborCity));
    if (newNeighbor2==NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNeighbor2->distance=dis;
    newNeighbor2->nextCity=cities[Indexcity2]->headOfCities;
    cities[Indexcity2]->headOfCities=newNeighbor2;
    newNeighbor2->cityIndex=Indexcity1;
}
// graph representation
void printGraph() {
    for (int i=0;i<citiesIndecies;i++) {
        printf("%s --> ",cities[i]->name);
        Neighbor ptr=cities[i]->headOfCities;
        while (ptr!=NULL) {
            printf("%s (%.1f km)   ",cities[ptr->cityIndex]->name,ptr->distance);
            ptr=ptr->nextCity;
        }
        printf("\n\n");
    }
}
void loadCities() {
    FILE *inputFile=fopen("cities.txt","r");
    if (inputFile==NULL) {
        printf("file is not existed\n");
        return;
    }
    char line[200];
    char cityName1[20];
    char cityName2[20];
    float distance;
    while (fgets(line,sizeof(line),inputFile)) {
        //extract city1 (source)
        char* token = strtok(line, "#");
        if (token == NULL)
            continue;
        strcpy(cityName1,token);

        //extract city2 (destination)
        token=strtok(NULL,"#");
        if (token == NULL)
            continue;
         strcpy(cityName2,token);

        //extract distance
        token=strtok(NULL," ");
        if (token == NULL)
            continue;
        distance=atof(token);

        int cityIndex1=searchCity(cityName1);
        int cityIndex2=searchCity(cityName2);
        addEdges(cityIndex1,cityIndex2,distance);

    }
    printGraph();
    printf("%d cities have been loaded \n\n",citiesIndecies);
    fclose(inputFile);
}
 MinHeapNode newMinHeapNode(int cityIndex, int key)
{
     MinHeapNode minHeapNode= (struct MinHeapNode*)malloc(sizeof(struct MinheapNode));
    minHeapNode->cityIndex= cityIndex;
    minHeapNode->key = key;
    return minHeapNode;
}

void freeMinHeap(MinHeap heap) {
    for (int i=0;i<heap->capacity;i++) {
        if (heap->array[i]!=NULL) {
            free(heap->array[i]);
        }
    }

    free(heap->array);
    free(heap->pos);
    free(heap);

}
void primsAlgorithm() {
    int source;
    for (int i=0;i<citiesIndecies;i++) {
        printf("%d. %s\n",i+1,cities[i]->name);
    }
    printf("\n");
    printf("Enter the source city: using its ID\n");
    scanf("%d",&source);
    if (source <1 || source >citiesIndecies) {
        printf("Invalid source city\n");
        return;
    }
    source--;
    double key[MAX_CITIES];
    int parent[MAX_CITIES];
    bool inHeap[MAX_CITIES];
    for (int i=0;i<citiesIndecies;++i) {
        parent[i]=-1;
        key[i]=1e9;
        inHeap[i]=true;
    }
    key[source]=0;
    MinHeap min_heap=createMinHeap(citiesIndecies,key);
    while (!isEmpty(min_heap)) {
        MinHeapNode minNode=extractMin(min_heap);
        int u=minNode->cityIndex;
        inHeap[u]=false;

        Neighbor nbr=cities[u]->headOfCities;
        while (nbr!=NULL) {
            int v=nbr->cityIndex;
            double dis=nbr->distance;

            if (inHeap[v] && dis<key[v]) {
                key[v]=dis;
                parent[v]=u;
                decreaseKey(min_heap,v,key[v]);
            }
            nbr=nbr->nextCity;
        }
    }

    double totalCost=0;

    for (int i=0;i<citiesIndecies;i++) {
        if (parent[i]!=-1) {

            totalCost+=key[i];
        }
    }
    pCost=totalCost;
    printf("Minimum cost is: %.1f\n",pCost);
    freeMinHeap(min_heap);
}

typedef struct {
    int src;
    int dest;
    double weight;
}Edge;
int CompareEdges(const int a,const int b) {
    Edge* EdgeA=(Edge*) a;
    Edge* EdgeB=(Edge*) b;
    if (EdgeA->weight < EdgeB->weight) return -1;
    else if (EdgeA->weight > EdgeB->weight) return 1;
    else return 0;
}
int findParent(int parent[],int component) {
    if (parent[component]==component) {
        return component;
    }
    return parent[component]=findParent(parent,parent[component]);
}
void unionSet(int u,int v,int parent[], int rank[]) {
    u=findParent(parent,u);
    v=findParent(parent,v);

    if (rank[u]<rank[v]) {
        parent[u]=v;
    }else if (rank[u]>rank[v]) {
        parent[v]=u;
    }else {
        parent[v]=u;
        rank[u]++;
    }
}
void kruskalsAlgorithm() {
    Edge edge[1000];
    //collecting edges and store them in an array
    int edgeIndex=0;
    for (int i=0;i<citiesIndecies;i++) {
        Neighbor nbr=cities[i]->headOfCities;
        while (nbr!=NULL) {
            if (i<nbr->cityIndex) {
                edge[edgeIndex].src=i;
                edge[edgeIndex].dest=nbr->cityIndex;
                edge[edgeIndex].weight=nbr->distance;
                edgeIndex++;
            }
            nbr=nbr->nextCity;
        }
    }
    qsort(edge,edgeIndex,sizeof(edge[0]),CompareEdges);
    int parent[MAX_CITIES];
    int rank[MAX_CITIES];
    for (int i=0;i<citiesIndecies;i++) {
        parent[i]=i;
        rank[i]=0;
    }
    double totalCost=0;
    int edgeInMST=0;
    for (int i=0;i<edgeIndex && edgeInMST<citiesIndecies-1;i++) {
        int u=edge[i].src;
        int v=edge[i].dest;

        if (findParent(parent,u)!=findParent(parent,v)) {
            totalCost+=edge[i].weight;
            unionSet(u,v,parent,rank);
            edgeInMST++;
        }
    }
    kCost=totalCost;
    printf("Minimum cost is: %.1f\n",kCost);
}
void compareTheTwoAlgorithms() {
    printf("Compare the two algorithms:\n");
    printf("Prim's Algorithm: Total Cost: %.5f , Execution Time: %.5f",pCost,PTime);
    printf("Kruskal's Algorithm: Total Cost: %.5f , Execution Time: %.5f",kCost,KTime);
}

void exitProgramm() {
    //free neighbor lists for each city
for (int i=0;i<citiesIndecies;i++) {
    Neighbor nbr=cities[i]->headOfCities;
    while (nbr!=NULL) {
        cities[i]->headOfCities=nbr->nextCity;
        free(nbr);
        nbr=cities[i]->headOfCities;
    }
}
    citiesIndecies=0;
    //free cities
    for (int i=0;i<MAX_CITIES;i++) {
        free(cities[i]);
    }

}
int main(void) {
    int choice;
    do {
        printf("1.Load cities:\n");
        printf("2.Apply Prim's Algorithm:\n");
        printf("3.Apply Kruskal's Algorithm:\n");
        printf("4.Compare the two algorithms:\n");
        printf("5.Exit:\n");
        printf("********************************************");
        printf("\nEnter your choice:\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:loadCities();
                break;
            case 2: {
                clock_t startTime = clock();
                primsAlgorithm();
                clock_t endTime = clock();
                double time_taken = (double)(endTime - startTime) / CLOCKS_PER_SEC;
                KTime=time_taken;
                printf("prims function took %.6f seocnds\n", time_taken);
                break;
            }
            case 3: {
                clock_t startTime = clock();
                 kruskalsAlgorithm();
                clock_t endTime = clock();
                double time_taken = (double)(endTime - startTime) / CLOCKS_PER_SEC;
                KTime=time_taken;
                printf("kruskals function took %.6f seocnds\n", time_taken);
                break;
            }
            case 4: compareTheTwoAlgorithms();
                break;
            case 5:exitProgramm();
                 break;
            default: printf("You have entered an invalid choice\n");
                break;
        }
    }while (choice!=5);




    return 0;
}