#ifndef BFS_PTHREAD_H
#define BFS_PTHREAD_H

void populate_random(int **graph, int *size, int vertices, int edges);
void populate_known(int **graph, int *size, int vertices, int edges);
void bfs(int vertex);
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, elapsedTime;

#endif
