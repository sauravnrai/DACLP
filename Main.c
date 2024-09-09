#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <malloc.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>

#define MAX_VERTICES 2000 // Maximum number of vertices you expect
#define MAX_FILENAME_LENGTH 600
#define MAX_FOLDERPATH_LENGTH 600
#define MAX_FILEPATH_LENGTH (MAX_FILENAME_LENGTH + MAX_FOLDERPATH_LENGTH + 1) // +1 for the '/' separator

//ulimit -s unlimited

int HH_RAND[MAX_VERTICES], HH_GREEDY[MAX_VERTICES], HH_RAND_size, HH_GREEDY_size;

int Q_dynamic[MAX_VERTICES][MAX_VERTICES]; /* Index 0 will represent the size of the forbidden set and 
from index 1 to vertex_count will have bits i.e 0 represents a vertex absence and 1 represents a vertex presence 
wrt distance R*/


long HH_GREEDY_AVG, HH_RAND_AVG; // default value is 0



int count_vertices(const char *distance_matrix_file) {
    FILE *file = fopen(distance_matrix_file, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int vertex_count = 1;
    int value;
    int in_sequence = 0;
    int first_row = 1; // Assuming the first row is a header

    while (fscanf(file, "%d", &value) != EOF) {
        if (first_row) {
            first_row = 0; // Skip the first row (header)
            continue;
        }

        if (value == -1 && in_sequence) {
            vertex_count++;
            in_sequence = 0;
        } else if (value != -1) {
            in_sequence = 1;
        }
    }

    fclose(file);
    return vertex_count;
}

void read_distance_matrix(const char *distance_matrix_file, int vertex_count, int matrix[MAX_VERTICES][MAX_VERTICES]) {
    FILE *file = fopen(distance_matrix_file, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int value;
    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            if (fscanf(file, "%d", &value) != 1) {
                fprintf(stderr, "Error reading distance value at (%d, %d)\n", i, j);
                fclose(file);
                return;
            }
            matrix[i][j] = value;
        }
    }

    fclose(file);
}


int Min_Cardinality_GASP(int Counter[], int vertex_count)
{
int index = -1;
int index_prev = -1;
int value1 = INT_MAX; 
int value2 = INT_MAX - 1;



// Checks for vertex with lowest cardinality
for(int i=0; i<vertex_count; i++)
{
  if(Counter[i] != -1 && value1 >= Counter[i] )
  {
  index_prev = index; // copies the previous best index
  index = i; // copies new index
  
  value2 = value1; // copies previous best counter value
  value1 = Counter[i]; // copies new counter value

  }

}

// If no vertex has highest cardinality
 if( value1 != INT_MAX && value2 != INT_MAX - 1){ // when gasp is needed but need to check value1 and value2

srand(rand());

int choice = rand() % 2;

index = (choice == 1) ? index : index_prev; // ternary operator to choice any one index 
  
}else if(value2 == INT_MAX - 1){ // when value2 is not updated we make no changes to index

index_prev = index; // index will be retunrned

}else{ // when both value1 and value2 are not changed
  index = -1;
}


return index;
}




int Min_Cardinality(int Counter[], int vertex_count)
{
int index = -1;
int value1 = INT_MAX; 



// Checks for vertex with lowest cardinality
for(int i=0; i<vertex_count; i++)
{
  if(Counter[i] != -1 && value1 >= Counter[i] )
  {
  index = i;
  value1 = Counter[i];
  }
}


// If no vertex has highest cardinality
if( value1 == INT_MAX) // when gasp is not needed
{
index = -1;
}


return index;
}


int Max_Cardinality_GASP(int Counter[], int vertex_count) {
    int index = -1;
    int index_prev = -1;
    int value1 = INT_MIN;
    int value2 = INT_MIN + 1;

    // Checks for vertex with highest cardinality
    for (int i = 0; i < vertex_count; i++) {
        if (Counter[i] != -1 && value1 <= Counter[i]) {
            index_prev = index; // copies the previous best index
            index = i; // copies new index

            value2 = value1; // copies previous best counter value
            value1 = Counter[i]; // copies new counter value
        }
    }

    // If no vertex has highest cardinality
    if (value1 != INT_MIN && value2 != INT_MIN + 1) { // when gasp is needed but need to check value1 and value2
        srand(rand());

        int choice = rand() % 2;

        index = (choice == 1) ? index : index_prev; // ternary operator to choice any one index
    } else if (value2 == INT_MIN + 1) { // when value2 is not updated we make no changes to index
        index_prev = index; // index will be returned
    } else { // when both value1 and value2 are not changed
        index = -1;
    }

    return index;
}



