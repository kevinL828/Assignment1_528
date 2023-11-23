#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>
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



// print 2D array
void print2DArray(double **array, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", array[i][j]);
        }
        printf("\n");
    }
}


void malloc_2DArray(double **array,int numOfElements){

  array = (double **) malloc ( numOfElements * sizeof ( double * ) ) ; 
  for ( int i = 0; i < numOfElements ; i++) { 
    array [ i ] = ( double *) malloc ( numOfElements * sizeof ( double ) ) ; 
  }
}


void free_2DArray(double **arr, int rows){
  // free memory
  for (int i = 0; i < rows; i++) {
      free(arr[i]);
  }
  free(arr);
}

// Function to find cheaptest insertion
int findCheapestInsertion(int *tour, int tourLength, double **distance_matrix, bool *visited, int numOfCoords) {
    double minIncrease = INFINITY;
    int minPosition = -1;
    int minVertex = -1;

    #pragma omp parallel
    {
        double localMinIncrease = INFINITY;
        int localMinPosition = -1;
        int localMinVertex = -1;

        #pragma omp for nowait
        for (int i = 0; i < numOfCoords; i++) {
            if (!visited[i]) {
                for (int j = 0; j < tourLength - 1; j++) {
                    double increase = distance_matrix[tour[j]][i] + distance_matrix[i][tour[j + 1]] - distance_matrix[tour[j]][tour[j + 1]];
                    if (increase < localMinIncrease) {
                        localMinIncrease = increase;
                        localMinPosition = j;
                        localMinVertex = i;
                    }
                }
            }
        }

        #pragma omp critical
        {
            if (localMinIncrease < minIncrease) {
                minIncrease = localMinIncrease;
                minPosition = localMinPosition;
                minVertex = localMinVertex;
            }
        }
    }

    // Insert vertex
    if (minPosition != -1) {
        for (int i = tourLength; i > minPosition + 1; i--) {
            tour[i] = tour[i - 1];
        }
        tour[minPosition + 1] = minVertex;
    }

    return minVertex;
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

  // Initialize the distance matrix
  double **distance_matrix = (double **) malloc ( numOfCoords * sizeof ( double * ) ) ; 
  for ( int i = 0; i < numOfCoords ; i++) { 
    distance_matrix [ i ] = ( double *) malloc ( numOfCoords * sizeof ( double ) ) ; 
  }

  get_distance_matrix_omp(coords, numOfCoords, distance_matrix);

  // initialize the visited array
  bool *visited = (bool *)calloc(numOfCoords, sizeof(bool));
  int *tour = (int *)malloc((numOfCoords+1) * sizeof(int));
  int tourLength;

  // initialize original vertex
  tour[0] = 0;
  tour[1] = 0;
  visited[0] = true;
  tourLength = 2;

  // Excute Cheapest Insertion
  while (tourLength < numOfCoords+1) {
      int nextVertex = findCheapestInsertion(tour, tourLength, distance_matrix, visited, numOfCoords);
      if (nextVertex != -1) {
          visited[nextVertex] = true;
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
