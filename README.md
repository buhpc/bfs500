# Graph 500 Implementation
[Graph500](http://www.graph500.org/referencecode)

### BFS
- First kernel constructs an undirected graph.
- Second kernel performs a breadth-first search of the graph.

### Benchmark
Steps for Graph 500 are provided [here](http://www.graph500.org/specifications).

1. Generate the edge list.
2. Construct a graph from the edge list (timed, kernel 1).
3. Randomly sample 64 unique search keys with degree at least one, not counting self-loops.
4. For each search key:
  - Compute the parent array (timed, kernel 2).
  - Validate that the parent array is a correct BFS search tree for the given search tree.
5. Compute and output performance information.

### Cluster Specs

| Cluster       | CPU                                   | Cores | Accelerator  |
|---------------|---------------------------------------|-------|--------------|
| 6 x86 nodes   | Intel(R) Xeon (R) CPU 5140 @ 2.33 GHz | 4     | None         |

