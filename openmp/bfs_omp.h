#ifndef BFS_OMP_H
#define BFS_OMP_H

void bfs(int** graph, int *size, int vertex, int *visited);
void printGraph(int **graph);
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, elapsedTime;

#endif