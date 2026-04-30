// COMSC-210 | Lab 34 | Christine Susic

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <climits>
using namespace std;

const int SIZE = 7;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }
    
    // --- Depth First Search (Recursive) ---
    void DFS(int v, vector<bool> &visited) {
        visited[v] = true;
        cout << v << " ";

        for (Pair neighbor : adjList[v]) {
            if (!visited[neighbor.first]) {
                DFS(neighbor.first, visited);
            }
        }
    }

    // --- Breadth First Search (Iterative) ---
    void BFS(int startNode) {
        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[startNode] = true;
        q.push(startNode);

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            cout << v << " ";

            for (Pair neighbor : adjList[v]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
    }
    
    // --- Dijkstra's Algorithm for Shortest Path ---
    void shortestPath(int startNode) {
        // Priority queue to store (distance, vertex) pairs
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        // Vector for distances, initialized to infinity
        vector<int> dist(SIZE, INT_MAX);

        pq.push(make_pair(0, startNode));
        dist[startNode] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    pq.push(make_pair(dist[v], v));
                }
            }
        }

        cout << "Shortest path from node " << startNode << ":" << endl;
        for (int i = 0; i < SIZE; i++) {
            if (dist[i] == INT_MAX)
                cout << startNode << " -> " << i << " : INF" << endl;
            else
                cout << startNode << " -> " << i << " : " << dist[i] << endl;
        }
    }

    // --- Prim's Algorithm for Minimum Spanning Tree ---
    void findMST(int startNode) {
        // Stores the edge used to reach each node: parent[dest] = src
        vector<int> parent(SIZE, -1); 
        // Stores the minimum weight to connect each node
        vector<int> key(SIZE, INT_MAX); 
        // Tracks nodes already included in the MST
        vector<bool> inMST(SIZE, false); 

        // Priority queue: <weight, vertex>
        priority_queue<Pair, vector<Pair>, greater<Pair>> pq;

        pq.push({0, startNode});
        key[startNode] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (inMST[u]) continue;
            inMST[u] = true;

            for (auto &neighbor : adjList[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

            // If v is not in MST and weight(u,v) is smaller than current key
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                pq.push({key[v], v});
                parent[v] = u;
            }
        }
    }

    // Print the resulting MST edges
    cout << "Minimum Spanning Tree edges:" << endl;
    for (int i = 0; i < SIZE; i++) {
        if (parent[i] != -1) {
            cout << "Edge from " << parent[i] << " to " << i;
            cout << " with capacity: " << key[i] << " units" << endl;
        }
    }
}
    // Print the graph's adjacency list
    void printGraph() {
        cout << "Bus Transit System:" << endl;
        cout << "================================" << endl;
        for (int i = 0; i < adjList.size(); i++) {
            cout << "Hub " << i;
            if (i == 0) cout << " (Central Facility)";
            if (i == 1) cout << " (North Suburban Depot)";
            if (i == 2) cout << " (West Industrial Park)";
            if (i == 3) cout << " (Downtown Center)";
            if (i == 4) cout << " (East Port Terminal)";
            if (i == 5) cout << " (Airport)";
            if (i == 6) cout << " (South Pier)";
            cout << " connects to:" << endl;
            
            if (adjList[i].empty()) {
                cout << "  → No bus available" << endl;
            }

            for (Pair v : adjList[i])
                cout << "  → Junction " << v.first 
                     << " (Transit Time: " << v.second << " min)" << endl;
            cout << endl;
        }
    }
    
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        //delete two nodes and change weights
        {0,1,11},{0,2,6},{2,6,4},{5,6,2},{4,5,7},{2,4,10},{2,5,7},
        //add six new nodes
        {0,4,9},{1,2,5},{2,3,3},{3,1,5},{3,4,7},{5,4,2}
        
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    
    cout << "\nDFS starting from vertex 0: " << endl;
    vector<bool> visited(SIZE, false);
    graph.DFS(0, visited);

    cout << "\nBFS starting from vertex 0: " << endl;
    graph.BFS(0);
    cout << endl;
    
    graph.shortestPath(0);
    graph.findMST(0);
    
    int choice;
    do {
        cout << "\n--- Water Distribution Network Menu ---" << endl;
        cout << "[1] Display water distribution network" << endl;
        cout << "[2] Check contaminant spread (BFS)" << endl;
        cout << "[3] Plan inspection route (DFS)" << endl;
        cout << "[4] Calculate shortest paths" << endl;
        cout << "[5] Find Minimum Spanning Tree" << endl;
        cout << "[0] Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                graph.printGraph();
                break;
            case 2:
                graph.BFS(0);
                break;
            case 3:
                graph.DFS(0, visited);
                break;
            case 4:
                graph.shortestPath(0);
                break;
            case 5:
                graph.findMST(0);
                break;
            case 0:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 0);
  
    return 0;
}