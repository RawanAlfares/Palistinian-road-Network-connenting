#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<string.h>

#define MAX_CITIES 200

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
int citiesIndecies=0;
City cities[MAX_CITIES];

//function that search for a city if its exist return its index . if not it add it to the array of cities
int searchCity(char cityName){
    for (int i=0;i<citiesIndecies;i++) {
        if (strcmp(cities[i]->name,cityName)==0) {
            return i;
        }
    }
    strcmp(cities[citiesIndecies]->name,cityName);
    cities[citiesIndecies]->headOfCities=NULL;
    cities[citiesIndecies]->visited=false;
    return citiesIndecies++;
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
    newNeighbor1->cityIndex=Indexcity2;
    cities[Indexcity1]->headOfCities=newNeighbor1;

    Neighbor newNeighbor2=(Neighbor)malloc(sizeof(struct NeighborCity));
    if (newNeighbor2==NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNeighbor2->distance=dis;
    newNeighbor2->nextCity=cities[Indexcity1]->headOfCities;
    newNeighbor2->cityIndex=Indexcity2;
    cities[Indexcity1]->headOfCities=newNeighbor2;
}
// graph representation
void printGraph() {
    for (int i=0;i<citiesIndecies;i++) {
        printf("%s --> ",cities[i]->name);
        Neighbor ptr=cities[i]->headOfCities;
        while (ptr!=NULL) {
            printf("%s (%.1f km)",cities[ptr->cityIndex]->name,ptr->distance);
            ptr=ptr->nextCity;
        }
        printf("\n");
    }
}
void loadCities() {
    FILE *inputFile=fopen("cities.txt","r");
    if (inputFile==NULL) {
        printf("file is not existed\n");
        return;
    }
    char line[200];
    char city1[200];
    char city2[200];
    float distance;

    while (fgets(line,sizeof(line),inputFile)!=NULL) {
        //extract city1 (source)
        char* token = strtok(city1, "#");
        if (token == NULL)
            continue;
        strcpy(city1,token);
        int cityIndex1=searchCity(city1);

        //extract city2 (distination)
        token=strtok(NULL,"#");
        if (token == NULL)
            continue;
        strcpy(city2,token);
        int cityIndex2=searchCity(city2);

        //extract distance
        token=strtok(NULL,"#");
        if (token == NULL)
            continue;
        distance=atof(token);

        addEdges(cityIndex1,cityIndex2,distance);
    }
    printf("%d cities have been loaded \n",citiesIndecies);
    fclose(inputFile);
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
                    printGraph();
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                 break;
            default: printf("You have entered an invalid choice\n");
                break;
        }
    }while (choice!=5);




    return 0;
}