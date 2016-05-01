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
#define NUM_THREADS 12 

#define VERTICES 100000
#define EDGES 5
#define STARTV 6
using namespace std;

int size[VERTICES] = {};
int *visited = new int[VERTICES];
int **graph = new int*[VERTICES];
deque<int> q;


int main() {
    int i;

    //Initialize Size and Visited Vectors
    for (i = 0; i < VERTICES; i++) {
        visited[i] = 0;
    }

    //Allocated Graph to be searched size VERTICES X VERTICES
    for (i = 0; i < VERTICES; i++) {
        graph[i] = new int[VERTICES];
    }
 
    //Popluate Graph
    //populate_random(graph, size, VERTICES, EDGES);
    populate_known(graph, size, VERTICES, EDGES);

    //Breadth First Search
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    //for (i = 0; i < VERTICES; i++) {
    bfs(STARTV);
    //}
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    //Calcuate Cycles Required to Execute
    elapsedTime = diff(time1, time2);   
    printf("%ld\n", (long int) ((
        (double) 10e-3) * 
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
    int next_vertex, j, ilimit;
    long vertex = (long) arg;
        
    if (!visited[vertex]) {
        visited[vertex] += 1;
        q.push_back(vertex);
   
        while(!q.empty()) {
            vertex = q.front();
            q.pop_front();
            ilimit = size[vertex];
     
            for (j = 0; j < ilimit; j++) {
                next_vertex = graph[vertex][j];
                if (!visited[next_vertex]) {
                    visited[next_vertex] += 1;
                    q.push_back(next_vertex);
                }
            }                  
        }
    }
}


//BFS Search
void bfs(int vertex) {
    int i, error;
    pthread_t threads[NUM_THREADS];   

    for (i = 0; i < NUM_THREADS; i++) {       
        error = pthread_create(&threads[i], NULL, &bfs_parellel_region, (void *) vertex);
        if (error) {
            printf("Thread %d could not be created\n", i);
        }
        error = pthread_join(threads[i], NULL);
        if (error) {
            printf("Thread %d could not be joined\n", i);
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
        size[i] = vertices;
        graph[i][0] = i;
        for (j = 1; j < size[i]; j++) {
            graph[i][j] = (j + i) % vertices;
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

