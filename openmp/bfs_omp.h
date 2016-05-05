#ifndef BFS_OMP_H
#define BFS_OMP_H

void bfs(int** graph, int *size, int vertex, int *visited);
void printGraph(int **graph);

double timeInSeconds(struct timespec *t);
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, elapsedTime;

void populate_known(int **graph, int *size, const int vertices, const int edges);
void populate_random(int **graph, int *size, const int vertices, const int edges);
void old_populate_random(int **graph, int *size, const int vertices, const int edges);

#endif
