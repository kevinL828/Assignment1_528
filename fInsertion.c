#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdbool.h>

// Include the functions from your existing code here
/*Gets the number of the coordinates in the file. Returns as a single integer*/
int readNumOfCoords(char *filename){
	FILE *file = fopen(filename, "r");
	int numOfCoords = 0;

	if(file == NULL){
		return -1;
	}

	char line[100];

	while(fgets(line, sizeof(line), file) != NULL){
		numOfCoords++;
	}
	
    return numOfCoords;
}

/*Gets the data from the file. Returns as an array of doubles, Ignores the first integer*/
double **readCoords(char *filename, int numOfCoords){
	FILE *file = fopen(filename,"r");
  int i;

	char line[100];
    
  if(file == NULL) {
      printf("Unable to open file: %s", filename);
      return NULL;
  }

	double **coords = (double **)malloc(numOfCoords * sizeof(double *));

	for(i = 0; i < numOfCoords; i++){
		coords[i] = (double *) malloc(2 * sizeof(int));
		if (coords[i] == NULL){
			perror("Memory Allocation Failed");
		}
	}

	int lineNum = 0;
	while(fgets(line, sizeof(line), file) != NULL){
		double x, y;
		if (sscanf(line, "%lf,%lf", &x, &y) == 2){
			coords[lineNum][0] = x;
			coords[lineNum][1] = y;
			lineNum++;
		}
	}

	return coords;
}

void *writeTourToFile(int *tour, int tourLength, char *filename){
	
	FILE *file = fopen(filename, "w");
	int i;	
	
	if(file == NULL){
		printf("Unable to open file: %s", filename);
		return NULL;
	}

	fprintf(file, "%d \n", tourLength);

	printf("Writing output data\n");
    for(i=0; i < tourLength; i++) {
        fprintf(file, "%d ", tour[i]);
    }

	fclose(file);
  return NULL;

}

// Function to calculate Euclidean distance between two points
double get_distance(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Function to get Euclidean distance matrix bewteen any two vertexes
void get_distance_matrix(double **coords, int num,double **distance_matrix){
  for(int i = 0; i < num; i++){
    for(int j = i; j < num; j++){
      if (i == j){
        distance_matrix[i][j] = 0;
      }else{
        int x1 = **(coords+i);
        int y1 = *(*(coords+i)+1);
        int x2 = **(coords+j);
        int y2 = *(*(coords+j)+1);
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

// Function to find cheapest insertion
int findFartestInsertion(int *tour, int tourLength, double **distance_matrix, bool *visited, int 
numOfCoords) {
    double minIncrease = INFINITY;
    int minPosition = -1;
    int minVertex = -1;
    int maxPosition = -1;
    int maxDistance = 0;

    //find the fartest position from the tour array position
    for (int i = 0; i < tourLength; i++){
      for(int j = 0; j < numOfCoords; j++){
        double distance = distance_matrix[tour[i]][j];
        if (maxDistance < distance){
          maxDistance = distance;
          maxPosition[i] = j;
        }
      }
    }

    for (int i = 0; i < numOfCoords; i++) {
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

    // 插入顶点
    if (minPosition != -1) {
        for (int i = tourLength; i > minPosition + 1; i--) {
            tour[i] = tour[i - 1];
        }
        tour[minPosition + 1] = minVertex;
    }

    return minVertex;
}



//-----------------------------------------------------------------


int main() {
  char *filename = "9_coords.coord";
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
  tour[1] = 0;
  visited[0] = true;
  tourLength = 2;

  // Excute Cheapest Insertion
  while (tourLength < numOfCoords+1) {
      int nextVertex = findFartestInsertion(tour, tourLength, distance_matrix, visited, numOfCoords);
      if (nextVertex != -1) {
          visited[nextVertex] = true;
          tourLength++;
      } else {
          break;
      }
  }

  // print tour
  printf("%d\n",tourLength);
  for (int i = 0; i < tourLength; i++) {
      printf("%d ", tour[i]);
  }
  printf("\n");

  // free memory
  free(tour);
  free(visited);
  //free memory
  free_2DArray(distance_matrix, numOfCoords);

  return 0;
}
