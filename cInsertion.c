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


// Function to find cheapest insertion
int findCheapestInsertion(int *tour, int tourLength, double **distance_matrix, bool *visited, int numOfCoords) {
  double minIncrease = INFINITY;
  int minPosition = -1;
  int minVertex = -1;

  for (int i = 0; i < numOfCoords; i++) {
    // if the vertex not in the tour, calculate the distance among the tour vertex
    if (!visited[i]) {
      for (int j = 0; j < tourLength-1; j++) {
        double increase = distance_matrix[tour[j]][i] + distance_matrix[i][tour[j + 1]] - distance_matrix[tour[j]][tour[j + 1]];
        if (increase < minIncrease) {
            minIncrease = increase;
            minPosition = j;
            minVertex = i;
        }
      }
    }
  }

  // insert vertex
  if (minPosition != -1) {
    for (int i = tourLength; i > minPosition + 1; i--) {
        tour[i] = tour[i - 1];
    }
    tour[minPosition + 1] = minVertex;
  }

  return minVertex;
}




int main(int argc, char *argv[]) {
  clock_t start, end;
  double times;
  start = clock();
  char *filename = argv[1];
  char *out_put_file_path = argv[2];
  int numOfCoords = readNumOfCoords(filename);
  double **coords = readCoords(filename, numOfCoords);


  // Initialize the distance matrix
  double **distance_matrix = (double **) malloc ( numOfCoords * sizeof ( double * ) ) ; 
  for ( int i = 0; i < numOfCoords ; i++) { 
    distance_matrix [ i ] = ( double *) malloc ( numOfCoords * sizeof ( int ) ) ; 
  }

  get_distance_matrix(coords, numOfCoords, distance_matrix);

  // initialize the visited array
  bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
  int *tour = (int *)malloc((numOfCoords+1) * sizeof(int));
  int tourLength = numOfCoords;

  // initialize original vertex
  tour[0] = 0;
  tour[1] = 0;
  visited[0] = true;
  tourLength = 2;

  // Excute Cheapest Insertion
  // change the new insert vertex's visited flag to true
  while (tourLength < numOfCoords+1) {
      int nextVertex = findCheapestInsertion(tour, tourLength, distance_matrix, visited, numOfCoords);
      if (nextVertex != -1) {
          visited[nextVertex] = true;
          tourLength++;
      } else {
          break;
      }
  }

  // write tour result to file
  writeTourToFile(tour, tourLength, out_put_file_path);

  // free memory
  free(tour);
  free(visited);
  //free memory
  for (int i = 0; i < numOfCoords; i++) {
      free(distance_matrix[i]);
  }
  free(distance_matrix);
  end = clock();
  times = (double)(end-start)/CLOCKS_PER_SEC;
  printf("The total cost of this code is: %f s\n",times);


  return 0;
}