int Best_index(int index1, int index2, int Q[MAX_VERTICES][MAX_VERTICES]){

int pointer1, pointer2, size, final_index;
pointer1 = 1; // since forbidden set starts from 1
pointer2 = 1; // since forbidden set starts from 1
size = Q[index1][0];
final_index = index1;

int count1 = Q[index1][0]; // Takes size of forbidden set of index 1
int count2 = Q[index2][0]; // Takes size of forbidden set of index 2


// Code segment to decide which Loop index is best 
  while(pointer1 <= size && pointer2 <= size){

     if(Q[index1][pointer1] == Q[index2][pointer2]){
      pointer1++;
      pointer2++;
      count1--;
      count2--;
    }else if(Q[index1][pointer1] < Q[index2][pointer2]){
      pointer1++;

    }else if(Q[index1][pointer1] > Q[index2][pointer2]){
      pointer2++;
    }

  }



//Selecting the best index based on the elimination of common vertices in the forbidden set
if(count1 < count2){
final_index = index1;
}else if(count1 > count2){
final_index = index2; 
}else{
int option = rand() % 2;

final_index = (option == 1)? index1 : index2;
}





return final_index;
}





int Max_Cardinality(int Counter[], int vertex_count, int Q[MAX_VERTICES][MAX_VERTICES])
{
int index = -1;
int value = INT_MIN; 

// Checks for vertex with highest cardinality
for(int i=0; i<vertex_count; i++)
{
  if(Counter[i]!= -1 && value < Counter[i])
  {
  index = i;
  value = Counter[i];
  }else if(Counter[i]!= -1 && value == Counter[i]){
    index = Best_index(index, i, Q); // Calls the function to pick the best index when forbidden set size is same
    value = Counter[i];
  }
}

// If no vertex has highest cardinality
if(value == INT_MIN)
{
index = -1;
}


return index;
}



//function to make the obtained solution a proper solution in the heuristic steps
void Isproper(int sol[],int *size, int vertex_count, int distance_matrix[MAX_VERTICES][MAX_VERTICES], int Q[MAX_VERTICES][MAX_VERTICES],int R){

int temp_sol[*size], temp_counter[*size]; // for storing solution vertices and its forbidden set size

int vertex_counter[vertex_count], counter_size; // for storing all vertices forbidden set size
int final_sol[vertex_count], final_size; // final solution array and its size
final_size = 0;
counter_size = vertex_count;

memcpy(temp_sol, sol, (*size)*sizeof(int)); // copies the solution into the temporary solution array


// loop to find the forbidden set of solution
for(int i=0; i< (*size); i++){
  int index = temp_sol[i] - 1; // gets the index for the Q
  temp_counter[i] = Q[index][0]; // stores the forbidden set size for the value present in solution i.e vertex

 // printf("%d -> %d ", temp_sol[i], temp_counter[i]);
}
//printf("\n");

// loop to find all forbidden set of vertices
for(int i=0; i< vertex_count; i++){
vertex_counter[i] = Q[i][0]; // stores forbidden set sizes in the array

//printf("%d ", vertex_counter[i]);
}
//printf("\n");



// Making the solution feasible
for(int i=0; i< (*size); i++){

int max_index = Max_Cardinality(temp_counter, *size, Q); // gets the index that holds maximum forbidden set

int index = temp_sol[max_index] - 1; // gets the index of that vertex for Q

if(vertex_counter[index] != -1){

int forbidden_size = Q[index][0]; // gets the forbidden size of that vertex

final_sol[final_size++] = temp_sol[max_index]; // adds that vertex to final solution

// marks all forbidden vertices to -1
for(int j=1; j<= forbidden_size; j++){

  int temp_index = Q[index][j] - 1; // gets the forbidden vertex index
  vertex_counter[temp_index] = -1; // puts -1 at forbidden vertex index in vertex counter array

}

vertex_counter[index] = -1; // puts -1 at that index in vertex counter
temp_counter[max_index] = -1; // puts -1 at that index in temporary counter

}else{
  temp_counter[max_index] = -1;
}


}


// loop to adjust size related to vertex_counter
for(int i=0; i< vertex_count; i++){
  if(vertex_counter[i] == -1){
    counter_size--;
  }
//printf("%d ", vertex_counter[i]);
}
//printf("\n");

//printf("%d ", counter_size);

// Loop to make solution proper
while(counter_size > 0){

int max_index = Max_Cardinality(vertex_counter, vertex_count, Q); // gets the index that holds maximum forbidden set


if(max_index != -1){

final_sol[final_size++] = max_index + 1; // adds that vertex to final solution

int forbidden_size = Q[max_index][0]; // gets the forbidden size of that vertex

// marks all forbidden vertices to -1
for(int j=1; j<= forbidden_size; j++){

  int temp_index = Q[max_index][j] - 1; // gets the forbidden vertex index
  vertex_counter[temp_index] = -1; // puts -1 at forbidden vertex index in vertex counter array

}

vertex_counter[max_index] = -1; // puts -1 at that index in vertex counter
counter_size--; // decreases counter size

}else{
  counter_size--; // decreases counter size
}



}



*size = final_size; // copies the final array size
memcpy(sol, final_sol, vertex_count*sizeof(int)); // copies the final array
}


