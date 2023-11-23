#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<omp.h>

// the functions from coordReader.c
int readNumOfCoords(char *fileName);
double **readCoords(char *filename, int numOfCoords);
void *writeTourToFile(int *tour, int tourLength, char *filename);
void print_coordinates(double **coords,int num);


// Function to calculate Euclidean distance between two points
double get_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


// Function to get Euclidean distance matrix bewteen any two vertexes
void get_distance_matrix_omp(double **coords, int num,double **distance_matrix){
  #pragma omp parallel for
  for(int i = 0; i < num; i++){
    for(int j = i; j < num; j++){
      if (i == j){
        distance_matrix[i][j] = 0;
      }else{
        double x1 = **(coords+i);
        double y1 = *(*(coords+i)+1);
        double x2 = **(coords+j);
        double y2 = *(*(coords+j)+1);
        double distance = get_distance(x1,y1,x2,y2);
        distance_matrix[i][j] = distance;
        distance_matrix[j][i] = distance;
      }
        
    }
  }

}


void free_2DArray(double **arr, int rows){
  // free memory
  for (int i = 0; i < rows; i++) {
      free(arr[i]);
  }
  free(arr);
}

// Function to find the farthest unvisited vertex
int findFarthestVertex(double **distance_matrix, bool *visited, int numOfCoords, int *tour, int tourLength) {
  double maxDistance = -1.0;
  int farthestVertex = -1;

  #pragma omp parallel
  {
    double localMaxDistance = -1.0;
    int localFarthestVertex = -1;

    #pragma omp for nowait
    for (int i = 0; i < numOfCoords; i++) {
      if (!visited[i]) {
        for (int j = 0; j < tourLength; j++) {
          if (distance_matrix[tour[j]][i] > localMaxDistance) {
            localMaxDistance = distance_matrix[tour[j]][i];
            localFarthestVertex = i;
          }
        }
      }
    }

    #pragma omp critical
    {
      if (localMaxDistance > maxDistance) {
        maxDistance = localMaxDistance;
        farthestVertex = localFarthestVertex;
      }
    }
  }
  return farthestVertex;
}

// Function to insert a vertex in the tour at the best position
void insertAtBestPosition(int *tour, int tourLength, int vertex, double **distance_matrix) {
  double minIncrease = INFINITY;
  int bestPosition = -1;

  #pragma omp parallel
  {
    double localMinIncrease = INFINITY;
    int localBestPosition = -1;
    #pragma omp for nowait
    for (int i = 0; i < tourLength - 1; i++) {
      double increase = distance_matrix[tour[i]][vertex] + distance_matrix[vertex][tour[i + 1]] - distance_matrix[tour[i]][tour[i + 1]];
      if (increase < localMinIncrease) {
        localMinIncrease = increase;
        localBestPosition = i;
      }
    }

    #pragma omp critical
    {
      if (localMinIncrease < minIncrease) {
        minIncrease = localMinIncrease;
        bestPosition = localBestPosition;
      }
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
  start = omp_get_wtime();
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

  get_distance_matrix_omp(coords, numOfCoords, distance_matrix);

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
  
  end = omp_get_wtime();
  times = (double)(end-start);
  printf("The total cost of this code is: %f s\n",times);
  
  return 0;
}
