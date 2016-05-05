#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <pthread.h>

#include "bfs_pthread.h"

#define GIG 10e9
#define NUM_THREADS 4 

#define VERTICES 50000
#define EDGES 10 
#define STARTV 0 
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
        graph[i] = new int[VERTICES+EDGES+2];
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
    int vertex, next_vertex, j, ilimit;
         
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


//BFS Search
void bfs(int vertex) {
    int i, error, j, next_vertex, ilimit;
    pthread_t threads[NUM_THREADS];   

    visited[vertex] += 1;
    q.push_back(vertex);
    
    for (i = 0; i < NUM_THREADS; i++) {       
        error = pthread_create(&threads[i], NULL, &bfs_parellel_region, NULL);
        if (error) {
            printf("Thread %d could not be created\n", i);
        }
        error = pthread_join(threads[i], NULL);
        if (error) {
            printf("Thread %d could not be joined\n", i);
        }
    }

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

