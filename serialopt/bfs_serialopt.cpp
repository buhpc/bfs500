
#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>

#define VERTICES 1000
#define EDGES 10 

#define GIG 1000000000
#define CPG 2.90            // Cycles per GHz -- Adjust to your computer

#include "bfs_serialopt.h"

using namespace std;


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


int main() {
        int i;

	// graph represents the matrix
	int **graph = new int*[VERTICES];
	for (i = 0; i < VERTICES; i++) {	
    	    graph[i] = new int[VERTICES]; 
  	}

	int size[VERTICES] = {};
	
	// visited contains whether a vertex has been visited
	int *visited = new int[VERTICES];
        for (i = 0; i < VERTICES; i++) {
            visited[i] = 0;
        }

	// Generate the graphs
        //populate_random(graph, size, VERTICES, EDGES);
        populate_known(graph, size, VERTICES, EDGES);	

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	// Breadth first search
	for (i = 0; i < VERTICES; i++) {
		if (!visited[i]) {
			bfs(graph, size, i, visited);
		}
	}

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	elapsedTime = diff(time1, time2);

	cout << "Serial BFS" << endl;
	printf("CPE: %ld\n", (long int)((double)(CPG) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec))); 
    
        //Validation - Checks each vertex was visited once
        for (i = 0; i < VERTICES; i++) {
            if (visited[i] != 1) {
                printf("visited[%d] was %d\n", i, visited[i]);
            }
        }
	return 0;
}

void bfs(int** graph, int *size, int vertex, int *visited) {
	// double-ended queue
	deque<int> q;
       
	visited[vertex] += 1;
	q.push_back(vertex);

	int i, next_vertex0, next_vertex1, next_vertex2, next_vertex3;
        int limit = size[vertex];

	while (!q.empty()) {
		vertex = q.front();
		q.pop_front();

		for (i = 0; i < limit; i += 4) {
                        next_vertex0 = graph[vertex][i];
                        next_vertex1 = graph[vertex][i+1];
                        next_vertex2 = graph[vertex][i+2];
                        next_vertex3 = graph[vertex][i+3];
			if (!visited[next_vertex0]) {
				visited[next_vertex0] += 1;
				q.push_back(next_vertex0);
			}
                        if (!visited[next_vertex1]) {
                        	visited[next_vertex1] += 1;
                                q.push_back(next_vertex1);
                        }
                        if (!visited[next_vertex2]) {
                                visited[next_vertex2] += 1;
                                q.push_back(next_vertex2);
                        }
                        if (!visited[next_vertex3]) {
                                visited[next_vertex3] += 1;
                                q.push_back(next_vertex3);
                        }
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
