// Depth First Search (DFS) and Breadth First Search (BFS) on a graph
// ---------------------------------------------------------------------
// Data structure choice: ADJACENCY LIST.

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

class Graph
{
public:
    // Construct a graph with `numNodes` nodes, each holding an integer
    explicit Graph(int numNodes)
    {
        adjList.resize(numNodes);
        nodeValue.resize(numNodes);
        for (int i = 0; i < numNodes; ++i) nodeValue[i] = i * 10;
    }

    void addEdge(int u, int v, bool directed = false)
    {
        adjList[u].push_back(v);
        if (!directed) adjList[v].push_back(u);
    }

    int numNodes() const { return static_cast<int>(adjList.size()); }

    // Depth First Search (iterative)
    void dfs(int start) const
    {
        std::vector<bool> visited(numNodes(), false);
        std::stack<int> stack;
        stack.push(start);

        std::cout << "DFS order: ";
        while (!stack.empty())
        {
            int node = stack.top();
            stack.pop();

            if (visited[node]) continue;
            visited[node] = true;
            std::cout << nodeValue[node] << " ";

            // Push neighbours in reverse so the traversal visits them
            // in the same left-to-right order as a DFS dictates.
            for (auto it = adjList[node].rbegin(); it != adjList[node].rend(); ++it)
            {
                if (!visited[*it]) stack.push(*it);
            }
        }
        std::cout << "\n";
    }

    // Breadth First Search
    void bfs(int start) const
    {
        std::vector<bool> visited(numNodes(), false);
        std::queue<int> queue;

        visited[start] = true;
        queue.push(start);

        std::cout << "BFS order: ";
        while (!queue.empty())
        {
            int node = queue.front();
            queue.pop();
            std::cout << nodeValue[node] << " ";

            for (int neighbour : adjList[node])
            {
                if (!visited[neighbour])
                {
                    visited[neighbour] = true;
                    queue.push(neighbour);
                }
            }
        }
        std::cout << "\n";
    }

private:
    std::vector<std::list<int>> adjList; // adjacency list: adjList[u] = neighbours of u
    std::vector<int> nodeValue;          // value stored at each node
};

int main() {
    // Example undirected graph (8 nodes)
    Graph g(8);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 6);
    g.addEdge(6, 7);

    std::cout << "Graph has " << g.numNodes() << " nodes.\n\n";

    g.dfs(0);
    g.bfs(0);

    return 0;
}
