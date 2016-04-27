#ifndef BFS_SERIAL_H
#define BFS_SERIAL_H

void bfs(int** graph, int *size, int vertex, int *visited);
void printGraph(int **graph);
double timeInSeconds(struct timespec *t);
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, elapsedTime;

#endif