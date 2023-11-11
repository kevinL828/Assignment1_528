#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>
#include"coordReader.h"

// Include the functions from your existing code here
// ...

// Function to calculate Euclidean distance between two points
double distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Function to find the farthest vertex from any vertex in the partial tour
int findFarthestVertex(double **coords, int numOfCoords, bool *visited) {
    double maxDist = -1.0;
    int farthestVertex = -1;

    for (int i = 0; i < numOfCoords; i++) {
        if (!visited[i]) {
            for (int j = 0; j < numOfCoords; j++) {
                if (visited[j]) {
                    double dist = distance(coords[i][0], coords[i][1], coords[j][0], coords[j][1]);
                    if (dist > maxDist) {
                        maxDist = dist;
                        farthestVertex = i;
                    }
                }
            }
        }
    }

    return farthestVertex;
}

// Function to insert the farthest vertex in the optimal position in the tour
void insertInTour(int *tour, int *tourLength, int vertex, double **coords) {
    // Find the best position to insert the vertex
    double minIncrease = INFINITY;
    int bestPosition = -1;

    for (int i = 0; i < *tourLength - 1; i++) {
        double currentIncrease = distance(coords[tour[i]][0], coords[tour[i]][1], coords[vertex][0], coords[vertex][1]) +
                                 distance(coords[vertex][0], coords[vertex][1], coords[tour[i + 1]][0], coords[tour[i + 1]][1]) -
                                 distance(coords[tour[i]][0], coords[tour[i]][1], coords[tour[i + 1]][0], coords[tour[i + 1]][1]);

        if (currentIncrease < minIncrease) {
            minIncrease = currentIncrease;
            bestPosition = i;
        }
    }

    // Insert the vertex at the best position
    for (int i = *tourLength; i > bestPosition + 1; i--) {
        tour[i] = tour[i - 1];
    }
    tour[bestPosition + 1] = vertex;
    (*tourLength)++;
}

int main() {
    char *filename = "16_coords.coord";
    int numOfCoords = readNumOfCoords(filename);
    double **coords = readCoords(filename, numOfCoords);

    // Initialize the tour with the first two vertices
    int *tour = (int *)malloc(numOfCoords * sizeof(int));
    bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
    tour[0] = 0; tour[1] = 1;
    visited[0] = visited[1] = true;
    int tourLength = 2;

    // Farthest Insertion Algorithm
    while (tourLength < numOfCoords) {
        int farthest = findFarthestVertex(coords, numOfCoords, visited);
        visited[farthest] = true;
        insertInTour(tour, &tourLength, farthest, coords);
    }

    // Write the tour to a file or print it
    // ...

    // Free allocated memory
    free(tour);
    free(visited);
    for (int i = 0; i < numOfCoords; i++) {
        free(coords[i]);
    }
    free(coords);

    return 0;
}
