#include <iostream>
#include <vector>
#include <queue>

#define VERTICES 1000000
#define EDGES 10

#define GIG 1000000000
#define CPG 3.07            // Cycles per GHz -- Adjust to your computer

#include "bfs_serial.h"

using namespace std;

//Unoptimized
void bfs(int** graph, int *size, int *visited, int vertex) {
    // double-ended queue
    deque<int> q;
    int j, ilimit, next_vertex; 

    visited[vertex] += 1; 
    q.push_back(vertex);
            
    while (!q.empty()) {
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


//Optimized verion 1: Inner loop unrolled by a factor of 4 and multiple accumulators
void bfs_opt(int **graph, int *size, int *visited, int vertex) {
    deque<int> q;
    int j, ilimit, next_vertex0, next_vertex1, next_vertex2, next_vertex3;
       
    visited[vertex] += 1;
    q.push_back(vertex);
 
    while (!q.empty()) {
       vertex = q.front();
       q.pop_front();
       ilimit = size[vertex] - 3;
       for (j = 0; j < ilimit; j += 4) {
           next_vertex0 = graph[vertex][j];
           next_vertex1 = graph[vertex][j+1];
           next_vertex2 = graph[vertex][j+2];
           next_vertex3 = graph[vertex][j+3];
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
        ilimit = size[vertex];
        for (j; j < ilimit; j++) {
            next_vertex0 = graph[vertex][j];
            if (!visited[next_vertex0]) {
                visited[next_vertex0] += 1;
                q.push_back(next_vertex0);
            }
        }
    }
}

void printGraph(int **graph) {
	int i;

	for (i = 0; i < VERTICES; i++) {
    	cout << i << " ";
    	for (int j = 0; j < VERTICES; j++) {
    		if (graph[i][j] != 0) {
    			printf("%d ", graph[i][j]);
    		}
    	}
    	cout << endl;
    }
}

/*
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

	// Generate the graphs
    populate_random(graph, size, VERTICES, EDGES);
    // populate_known(graph, size, VERTICES, EDGES);	

    clock_gettime(CLOCK_MONOTONIC, &time1);
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);

	// Breadth first search
	for (i = 0; i < VERTICES; i++) {
		if (!visited[i]) {
			bfs(graph, size, i, visited);
		}
	}

    clock_gettime(CLOCK_MONOTONIC, &time2);
	// clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

	// elapsedTime = diff(time1, time2);

	cout << "Serial BFS" << endl;
	/*
	printf("CPE: %ld\n", (long int)((double)(CPG) * (double)(GIG * elapsedTime.tv_sec + elapsedTime.tv_nsec)));
	long ms = (elapsedTime.tv_sec * 1000) + (elapsedTime.tv_nsec / 1.0e6);		
 	printf("Time: %ld (msec)\n", ms);
 	*/
 	/*
 	double testTime = timeInSeconds(&time2)-timeInSeconds(&time1);
    printf("Milliseconds: %lf\n", testTime * 1000.0);

	return 0;
}
*/

double timeInSeconds(struct timespec *t) {
    // a timespec has integer values for seconds and nano seconds
    return (t->tv_sec + 1.0e-9 * (t->tv_nsec));
}
