#include <stdio.h>
#include <string.h>

#define MAX 20
#define NAME_LEN 30
#define INF 999999

int  n = 0;                         
int  graph[MAX][MAX];              
char names[MAX][NAME_LEN];          
int  dist[MAX];                     
int  parent[MAX];                  
int  visited[MAX];
int  source = -1;
int  graphEntered = 0;
int  computed = 0;

void clearInput(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

int readInt(const char *prompt, int lo, int hi) {
    int x;
    while(1){
        printf("%s", prompt);
        if(scanf("%d", &x) == 1 && x >= lo && x <= hi) {
            clearInput();
            return x;
        }
        clearInput();
        printf("Invalid input. Enter a number between %d and %d.\n", lo, hi);
    }
}

void enterGraph(void) {
    int i, j, w, edges, u, v;
    n = readInt("Enter number of locations (2-20): ", 2, MAX);
    printf("\nEnter location names (one per line, use '_' instead of spaces if you like):\n");
    for (i = 0; i < n; i++) {
        printf("  Location %d: ", i + 1);
        scanf(" %29[^\n]", names[i]);
        clearInput();
    }

      for(i = 0; i < n; i++)
        for(j = 0; j < n; j++)
            graph[i][j] = (i == j) ? 0 : INF;
    printf("\nLocations:\n");
    for(i = 0; i < n; i++)
        printf("  %d. %s\n", i + 1, names[i]);

    edges = readInt("\nEnter number of roads (connections): ", 0, n * (n - 1) / 2);

    printf("Enter each road as: <from no.> <to no.> <distance>  (roads are two-way)\n");
    for(i = 0; i < edges; ) {
        printf("  Road %d: ", i + 1);
        if (scanf("%d %d %d", &u, &v, &w) != 3) {
            clearInput();
            printf("  Invalid input. Try again.\n");
            continue;
        }
        clearInput();
        if (u < 1 || u > n || v < 1 || v > n || u == v) {
            printf("  Invalid location numbers. Try again.\n");
            continue;
        }
        if (w < 0) {
            printf("  Distance must be non-negative. Try again.\n");
            continue;
        }
        graph[u - 1][v - 1] = w;
        graph[v - 1][u - 1] = w;
        i++;
    }

    graphEntered = 1;
    computed = 0;
    source = -1;
    printf("\nCampus graph stored successfully.\n");
}

void displayMatrix(void) {
    int i, j;
    if (!graphEntered) { printf("Please enter the campus graph first (option 1).\n"); return;
	}

    printf("\nAdjacency Matrix (INF = no direct road):\n\n");
    for (j = 0; j < n; j++) printf("%6d", j + 1);
    printf("\n      ");
    for (j = 0; j < n; j++) printf("------");
    printf("\n");
    for (i = 0; i < n; i++) {
        printf("%4d |", i + 1);
        for (j = 0; j < n; j++) {
            if (graph[i][j] == INF) 
			printf("%6s", "INF");
            else 
			printf("%6d", graph[i][j]);
        }
        printf("%s\n", names[i]);
    }
}

void selectSource(void) {
    int i;
    if(!graphEntered) { 
	printf("Please enter the campus graph first (option 1).\n"); return;
	}
    printf("\nLocations:\n");
    for(i = 0; i < n; i++) 
	printf("%d. %s\n", i + 1, names[i]);
    source = readInt("Select source location number: ", 1, n) - 1;
    computed = 0;
    printf("Source selected: %s\n", names[source]);
}


void dijkstra(void) {
    int i, count, u, v, min;

    if(!graphEntered){ 
	printf("Please enter the campus graph first (option 1).\n"); return;
	}
    if(source < 0){
	printf("Please select a source location first (option 3).\n"); return; 
	}

    for(i = 0; i < n; i++) {
        dist[i]    = INF;
        parent[i]  = -1;
        visited[i] = 0;
    }
    dist[source] = 0;

    for(count = 0; count < n - 1; count++) {
        min = INF;
        u = -1;
        for(i = 0; i < n; i++)
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        if(u == -1) break;              
        visited[u] = 1;

            for(v = 0; v < n; v++) {
            if(!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    computed = 1;
    printf("Shortest distances computed from %s.\n", names[source]);
}

void printPath(int v) {
    if(parent[v] == -1) {
        printf("%s", names[v]);
        return;
    }
    printPath(parent[v]);
    printf(" -> %s", names[v]);
}

int checkReady(void) {
    if(!computed) {
        printf("Please run option 4 (Find Shortest Distance) first.\n");
        return 0;
    }
    return 1;
}

void displayPaths(void) {
    int i;
    if(!checkReady()) return;

    printf("\nSource: %s\n\n", names[source]);
    printf("%-22s %-10s %s\n", "Destination", "Distance", "Shortest Path");
    printf("--------------------------------------------------------------------\n");
    for(i = 0; i < n; i++) {
        if(i == source) continue;
        printf("%-22s ", names[i]);
        if(dist[i] == INF) {
            printf("%-10s %s\n", "INF", "No path available");
        } 
		else {
            printf("%-10d ", dist[i]);
            printPath(i);
            printf("\n");
        }
    }
}

void displayDistances(void) {
    int i;
    if(!checkReady()) return;

    printf("\nSource: %s\n\n", names[source]);
    printf("%-22s %s\n", "Destination", "Shortest Distance");
    printf("-------------------------------------------\n");
    for(i = 0; i < n; i++) {
        if(dist[i] == INF) 
		printf("%-22s %s\n", names[i], "INF (unreachable)");
        else                
		printf("%-22s %d\n", names[i], dist[i]);
    }
}

int main(void) {
    int choice;
    printf("=========================================\n");
    printf("  Campus Shortest Route Finder (Dijkstra)\n");
    printf("=========================================\n");

    do {
        printf("\n------------- MENU -------------\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths\n");
        printf("6. Display Distance from Source to All Locations\n");
        printf("7. Exit\n");
        choice = readInt("Enter your choice: ", 1, 7);

        switch (choice) {
            case 1: enterGraph();       break;
            case 2: displayMatrix();    break;
            case 3: selectSource();     break;
            case 4: dijkstra();         break;
            case 5: displayPaths();     break;
            case 6: displayDistances(); break;
            case 7: printf("Exiting program. Goodbye!\n"); break;
        }
    } while (choice != 7);

    return 0;
}