//Function to return a randomized index of a vertex whose forbidden set is huge
int getIndex(int counter[], int* count_size, int vertex_count, int Q[MAX_VERTICES][MAX_VERTICES]){

int random_index = -1;
 srand(rand());

  if( (*count_size) == 1){
    

   //printf(" loop 1 \n");
   // if the counter has only 1 vertex
    for(int i=0; i<vertex_count; i++){
      if(counter[i] != -1){
        random_index = i;
        (*count_size) --;
      }
    }

  }else if( (*count_size) == 2){
 
  //printf(" loop 2 \n"); 
   // if the counter has 2 vertices 
    int temp1, temp2;
    int index1, index2;
    temp1 = INT_MAX;
    temp2 = INT_MAX - 1;
    index1 = index2 = -1;

    for (int i = 0; i < vertex_count; i++) {
        if (counter[i] != -1) {
            if (counter[i] < temp1) {
                temp2 = temp1;
                index2 = index1;
                temp1 = counter[i];
                index1 = i;
            } else if (counter[i] < temp2) {
                temp2 = counter[i];
                index2 = i;
            }
        }
    }
   
    int choice;
    choice = rand() % 2;
    random_index = (choice == 0)? index1: index2;
    counter[random_index] = -1;
    (*count_size)--;

  }else if( (*count_size) > 2){

    //printf(" loop 3 \n");
    // if the counter has more than 2 vertices
    int small_val[5], small_index[5];

    for(int i=0; i<5; i++){
      small_val[i] = INT_MAX; // stores largest values of after another
      small_index[i] = -1; // stores indices
    }
    
    // loop to find the small and small index array 
    for(int i=0; i<vertex_count; i++){

      if (counter[i] == -1) {
            continue;  // Skip -1 values
        }
      

      for (int j = 0; j < 5; j++) {
            if (counter[i] < small_val[j]) {
                // Shift larger elements and indices to the right to make room for the new smallest element
                for (int k = 4; k > j; k--) {
                    small_val[k] = small_val[k-1];
                    small_index[k] = small_index[k-1];
                }
                small_val[j] = counter[i];
                small_index[j] = i;
                break;
            }
        }

    }

      int small_size = 0;
      for(int i=0; i<5; i++){
        if(small_val[i] != INT_MAX){
          small_size++;
        }
      }

     int temp_index = rand() % small_size; // selects an index at random
     random_index = small_index[temp_index]; // gives an index at random
      (*count_size) --;
      counter[random_index] = -1;

     // puts -1 in its neighbours in the counter
      int forbidden_size = Q[random_index][0]; // size of the forbidden set

    for(int j=1; j<= forbidden_size; j++){
      counter[ Q[random_index][j] - 1] = -1; // puts -1 on the neighbours
      (*count_size) --; // decreases size
    }


  }


//printf("Exiting getIndex method\n");

return random_index;

}





int init_sol_gen (int sol[], int Q[MAX_VERTICES][MAX_VERTICES], int distance_matrix[MAX_VERTICES][MAX_VERTICES], int vertex_count, int R, double pd)
{

//printf("enteriing initial solution generation\n");


  int size = 0; // initial size of the solution
  double probability; // variable to generate the probability

  int vertices[vertex_count]; // vertices array
  int counter[vertex_count]; // to get the neigbour count avoiding forbidden set vertices
  int counter_size = vertex_count; 

  
  for(int i=0; i<vertex_count; i++)
{
  vertices[i] = i + 1; // initializes the vertices array
  counter[i] = vertex_count - Q[i][0] - 1; // gives vertices that can be added
}  


// copies Q and Q_dynamic into local arrays
int Q_temp[MAX_VERTICES][MAX_VERTICES], Q_dynamic_temp[MAX_VERTICES][MAX_VERTICES];
memcpy(Q_temp, Q, MAX_VERTICES*MAX_VERTICES*sizeof(int)); // copies Q into a temporary Q
memcpy(Q_dynamic_temp, Q_dynamic, MAX_VERTICES*MAX_VERTICES*sizeof(int) ); // copies Q_dynamic into a temporary array




int max_set, index;
max_set = Q_temp[0][0]; // gives forbidden set of first element
index = 0; // refers to the first element index i.e max_set

// loop to find the maximum forbidden set vertex
for(int i=1; i<vertex_count; i++){

  if(max_set == Q_temp[i][0]){ // To choose the best vertex for optimial result when forbidden set size is same
    index = Best_index(index, i, Q); // Calls the function to pick the best index when forbidden set size is same
    // We only update index as set size is same

  }else if(max_set < Q_temp[i][0]){ // If the set finds a new vertex with max forbidden set
    max_set = Q_temp[i][0];
    index = i;
  }
}



// Always add the vertex with highest forbidden set in the solution
sol[size++] = vertices[index]; // adds the value of the highest forbidden vertex to the solution
counter[index] = -1; // mark it -1 in the counter
counter_size--; // reduce the size

// Making all its neighbours -1 in the counter
int forbidden_size = Q_temp[index][0];

//Loop to mark -1 in the neighbours of highest forbidden set vertex
for(int i = 1; i <= forbidden_size; i++){

  int vertex_index = Q_temp[index][i]; // gives the forbidden vertex
 counter[Q_temp[index][i] - 1] = -1; // marks -1 to neighbours in the counter
 counter_size--; // reduce the size of the counter

 // Loop to put -1 in bit array where vertices are in forbidden set
 for(int j=0; j<vertex_count; j++){ // We access row wise to make change in a column i.e vertex index

 // checks if the vertex itself is not getting modified. Only other vertices containing it gets modified
  if(j != vertex_index - 1 && Q_dynamic_temp[j][vertex_index] == 0){ // modifies at one column value i.e forbidden vertex
  Q_dynamic_temp[j][vertex_index] = -1; // puts -1 instead of 0 for the forbidden vertex
  Q_dynamic_temp[j][0] = Q_dynamic_temp[j][0] - 1; // decreases size of the forbidden set
 }

 }

  Q_dynamic_temp[vertex_index - 1][0] = -1; // puts -1 at those vertex sizes whose are included in forbidden set
  Q_temp[vertex_index - 1][0] = -1;  // puts -1 at those vertex sizes whose are included in forbidden set

}


//Loop to readjust the forbidden set of all vertices
for(int i=0; i<vertex_count; i++){

// neglects vertex that being part of the solution along with its forbidden vertices
  if(Q_temp[i][0] != -1 && counter[i] != -1 && counter[i] != vertex_count - Q_temp[i][0] - 1 ){
   
   int temp_count, temp_index;
   temp_count = 0; // to calculate new size of forbidden set
   temp_index = 1; // to insert vertices in forbidden set

   for(int j = 1; j <= vertex_count; j++){
     if(Q_dynamic_temp[i][j] == 0){
      temp_count++;
      Q_temp[i][temp_index] = j; // adds forbidden vertex
      temp_index++;
     }
   }
   
   Q_temp[i][0] = temp_count; // updates the size of the forbidden set
   Q_dynamic_temp[i][0] = temp_count; // updates the size of the forbidden set
   counter[i] = vertex_count - Q_temp[i][0] - 1; // updates the counter
  }
}





printf("Vertex %d has forbidden set %d\n", sol[0], max_set);



//printf("Entering getIndex funtion\n");
// Initial solution generation
while(counter_size > 0){
  probability = (double)rand() / (RAND_MAX + 1.0);

//printf("%d ", counter_size);

  if(probability < pd){
   
   sol[size++] = getIndex(counter, &counter_size, vertex_count, Q_temp) + 1 ;// gets the index of randomized vertex whose forbidden set is huge and we add 1 to store its vertex
   
  }
}



  return size;
  

}




