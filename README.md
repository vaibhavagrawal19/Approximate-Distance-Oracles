# Instructions to run `main.cpp`
The file main.cpp has to be compiled first usign the following command:
```
g++ main.cpp
```
The generated file will be named `a.out` by default. 
To run this file, the following command has to be executed:
```
./a.out <graph-file>
```

The `graph-file` is the name of the file where the graph is stored. 

## Important
The program only recognizes the graph file if it is in a specific format, which is mentioned below:

* The first line of the file is ignored, and it may contain a comment describing the graph, or may even be empty.
* The seocnd line of the file should contain the number of nodes (`n`) and the number of edges (`m`) in the graph, separated by a space.
* The following `m` lines should contain at least two integers `a` and `b` (separated by a space), which represents that there is an edge between nodes `a` and `b` in the graph. Note that `a` and `b` both must be less than `n` for the program to work correctly.

A sample graph-file has been attached for reference. 

# Output format
The first two lines of output contains the number of nodes and the number of edges in the graph (separated by a space).

The following line contains the time (in seconds) taken by the distance oracle for the pre-computation
The next line contains the time (in seconds) taken by the Floyd-Warshall algorithm for pre-computing the distances between all pairs of nodes in the graph.

# Testing the validity of the oracle
A certain portion of the code (commented at the end) confirms the validity of the distnace oracle. It asserts that the answer given by the distance oracle is stretched by at most 2k - 1 times the actual shortest path computed by the Floyd Warshall algorithm.