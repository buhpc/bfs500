#include <vector>
#include <queue>

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