void RR_GA(int init_sol[], int Q[MAX_VERTICES][MAX_VERTICES], int distance_matrix[MAX_VERTICES][MAX_VERTICES], int vertex_count, int R, int size, int HH, float remove)
{
  int Remain[vertex_count], Counter[vertex_count],sol[vertex_count], remain_size;
  remain_size = vertex_count;
 
//printf("Entering RR_GA\n");


 // Loop to find remain array, copies solution array, Counter array i.e number of vertices more than R for each vertex
   for (int i = 0; i < vertex_count; i++) {
        Remain[i] = i + 1; // populate the remain array with vertices
        sol[i] = init_sol[i]; // copies entire initial solution
        Counter[i] = vertex_count - Q[i][0] -1; // gives the count excluding the vertex itself i.e vertices except neighbours and vertex itself
    }
  
  int elements_to_remove = round(size * remove); // Rounds off the value to get elements to remove
  

  //printf("entering removal i.e 1st step \n");
//Logic to remove the vertices from the initial solution i.e Random Removal
     int random_index, temp;
    for (int j = 0; j < elements_to_remove; j++) {
  
        random_index = rand() % size + 1; // Finds the random index from 1 to Initial_solution_array_size
        sol[random_index] = -1;
        size--; // decrement by 1 so that we find random value in range
        
        // Shifts the array when an vertex is removed and making sure that index is within the bound
        while((random_index + 1 < vertex_count) && sol[random_index + 1] != -1)
        {
         temp = sol[random_index + 1];
         sol[random_index + 1] = sol[random_index];
         sol[random_index] = temp;
         random_index = random_index + 1;
         
        }
    }


// Put -1 at those indices that are in initial solution
  for(int i=0; i<size; i++)
  {
    Remain[sol[i] - 1] = -1;
    Counter[sol[i] - 1] = -1;  // Set the count of the vertices to -1 in Counter
    remain_size--;
  }

//To mark all neigbours of highest forbidden set vertex -1 
int index = sol[0] - 1;

// Making all its neighbours -1 in the counter
int forbidden_size = Q[index][0];

//Loop to mark -1 in the neighbours of highest forbidden set vertex
for(int i=0; i<forbidden_size; i++){

 Counter[Q[index][i] - 1] = -1; // marks -1 to neighbours
 Remain[Q[index][i] - 1] = -1;
 remain_size--;
}


//printf("Enter additon i.e 2nd step\n");
// Logic to add vertices in the new solution i.e Greedy Addition
 int max_index;
 while(remain_size > 0)
 {
   max_index = Min_Cardinality_GASP(Counter, vertex_count);

    if(max_index != -1)
    {
          sol[size] = max_index + 1;
          size++;
          Remain[max_index] = -1;
          Counter[max_index] = -1; // puts -1 at existing max_index vertex in the set
          remain_size--;


        
        // loop to set all the selected vertices neighbours to -1
       int temp_size = Q[max_index][0]; // fetches size of the forbidden set size
       for(int j=1; j<=temp_size; j++)
       {
          int index = Q[max_index][j] - 1; // fetches the indices that are less than or equal to R
          Remain[index] = -1; // puts -1 at that index
          Counter[index] = -1;
          remain_size--;

        //  printf("max_index is %d, index is %d, j is %d\n", max_index, index, j);
       }   
    }
    else
    {
      remain_size--;
    }


//printf("Remaining size is %d\n", remain_size);
   
   
 }


// Making the obtained solution a proper solution 
//printf("Calling isproper from RR_GA");
Isproper( sol , &size, vertex_count, distance_matrix, Q, R);



//verifies the solution
//int verify_var = verify(sol , &size, vertex_count, distance_matrix, R);



// prints the array and size
if(HH == 0 && HH_RAND_size > size)
{
for(int i=0; i<size; i++)
{
HH_RAND[i] = sol[i];
}

HH_RAND_size = size;


}
else if (HH == 1 && HH_GREEDY_size > size)
{
  for(int i=0; i<size; i++)
{
  HH_GREEDY[i] = sol[i];
}

  HH_GREEDY_size = size;
 
}

}




