/*
 * bfs_cuda.cu
 * nvcc -o bfs_cuda bfs_cuda.cu 
 */

#include <iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <vector>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <cuda.h>
// #include "cuPrintf.cu"
// #include "cuPrintf.cuh"

#define VERTICES 1000000
#define EDGES 20
#define MAX_THREADS_PER_BLOCK 256

#define GIG 1000000000
#define CPG 2.60            // Cycles per GHz -- Adjust to your computer

#include "bfs_cuda.cuh"

using namespace std;

int main() {
	// GPU Timing variables
	cudaEvent_t start, stop, start1, stop1;
	float elapsed_gpu, elapsed_gpu1;	

	printf("Number of vertices: %d\n", VERTICES);
	printf("Max number of edges: %d\n", EDGES);

	Node* graph_nodes = (Node*) malloc(sizeof(Node) * VERTICES);
	int* graph_edge;
	
	bool *graph_mask;
	if ((graph_mask = (bool*) malloc(sizeof(bool) * VERTICES)) == NULL) {
		printf("Could not allocate memory for graph_mask.\n");
		exit(1);
	}

	bool *updating_graph_mask;
	if ((updating_graph_mask = (bool*) malloc(sizeof(bool) * VERTICES)) == NULL) {
		printf("Could not allocate memory for updating_graph_mask.\n");
		exit(1);
	}
	bool *graph_visited;
	if ((graph_visited = (bool*) malloc(sizeof(bool) * VERTICES)) == NULL) {
		printf("Could not allocate memory for graph_visited.\n");
		exit(1);
	}
	bool *h_graph_visited;
	if ((h_graph_visited = (bool*) malloc(sizeof(bool) * VERTICES)) == NULL) {
		printf("Could not allocate memory for graph_mask.\n");
		exit(1);
	}

	int i = 0;
	int j = 0;
	int len;

	populate_random(graph_nodes, graph_edge, graph_mask, updating_graph_mask, graph_visited, h_graph_visited);
	// populate_known(graph_nodes, graph_edge, graph_mask, updating_graph_mask,graph_visited, h_graph_visited);

	// Create the cuda events
	cudaEventCreate(&start1);
	cudaEventCreate(&stop1);
	// Record event on the default stream
	cudaEventRecord(start1, 0);

	int vertex;
	for (vertex=0; vertex < VERTICES; vertex++) {
		if (!h_graph_visited[vertex]) {   		
			bfs(graph_nodes,graph_edge,vertex,h_graph_visited);	
		}
	}

	// Stop and destroy the timer
	cudaEventRecord(stop1,0);
	cudaEventSynchronize(stop1);
	cudaEventElapsedTime(&elapsed_gpu1, start1, stop1);
	printf("\nCPU time: %f (msec)\n", elapsed_gpu1);
	cudaEventDestroy(start1);
	cudaEventDestroy(stop1);
	
	int source=0;

	// Set the source node as true in the mask.
	graph_mask[source]=true;
	graph_visited[source]=true;

	// Copy the Graph to device memory.
	Node *d_graph;
	CUDA_SAFE_CALL(cudaMalloc( (void**) &d_graph, sizeof(Node) *VERTICES)) ;
	CUDA_SAFE_CALL(cudaMemcpy( d_graph, graph_nodes, sizeof(Node) *VERTICES, cudaMemcpyHostToDevice));

	// Copy the Edge List to device Memory.
	int* d_edge;
	cudaMalloc( (void**) &d_edge, sizeof(int)*(len)) ;
	cudaMemcpy( d_edge, graph_edge, sizeof(int)*(len), cudaMemcpyHostToDevice) ;

	// Copy the Mask to device memory.
	bool* d_graph_mask;
	CUDA_SAFE_CALL(cudaMalloc( (void**) &d_graph_mask, sizeof(bool)*VERTICES)) ;
	CUDA_SAFE_CALL(cudaMemcpy( d_graph_mask, graph_mask, sizeof(bool)*VERTICES, cudaMemcpyHostToDevice));

	bool* d_updating_graph_mask;
	CUDA_SAFE_CALL(cudaMalloc( (void**) &d_updating_graph_mask, sizeof(bool)*VERTICES)) ;
	CUDA_SAFE_CALL(cudaMemcpy( d_updating_graph_mask, updating_graph_mask, sizeof(bool)*VERTICES, cudaMemcpyHostToDevice)) ;

	// Copy the visited nodes array to device memory.
	bool* d_graph_visited;
	CUDA_SAFE_CALL(cudaMalloc( (void**) &d_graph_visited, sizeof(bool)*VERTICES)) ;
	CUDA_SAFE_CALL(cudaMemcpy( d_graph_visited, graph_visited, sizeof(bool)*VERTICES, cudaMemcpyHostToDevice)) ;

	// Use a bool to check if the execution is over.
	bool *d_over;
	CUDA_SAFE_CALL(cudaMalloc( (void**) &d_over, sizeof(bool)));

	printf("Copied everything to GPU memory.\n");
	
	int num_of_blocks = 1;
	int num_of_threads_per_block = VERTICES;

	// Make execution parameters according to the number of nodes.
	// Distribute threads across multiple Blocks if necessary.
	if (VERTICES > MAX_THREADS_PER_BLOCK) {
		num_of_blocks = (int)ceil(VERTICES/(double)MAX_THREADS_PER_BLOCK); 
		num_of_threads_per_block = MAX_THREADS_PER_BLOCK; 
	}

	// Setup execution parameters
	dim3  grid( num_of_blocks, 1, 1);
	dim3  threads( num_of_threads_per_block, 1, 1);

	int k = 0;
	printf("Start traversing the tree.\n");
	bool over;
	
	// Call the Kernel untill all the elements of Frontier are not false
	// Create the cuda events
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	// Record event on the default stream
	cudaEventRecord(start, 0);
	
	// cudaPrintfInit ();
	do {
		// if no thread changes this value then the loop stops
		over = false;
		cudaMemcpy( d_over, &over, sizeof(bool), cudaMemcpyHostToDevice) ;
		
		Kernel<<< grid, threads, 0 >>>( d_graph,d_edge, d_graph_mask, d_updating_graph_mask, d_graph_visited);
		// check if kernel execution generated and error
		
		Kernel2<<< grid, threads, 0 >>>( d_graph_mask, d_updating_graph_mask, d_graph_visited, d_over);
		// check if kernel execution generated and error
		
		cudaMemcpy( &over, d_over, sizeof(bool), cudaMemcpyDeviceToHost) ;
		k++;
	} while(over);

	// cudaPrintfDisplay (stdout, true);
	// cudaPrintfEnd ();

	// Stop and destroy the timer
	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_gpu, start, stop);
	printf("\nGPU time: %f (msec)\n", elapsed_gpu);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	printf("Kernel executed: %d times\n",k); 
		
	// Cleanup memory
	free( graph_nodes);
	free( graph_edge);
	free( graph_mask);
	free( updating_graph_mask);
	free( graph_visited);
	cudaFree(d_graph);
	cudaFree(d_edge);
	cudaFree(d_graph_mask);
	cudaFree(d_updating_graph_mask);
	cudaFree(d_graph_visited);
}

