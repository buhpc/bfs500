#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <pthread.h>

#include "bfs_pthread.h"

#define GIG 10e9
#define CPG 2.90
#define NUM_THREADS 4

#define VERTICES 100
#define EDGES 5

using namespace std;


int main() {
    int i;

    //Initialize Size and Visited Vectors
    int size[VERTICES] = {};
    int *visited = new int[VERTICES];
    for (i = 0; i < VERTICES; i++) {
        visited[i] = 0;
    }

    //Allocated Graph to be searched size VERTICES X VERTICES
    int **graph = new int*[VERTICES];
    for (i = 0; i < VERTICES; i++) {
        graph[i] = new int[VERTICES];
    }
 
    //Popluate Graph
    populate_random(graph, size, VERTICES, EDGES);
    //populate_known(graph, size, VERTICES, EDGES);

    //Breadth First Search
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (i = 0; i < VERTICES; i++) {
        if (!visited[i]) {
            bfs(graph, size, i, visited);
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    //Calcuate Cycles Required to Execute
    elapsedTime = diff(time1, time2);   
    printf("%ld Cycles\n", (long int) ((
        (double) CPG) * 
        ((double) (GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec))
    ));

    //Validation - Checks that each node in the tree was visited exactly once
    for (i = 0; i < VERTICES; i++) {
        if (visited[i] != 1) {
            printf("visited[%d] = %d\n", i, visited[i]);
        }
    }

    return 0;
}


//Thread Function
void *bfs_parellel_region(void *arg) {
    int vertex, next_vertex, i;

    
}


//Argument Data structure
struct thread_args {
    int vertex;
};


//BFS Search
void bfs(int **graph, int *size, int vertex, int *visited) {
    int i, error;
        
    visited[vertex] += 1;
    deque<int> *vertex_queue = new deque<int>[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++) {
        vertex_queue[i].push_back(vertex);
        error = pthread_create(&threads[i], NULL, &bfs_parellel_region, NULL);
        if (error) {
            printf("Thread %d could not be created\n", i);
        }
           
    }
} 
  
 
void populate_random(int **graph, int *size, const int vertices, const int edges) {
    int i, j;
    srand(time(NULL));

    for (i = 0; i < vertices; i++) {
        size[i] = rand() % edges;
        for (j = 0; j < size[i]; j++) {
            graph[i][j] = rand() % vertices;
        }
    }
}


void populate_known(int **graph, int* size, const int vertices, const int edges) {
    int i, j;

    for (i = 0; i < vertices; i++) {
        size[i] = i % edges;
        for (j = 0; j < size[i]; j++) {
            graph[i][j] = j % vertices;
        }
    }
}


struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 10e9 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return temp;
}