void GR_GA(int init_sol[], int Q[MAX_VERTICES][MAX_VERTICES], int distance_matrix[MAX_VERTICES][MAX_VERTICES], int vertex_count, int R, int size, int HH, float remove)
{
    int Remain[vertex_count], Counter[vertex_count], sol[vertex_count], remain_size;
  remain_size = vertex_count;
 
  
 //printf("entering GR_GA\n");

    // Loop to find remain array, copies solution array, Counter array i.e number of vertices more than R for each vertex
   for (int i = 0; i < vertex_count; i++) {
        Remain[i] = i + 1; // populate the remain array with vertices
        sol[i] = init_sol[i]; // copies entire initial solution
        Counter[i] = vertex_count - Q[i][0] - 1; // gives the count excluding the vertex itself
    }
    
  
  int elements_to_remove = round(size * remove); // Rounds off the value to get elements to remove
  
  
  //printf("Entering removal i.e 1st step");
  //Logic to remove the vertices from the initial solution i.e Greedy Remove
     int random_index, temp;
    for (int j = 0; j < elements_to_remove; j++) {
    
    random_index = Max_Cardinality(Counter, vertex_count, Q);
     
        sol[random_index] = -1;
        size--; // decrement by 1 so that we find random value in range
        
        
        // Shifts the array when an vertex is removed and making sure that index is within the bound
        while((random_index + 1 < vertex_count) && sol[random_index + 1] != -1)
        {
         temp = sol[random_index + 1];
         sol[random_index + 1] = sol[random_index];
         sol[random_index] = temp;
         random_index = random_index + 1;
         
        }
    
  }
  

  // Put -1 at those indices that are in initial solution
  for(int i=0; i<size; i++)
  {
    Remain[sol[i] - 1] = -1;
    Counter[sol[i] - 1] = -1;  // Set the count of the vertices to -1 in Counter
    remain_size--;
  }


//To mark all neigbours of highest forbidden set vertex -1 
int index = sol[0] - 1;

// Making all its neighbours -1 in the counter
int forbidden_size = Q[index][0];

//Loop to mark -1 in the neighbours of highest forbidden set vertex
for(int i=0; i<forbidden_size; i++){

 Counter[Q[index][i] - 1] = -1; // marks -1 to neighbours
 Remain[Q[index][i] - 1] = -1;
 remain_size--;
}


//printf("entering addition i.e 2nd step");
int max_index;
 while(remain_size > 0)
 {
   max_index = Min_Cardinality_GASP(Counter, vertex_count);

    if(max_index != -1)
    {
          sol[size] = max_index + 1;
          size++;
          Remain[max_index] = -1;
          Counter[max_index] = -1; // puts -1 at existing max_index vertex in the set
          remain_size--;



       
        // loop to set all the selected vertices neighbours to -1
       int temp_size = Q[max_index][0]; // fetches size of the forbidden set size
       for(int j=1; j<=temp_size; j++)
       {
          int index = Q[max_index][j] - 1; // fetches the indices that are less than or equal to R
          Remain[index] = -1; // puts -1 at that index
          Counter[index] = -1;
          remain_size--;

        //  printf("max_index is %d, index is %d, j is %d\n", max_index, index, j);
       }   
    }
    else
    {
      remain_size--;
    }


//printf("Remaining size is %d\n", remain_size);
   
   
 }
     



// Making the obtained solution a proper solution 
//printf("Calling isproper from GR_GA");
Isproper( sol , &size, vertex_count, distance_matrix, Q, R);



//verifies the solution
//int verify_var = verify(sol , &size, vertex_count, distance_matrix, R);

 
// prints the array and size

if(HH == 0 && HH_RAND_size > size)
{
for(int i=0; i<size; i++)
{
HH_RAND[i] = sol[i];
}

HH_RAND_size = size;


}
else if (HH == 1 && HH_GREEDY_size > size)
{
  for(int i=0; i<size; i++)
{
  HH_GREEDY[i] = sol[i];
}

  HH_GREEDY_size = size;
 
}

}



