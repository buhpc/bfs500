#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>

#define VERTICES 90000
#define EDGES 100 
#define STARTV 0
#define GIG 10e9

#include "bfs_serial.h"
#include "bfs_main.h"

using namespace std;


int main() {
//=============Initialization============================
    int i;

    // graph represents the matrix
    int **graph = new int*[VERTICES];
    for (i = 0; i < VERTICES; i++) {	
    	graph[i] = new int[VERTICES+EDGES+2]; 
    }  	
 
    int size[VERTICES] = {};
    //populate_random(graph, size, VERTICES, EDGES);
    populate_known(graph, size, VERTICES, EDGES);	

    // visited contains whether a vertex has been visited
    int *visited = new int[VERTICES];
//==============Serial Version=================================
    for (i = 0; i < VERTICES; i++) {
        visited[i] = 0;
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    //for (i = 0; i < VERTICES; i++) {
        bfs(graph, size, visited, STARTV); 
    //}
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    elapsedTime = diff(time1, time2);
    //Prints execution time IN MSEC!!!
    printf("%ld\n", (long int)((double)(10e-3) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec))); 
    
    //Validation - Checks each vertex was visited once
    for (i = 0; i < VERTICES; i++) {
        if (visited[i] != 1) {
            printf("visited[%d] was %d\n", i, visited[i]);
        }
    }
//=============Serial Optimized Version 1====================
    //Zero visited array for next test
    for (i = 0; i < VERTICES; i++) {
        visited[i] = 0;
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    //for (i = 0; i < VERTICES; i++) {
        bfs_opt(graph, size, visited, STARTV);
    //}
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    elapsedTime = diff(time1, time2);
    
    printf("%ld\n", (long int)((double)(10e-3) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec)));
    for (i = 0; i < VERTICES; i++) {
        if (visited[i] != 1) {
            printf("visited[%d] was %d\n", i , visited[i]);
        }
    }
  
    //Print Graph 
    /*int j;
    for (i = 0; i < VERTICES; i++) {
        printf("%d ", graph[i][0]);
        for (j = 1; j < size[i]; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }*/
    return 0;
}


//Populates graph matrix and size array with random values
void populate_random(int **graph, int *size, const int vertices, const int edges) {
    int i, j, sizev;
    queue<int> unassigned;
    srand(time(NULL));
    for (i = 0; i < vertices; i++) {
        unassigned.push(i); //Every vertex is initially unassigned
    }
    
    for (i = 0; i < vertices; i++) {
        sizev = rand() % edges + 2; //Each vertex connected to at least one other
        size[i] = sizev;
        graph[i][0] = i;
        for (j = 1; j < sizev; j++) {
            if(!unassigned.empty() && unassigned.front() != i) { 
                graph[i][j] = unassigned.front();
                unassigned.pop(); 
            } else {
                graph[i][j] = rand() % vertices;            
            }      
        }
    }
}


//Populates graph matrix and size array with known values
void populate_known(int **graph, int* size, const int vertices, const int edges) {
    int i, j;
    j = 0;
 
    for (i = 0; i < vertices; i++) {
        size[i] = vertices;
        graph[i][0] = i;
        for (j = 1; j < size[i]; j++) {
            graph[i][j] = (j + i) % vertices;
        }       
    }
}


struct timespec diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec-start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

