DELHI METRO RAIL APP
This is NOT an ANDROID Application!

This is a simple C++ program that will take information (name) of the source station and the destination station, of Delhi Metro, from the user and display the fare and shortest metro route to reach the destination. It will also have a metro map for commuters' better navigation.

The idea is implemented using Graph and Heap data structures. The graph has nodes and edges. Nodes represent a metro station that will contain certain information regarding that station like its name, its metro corridor, and the lines it connects. Edges (the connection between two nodes) represent the distance between the two stations, and the cost of each edge will be equal to the distance between the two connecting stations (nodes).

By using different algorithms like Dijkstra, breadth-first search, depth-first search, etc., the shortest path between the source station and the destination station is determined, and accordingly, the fare is calculated based on the total distance between the two stations. Finally, the metro route between the two stations and the total fare is displayed.

graph.cpp contains all the major code and heap.cpp contains the heap implementation.