void GA_GR(int init_sol[], int Q[MAX_VERTICES][MAX_VERTICES], int distance_matrix[MAX_VERTICES][MAX_VERTICES], int vertex_count, int R, int size, int HH, float remove)
{

 int Remain[vertex_count], Counter[vertex_count], sol[vertex_count], remain_size;
  remain_size = vertex_count;
 
  //printf("Entering GA_GR\n");
  
   // Loop to find remain array, copies solution array, Counter array i.e number of vertices more than R for each vertex
   for (int i = 0; i < vertex_count; i++) {
        Remain[i] = i + 1; // populate the remain array with vertices
        sol[i] = init_sol[i]; // copies entire initial solution
        Counter[i] = vertex_count - Q[i][0] -1 ; // gives the count excluding the vertex itself
    }
    
  
  int elements_to_remove = round(size * remove); // Rounds off the value to get elements to remove or add
  
  // Put -1 at those indices that are in initial solution
  for(int i=0; i<size; i++)
  {
    Remain[sol[i] - 1] = -1;
    remain_size--;
  }
  
  
// Set the count of the vertices to -1 in Counter
  for(int i=0; i<vertex_count; i++)
  {
  if(Remain[i] == -1)
  {
    Counter[i] = -1;  
  }
  
  }


  
  //printf("Entering addition i.e 1st step");
// Logic to add vertices in the new solution i.e Greedy addition
 int max_index;
 while(remain_size > 0)
 {
   max_index = Min_Cardinality(Counter, vertex_count); // finds the vertex with maximum cardinality
   
   if(max_index != -1) // Checks if vertex is not in range of other vertices and adds it to the solution array sol
   {
     
          sol[size] = max_index + 1;
          size++;
          Remain[max_index] = -1;
          Counter[max_index] = -1; // puts -1 at existing max_index vertex in the set
          remain_size--;
        
          
       
     
     // Setting all neighbours of max_index to -1 in the count array to avoid adding them to solution
  int temp_size = Q[max_index][0];

  for(int i=1; i<=temp_size; i++)
  {
    int index = Q[max_index][i] - 1;
    Counter[index] = -1;
    Remain[index] = -1;
    remain_size--;

  } 

   }else{
    remain_size--;
   }
          
 }
 
  
  
//printf("Calling isproper from GR_GA i.e 2nd step");
Isproper( sol , &size, vertex_count, distance_matrix, Q, R);

 
 // prints the array and size

if(HH == 0 && HH_RAND_size > size)
{
for(int i=0; i<size; i++)
{
HH_RAND[i] = sol[i];
}

HH_RAND_size = size;


}
else if (HH == 1 && HH_GREEDY_size > size)
{
  for(int i=0; i<size; i++)
{
  HH_GREEDY[i] = sol[i];
}

  HH_GREEDY_size = size;
  
}


}




int getIndex_RA_GR(int counter[], int remain_size, int vertex_count, int Q[MAX_VERTICES][MAX_VERTICES]){

int random_index = -1;

 if(remain_size == 1){
   //printf(" loop 1 \n");
   // if the counter has only 1 vertex
    for(int i=0; i<vertex_count; i++){
      if(counter[i] != -1){
        random_index = i;
          }
        }
    } else if(remain_size == 2){
 
  //printf(" loop 2 \n"); 
   // if the counter has 2 vertices 
    int temp1, temp2;
    int index1, index2;
    temp1 = INT_MAX;
    temp2 = INT_MAX - 1;
    index1 = index2 = -1;

    for (int i = 0; i < vertex_count; i++) {
        if (counter[i] != -1) {
            if (counter[i] < temp1) {
                temp2 = temp1;
                index2 = index1;
                temp1 = counter[i];
                index1 = i;
            } else if (counter[i] < temp2) {
                temp2 = counter[i];
                index2 = i;
            }
        }
    }
   
    int choice;
    choice = rand() % 2;
    random_index = (choice == 0)? index1: index2;

  } else if(remain_size > 2){

    //printf(" loop 3 \n");
    // if the counter has more than 2 vertices
    int small_val[5], small_index[5];

    for(int i=0; i<5; i++){
      small_val[i] = INT_MAX; // stores largest values of after another
      small_index[i] = -1; // stores indices
    }
    
    // loop to find the small and small index array 
    for(int i=0; i<vertex_count; i++){

      if (counter[i] == -1) {
            continue;  // Skip -1 values
        }
      

      for (int j = 0; j < 5; j++) {
            if (counter[i] < small_val[j]) {
                // Shift larger elements and indices to the right to make room for the new smallest element
                for (int k = 4; k > j; k--) {
                    small_val[k] = small_val[k-1];
                    small_index[k] = small_index[k-1];
                }
                small_val[j] = counter[i];
                small_index[j] = i;
                break;
            }
        }

    }

      int small_size = 0;
      for(int i=0; i<5; i++){
        if(small_val[i] != INT_MAX){
          small_size++;
        }
      }

     int temp_index = rand() % small_size; // selects an index at random
     random_index = small_index[temp_index]; // gives an index at random

}
return random_index;
}

