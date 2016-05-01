#ifndef BFS_MAIN_H
#define BFS_MAIN_H


void populate_known(int **graph, int *size, const int vertices, const int edges);
void populate_random(int **graph, int *size, const int vertices, const int edges); 
struct timespec diff(struct timespec start, struct timespec end);
struct timespec time1, time2, elapsedTime;

#endif
