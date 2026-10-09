#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VEHICLES 100
#define V 6 // Number of intersections in the city


// --- 1. PRIORITY QUEUE (MIN-HEAP) FOR VEHICLE MANAGEMENT ---


typedef struct {
    char id[20];
    int type; // 1: Emergency, 2: Public Transport, 3: Private Car
    int urgency_score; // Lower number = higher priority
} Vehicle;



Vehicle heap[MAX_VEHICLES];
int heapSize = 0;

void swap(Vehicle *a, Vehicle *b) {
    Vehicle temp = *a;
    *a = *b;
    *b = temp;
}

void insertVehicle(char id[], int type) {
    if (heapSize == MAX_VEHICLES) {
        printf("Traffic lane full!\n");
        return;
    }
    
    Vehicle newVehicle;
    strcpy(newVehicle.id, id);
    newVehicle.type = type;
    newVehicle.urgency_score = type; // Using type directly as score for simplicity

    heapSize++;
    int i = heapSize - 1;
    heap[i] = newVehicle;

    // Min-Heapify up
    while (i != 0 && heap[(i - 1) / 2].urgency_score > heap[i].urgency_score) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void minHeapify(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int smallest = i;

    if (left < heapSize && heap[left].urgency_score < heap[smallest].urgency_score)
        smallest = left;
    if (right < heapSize && heap[right].urgency_score < heap[smallest].urgency_score)
        smallest = right;

    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        minHeapify(smallest);
    }
}

Vehicle extractHighestPriority() {
    if (heapSize <= 0) {
        Vehicle empty = {"", 0, 0};
        return empty;
    }
    if (heapSize == 1) {
        heapSize--;
        return heap[0];
    }

    Vehicle root = heap[0];
    heap[0] = heap[heapSize - 1];
    heapSize--;
    minHeapify(0);

    return root;
}

// --- 2. GRAPH & DIJKSTRA'S ALGORITHM FOR EMERGENCY ROUTING ---

int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == 0 && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void printPath(int parent[], int j) {
    if (parent[j] == -1) return;
    printPath(parent, parent[j]);
    printf(" -> Intersection %d", j);
}

void dijkstra(int graph[V][V], int src, int dest) {
    int dist[V]; 
    int sptSet[V]; 
    int parent[V]; 

    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = 1;

        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
    }

    printf("\n--- Emergency Route Calculation ---\n");
    printf("Fastest route from Intersection %d to %d:\n", src, dest);
    printf("Intersection %d", src);
    printPath(parent, dest);
    printf("\nTotal Time/Distance: %d units\n", dist[dest]);
}

// --- MAIN FUNCTION TO DEMONSTRATE ---
int main() {
    printf("--- Processing Traffic at Intersection A ---\n");
    insertVehicle("Car-123", 3);
    insertVehicle("Bus-45", 2);
    insertVehicle("Ambulance-911", 1);
    insertVehicle("Car-999", 3);

    printf("Releasing vehicles based on priority (Min-Heap):\n");
    while (heapSize > 0) {
        Vehicle v = extractHighestPriority();
        char typeStr[20];
        if (v.type == 1) strcpy(typeStr, "Emergency");
        else if (v.type == 2) strcpy(typeStr, "Public Transport");
        else strcpy(typeStr, "Private Car");
        
        printf("Clearing %s (%s) - Urgency: %d\n", v.id, typeStr, v.urgency_score);
    }

    // City Graph Representation (Adjacency Matrix)
    // 0 means no direct road. Numbers represent distance/traffic time.
    int cityGraph[V][V] = {
        {0, 4, 0, 0, 0, 0},
        {4, 0, 8, 0, 0, 0},
        {0, 8, 0, 7, 0, 4},
        {0, 0, 7, 0, 9, 14},
        {0, 0, 0, 9, 0, 10},
        {0, 0, 4, 14, 10, 0}
    };

    // Calculate route for Ambulance from Intersection 0 to Intersection 4
    dijkstra(cityGraph, 0, 4);

    return 0;
}