void RA_GR(int init_sol[], int Q[MAX_VERTICES][MAX_VERTICES], int distance_matrix[MAX_VERTICES][MAX_VERTICES], int vertex_count, int R, int size, int HH, float remove)
{

 int Remain[vertex_count], Counter[vertex_count], sol[vertex_count], remain_size;
  remain_size = vertex_count;
  
  //printf("Entering RA_GR\n");
  
   // Loop to find remain array, copies solution array, Counter array i.e number of vertices more than R for each vertex
   for (int i = 0; i < vertex_count; i++) {
        Remain[i] = i + 1; // populate the remain array with vertices
        sol[i] = init_sol[i]; // copies entire initial solution
        Counter[i] = vertex_count - Q[i][0] - 1; // gives the count excluding the vertex itself
    }
    
  
  int elements_to_remove = round(size * remove); // Rounds off the value to get elements to remove
  
  // Put -1 at those indices that are in initial solution
  for(int i=0; i<size; i++)
  {
    int index = sol[i] - 1;
    Remain[index] = -1;
    Counter[index] = -1;
    remain_size--;
  }
  

 // printf("Entering addition\n");
// Logic to add vertices in the new solution i.e Random addition
 int max_index, temp;
 while(remain_size > 0)
 {

     max_index = getIndex_RA_GR(Counter, remain_size, vertex_count, Q); // Finds the random index from 0 to _array_size

          sol[size] = max_index + 1;
          size++;
          Remain[max_index] = -1;
          remain_size--;
          Counter[max_index] = -1; // puts -1 at existing max_index vertex in the set
          
   
   
// Setting all neighbours of max_index to -1 in the count array to avoid adding them to solution
  int temp_size = Q[max_index][0];

  for(int i=1; i<=temp_size; i++)
  {
    int index = Q[max_index][i] - 1;
    Counter[index] = -1;
    Remain[index] = -1;

    remain_size--;

  } 

 }
   

 


 //printf("Calling isproper from GR_GA i.e 2nd step");
Isproper( sol , &size, vertex_count, distance_matrix, Q, R);
 
 
 // prints the array and size
if(HH == 0 && HH_RAND_size > size)
{
for(int i=0; i<size; i++)
{
HH_RAND[i] = sol[i];
}

HH_RAND_size = size;


}
else if (HH == 1 && HH_GREEDY_size > size)
{
  for(int i=0; i<size; i++)
{
  HH_GREEDY[i] = sol[i];
}

  HH_GREEDY_size = size;
  
}
 
 

}