void populate_random(Node* graph_nodes, int* graph_edge, bool *graph_mask, bool *updating_graph_mask, bool *graph_visited, bool *h_graph_visited) {
	int i = 0;
	int len;

	for (i = 0; i < VERTICES; i++) {
		// GPU transfer
		graph_nodes[i].no_of_edges = (rand() % (EDGES)) + 1;

		if (i == 0) {
			graph_nodes[i].start= i;
			len = graph_nodes[i].no_of_edges;
			graph_edge = (int *) malloc(sizeof(int)*len);
			if ((graph_edge = (int *) malloc(sizeof(int) * len)) == NULL) {
				printf("Could not allocate memory for graph_edge : %d\n", i);
				exit(1);
			} 
		} else {
			graph_nodes[i].start = graph_nodes[i-1].start + graph_nodes[i-1].no_of_edges;
			len += graph_nodes[i].no_of_edges;
			graph_edge = (int *) realloc(graph_edge, sizeof(int)*len);
			if ((graph_edge = (int *) realloc(graph_edge, sizeof(int)*len)) == NULL) {
				printf("Could not reallocate memory for graph_edge : %d\n", i);
				exit(1);
			}
		}
		
		// printf("%d:\t", i);
		graph_mask[i] = false;
		updating_graph_mask[i] = false;
		graph_visited[i] = false;
		h_graph_visited[i] = false;
		for (j = graph_nodes[i].start; j < (graph_nodes[i].no_of_edges+graph_nodes[i].start); j++) {
			graph_edge[j] = rand() % VERTICES;
			// printf("%d, ", graph_edge[j]);
		}
	}
}

void populate_known(Node* graph_nodes, int* graph_edge, bool *graph_mask, bool *updating_graph_mask, bool *graph_visited, bool *h_graph_visited) {
	for (i = 0; i < VERTICES; i++) {
		// GPU transfer
		graph_nodes[i].no_of_edges = (rand() % (EDGES)) + 1;

		if (i == 0) {
			graph_nodes[i].start= i;
			len = graph_nodes[i].no_of_edges;
			graph_edge = (int *) malloc(sizeof(int)*len);
			if ((graph_edge = (int *) malloc(sizeof(int) * len)) == NULL) {
				printf("Could not allocate memory for graph_edge : %d\n", i);
				exit(1);
			} 
		} else {
			graph_nodes[i].start = graph_nodes[i-1].start + graph_nodes[i-1].no_of_edges;
			len += graph_nodes[i].no_of_edges;
			graph_edge = (int *) realloc(graph_edge, sizeof(int)*len);
			if ((graph_edge = (int *) realloc(graph_edge, sizeof(int)*len)) == NULL) {
				printf("Could not reallocate memory for graph_edge : %d\n", i);
				exit(1);
			}
		}
		
		// printf("%d:\t", i);
		graph_mask[i] = false;
		updating_graph_mask[i] = false;
		graph_visited[i] = false;
		h_graph_visited[i] = false;
		for (j = graph_nodes[i].start; j < (graph_nodes[i].no_of_edges+graph_nodes[i].start); j++) {
			graph_edge[j] = (j + i ) % VERTICES;
			// printf("%d, ", graph_edge[j]);
		}
	}
}

void bfs(Node* graph_nodes, int* graph_edge, int vertex, bool* visited) {
	// double-ended queue
	deque<int> q;

	visited[vertex] = true;
	q.push_back(vertex);

	while (!q.empty()) {
		vertex = q.front();
		// printf("At vertex: %d\n", vertex);
		q.pop_front();
		int i;

		for (i = 0; i < graph_nodes[vertex].no_of_edges; i++) {
			if (!visited[graph_edge[graph_nodes[vertex].start + i]]) {
				visited[graph_edge[graph_nodes[vertex].start + i]] = true;
				q.push_back(graph_edge[graph_nodes[vertex].start + i]);
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