#ifndef BFS_OMP_H
#define BFS_OMP_H

void bfs(int** graph, int *size, int vertex, int *visited);
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, time3, time4, elapsedTime;
double timeInSeconds(struct timespec *t);

#endif