int main() {


 // to start measuring time
 struct tms start_time, end_time;
 clock_t start_clock, end_clock;
 long ticks_per_second;

    // Get the number of clock ticks per second
    ticks_per_second = sysconf(_SC_CLK_TCK);

    // Record the start time
    start_clock = times(&start_time);
    if (start_clock == (clock_t)-1) {
        perror("times");
        return 1;
    }



    int HH = -1; // Variable to set Hyper-Heuristic
    char filename[MAX_FILENAME_LENGTH];
    char folderpath[MAX_FOLDERPATH_LENGTH];
    int choice;
    
    
    printf("Choose the dataset:\n");
    printf("1. DACLP_Datasets/ORLib_Datasets\n");
    printf("2. DACLP_Datasets/TSP_Datasets\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        strcpy(folderpath, "DACLP_Datasets/ORLib_Datasets");
    } else if (choice == 2) {
        strcpy(folderpath, "DACLP_Datasets/TSP_Datasets");
    } else {
        printf("Invalid choice\n");
        return 1;
    }

    printf("Enter the file name: ");
    scanf("%s", filename);

    char filepath[MAX_FOLDERPATH_LENGTH + MAX_FILENAME_LENGTH];
    
    // Form the file path
snprintf(filepath, MAX_FILEPATH_LENGTH, "%s/%s", folderpath, filename);

// Now you can use the file path (filepath) to read the distance matrix
printf("File path formed: %s\n", filepath);

int vertex_count = count_vertices(filepath);


HH_RAND_size = vertex_count;
HH_GREEDY_size = vertex_count;

    if (vertex_count == -1) {
        fprintf(stderr, "Unable to determine the number of vertices.\n");
        return 1;
    }
    

    printf("Number of vertices: %d\n", vertex_count);

    int distance_matrix[MAX_VERTICES][MAX_VERTICES];
    int Q[MAX_VERTICES][MAX_VERTICES],Q_bit[MAX_VERTICES][MAX_VERTICES];

    // Read the distance matrix into the allocated matrix
   read_distance_matrix(filepath, vertex_count, distance_matrix);
    
    

     
    // initial solution generation
    double pd = 0.75; // Probability to add a vertex
    int R;// distance R
    float remove[4];
    
    printf("Enter the distance R: ");
    scanf("%d", &R);
    printf("Enter number of vertices to add or remove for RR_GA: ");
    scanf("%f", &remove[0]);
    printf("Enter number of vertices to add or remove for GR_GA: ");
    scanf("%f", &remove[1]);
    printf("Enter number of vertices to add or remove for GA_GR: ");
    scanf("%f", &remove[2]);
    printf("Enter number of vertices to add or remove for RA_GR: ");
    scanf("%f", &remove[3]);




   // Sets Q , Q_dynamic, and Q_bit entire 2D array
    int temp_counter, temp_index;
    for (int i = 0; i < vertex_count; i++) {
      temp_counter = 0; // used to add the count
      temp_index = 1; // used to add values in the forbidden set from the index 1 of each vertex

        for (int j = 0; j < vertex_count; j++) {

            if(distance_matrix[i][j] <= R){ // Vertex is a neighbour i.e within R

              if(i!= j) // self vertex is not added in the set
              {
              Q[i][temp_index] = j + 1; // adds value to forbidden set
              temp_index++;
              temp_counter++;
              Q_bit[i][j] = 0; // sets value to 0 if vertex is within R in forbidden set
              Q_dynamic[i][j + 1] = 0; // sets value to 0 if vertex is within R in forbidden set
              }else if(i == j){
              Q_bit[i][j] = -1; // sets value to -1 if vertex is referring itself i.e i == j
              Q_dynamic[i][j + 1] = -1; // sets value to -1 if vertex is referrin itself i.e i + 1 == j since bits are stored from 1 to vertex_count
              }        
                }
            else{ // vertex doesn't gets added to the forbidden set 
               Q_bit[i][j] = 1; // sets value to 1 if vertex is not witin R in forbidden set
               Q_dynamic[i][j + 1] = 1; // sets value to 1 if vertex is not within R
            }
        }

       Q[i][0] = temp_counter; // keeps size of the forbidden set
       Q_dynamic[i][0] = temp_counter; // keeps the size of the forbidden set in Q dynamic
    }

//printf("Computed forbidden sets\n");


/*
// prints the forbidden set Q
for(int i=0; i<vertex_count; i++)
{
  int size = Q[i][0]; // gives the size of the forbidden set 
  printf("%d vertex forbidden set size is %d and its elements are\n", i+1, size);
  
  for(int j=1; j<= size; j++)
  {
    printf("%d ", Q[i][j]);
  }
  printf("\n");
}
*/


/*
//loop to print Q_dynamic

for(int i=0; i<vertex_count; i++){
int size = Q_dynamic[i][0]; // gives the size of the forbidden set 
  printf("%d vertex forbidden set size is %d and its elements are\n", i+1, size);
  
  for(int j=1; j<= vertex_count; j++)
  {
    printf("%d ", Q_dynamic[i][j]);
  }
  printf("\n");
}

*/


    int init_solution[vertex_count], init_count;
    
    for(int i=0; i<vertex_count; i++)
    {
      init_solution[i] = -1;
    }
  
  
  for(int j=0; j<10; j++){

 // Function call to initial solution generation
  int init_sol_size =  init_sol_gen(init_solution, Q, distance_matrix, vertex_count, R, pd);
 
 printf("..........................................................\n");
  
  // prints initial solution
  for(int i=0; i<init_sol_size; i++)
  {
   printf("%d ", init_solution[i]);
  }
  printf("\n");
  printf("\nSize of initial solution is %d\n", init_sol_size);
  printf("\n");




    // Loop to take restarts
    for (int i=0; i< 100; i++)
    {
    srand(i); // setting the seed value to 1 before the function call
    
   
  

   HH = 0;
  
  switch(rand() % 4)
  {
  
  
  // Random Remove and Greedy Add
 case 0: RR_GA(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[0]);
          break;
 
 
 // Greedy Remove and Greedy Add
  case 1: GR_GA(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[1]);
          break;
 
 
 // Greedy Add and Greedy Remove
 case 2: GA_GR(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[2]);
        break;
 
 // Random Add and Greedy Remove
 case 3: RA_GR(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[3]);
         break; 

 }
 
 HH = 1;
 RR_GA(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[0]);
 GR_GA(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[1]);
 GA_GR(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[2]);
 RA_GR(init_solution, Q, distance_matrix, vertex_count, R, init_sol_size, HH, remove[3]);

 
 // To find average
 HH_GREEDY_AVG += HH_GREEDY_size;
 HH_RAND_AVG += HH_RAND_size; 
 
printf("\n");

for(int i=0; i<HH_RAND_size; i++)
{
  printf("%d ", HH_RAND[i]);
}

printf("\n");
printf("\nThe size of HH_RAND is %d\n", HH_RAND_size);
printf("\n");

for(int i=0; i<HH_GREEDY_size; i++)
{
  printf("%d ", HH_GREEDY[i]);
}

printf("\n");
printf("\nThe size of HH_GREEDY is %d\n", HH_GREEDY_size);




printf("...............................................................\n");
} // end of for loop 

} // end of outer most loop

printf("Average size of HH_RAND is %.2f \n", HH_RAND_AVG / 1000.0);
printf("Average size of HH_GREEDY is %.2f \n", HH_GREEDY_AVG / 1000.0);

// Record the end time
    end_clock = times(&end_time);
    if (end_clock == (clock_t)-1) {
        perror("times");
        return 1;
    }

    double total_time = (double)(end_clock - start_clock) / ticks_per_second; // gives cpu time for entire program to run


    printf("Total time: %lf seconds\n", total_time);

    return 0;
}


