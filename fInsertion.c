#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>


// the functions from coordReader.c
int readNumOfCoords(char *fileName);
double **readCoords(char *filename, int numOfCoords);
void *writeTourToFile(int *tour, int tourLength, char *filename);
void print_coordinates(double **coords,int num);
double get_distance(double x1, double y1, double x2, double y2);
void get_distance_matrix(double **coords, int num,double **distance_matrix);


void free_2DArray(double **arr, int rows){
  // free memory
  for (int i = 0; i < rows; i++) {
      free(arr[i]);
  }
  free(arr);
}

// memorry allocate
void malloc_2DArray(double **array,int numOfElements){

  array = (double **) malloc ( numOfElements * sizeof ( double * ) ) ; 
  for ( int i = 0; i < numOfElements ; i++) { 
    array [ i ] = ( double *) malloc ( numOfElements * sizeof ( double ) ) ; 
  }
}


// Function to find the farthest unvisited vertex
int findFarthestVertex(double **distance_matrix, bool *visited, int numOfCoords, int *tour, int tourLength) {
  double maxDistance = -1.0;
  int farthestVertex = -1;
  double distance_sum;

  for (int i = 0; i < numOfCoords; i++) {
    if (!visited[i]) {
      for (int j = 0; j < tourLength; j++) {
        if (distance_matrix[tour[j]][i] > maxDistance) {
            maxDistance = distance_matrix[tour[j]][i];
            farthestVertex = i;
        }
      }
    }
  }

  return farthestVertex;
}

// Function to insert a vertex in the tour at the best position
void insertAtBestPosition(int *tour, int tourLength, int vertex, double **distance_matrix) {
    double minIncrease = INFINITY;
    int bestPosition = -1;

    for (int i = 0; i < tourLength - 1; i++) {
        double increase = distance_matrix[tour[i]][vertex] + distance_matrix[vertex][tour[i + 1]] - distance_matrix[tour[i]][tour[i + 1]];
        if (increase < minIncrease) {
            minIncrease = increase;
            bestPosition = i;
        }
    }

    // Shift elements to make space for the new vertex
    for (int i = tourLength; i > bestPosition + 1; i--) {
        tour[i] = tour[i - 1];
    }
    tour[bestPosition + 1] = vertex;
}


//-----------------------------------------------------------------


int main(int argc, char *argv[]) {
  clock_t start, end;
  double times;
  start = clock();
  char *filename = argv[1];
  char *out_put_file_path = argv[2];
  int numOfCoords = readNumOfCoords(filename);
  double **coords = readCoords(filename, numOfCoords);

  // print2DArray(coords, numOfCoords,2);

  // Initialize the distance matrix
  double **distance_matrix = (double **) malloc ( numOfCoords * sizeof ( double * ) ) ; 
  for ( int i = 0; i < numOfCoords ; i++) { 
    distance_matrix [ i ] = ( double *) malloc ( numOfCoords * sizeof ( double ) ) ; 
  }

  get_distance_matrix(coords, numOfCoords, distance_matrix);

  // print2DArray(distance_matrix, numOfCoords, numOfCoords);

  // initialize the visited array
  bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
  int *tour = (int *)malloc((numOfCoords+1) * sizeof(int));
  int tourLength = numOfCoords;

  // initialize original vertex
  tour[0] = 0;
  tour[2] = 0;
  visited[0] = true;
  tourLength = 2;

  // Find the farthest vertex from the initial vertex
    int farthest = findFarthestVertex(distance_matrix, visited, numOfCoords, tour, tourLength);
    tour[1] = farthest;
    visited[farthest] = true;
    tourLength = 3;

    // Main loop of the Farthest Insertion algorithm
    while (tourLength < numOfCoords+1) {
        farthest = findFarthestVertex(distance_matrix, visited, numOfCoords, tour, tourLength);
        if (farthest != -1) {
            insertAtBestPosition(tour, tourLength, farthest, distance_matrix);
            visited[farthest] = true;
            tourLength++;
        } else {
            break;
        }
    }


  // char *myfileName = "./mycout/cout_4096_my"; 
  writeTourToFile(tour, tourLength, out_put_file_path);


  // free memory
  free(tour);
  free(visited);
  //free memory
  free_2DArray(distance_matrix, numOfCoords);
  
  end = clock();
  times = (double)(end-start)/CLOCKS_PER_SEC;
  printf("The total cost of this code is: %f s",times);
  
  return 0;
}
