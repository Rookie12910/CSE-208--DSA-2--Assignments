#include<bits/stdc++.h>
#define Max_wt 200
#define Min_wt 50

using namespace std;

class Graph
{

public:
    int V;
    vector<vector<int>> graph;
    Graph(int n)
    {
        this->V = n;
        graph.assign(V, vector<int>(V, 0));
    }

    void add_edge(int u, int v, int wt)
    {
        graph[u][v] = wt;
        graph[v][u] = wt;
    }

    int get_wt(int u, int v) const
    {
        return graph[u][v];
    }

    int get_size()
    {
        return V;
    }

};


Graph createRandomGraph(int V)
{
    Graph random_graph(V);
    srand(time(0));
    for (int i = 0; i < V; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            int weight = rand() % (Max_wt - Min_wt + 1) + Min_wt;
            random_graph.add_edge(i, j, weight);
        }
    }
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            for (int k = 0; k < V; k++)
            {
                if (random_graph.get_wt(i,j) > random_graph.get_wt(i,k) + random_graph.get_wt(k,j))
                {
                    random_graph.add_edge(i,j,(random_graph.get_wt(i,k) + random_graph.get_wt(k,j)));
                }
            }
        }
    }

    return random_graph;
}


int calculateTourLength(vector<int>& tour, Graph& G)
{
    int length = 0;
    int V = G.get_size();

    for (int i = 0; i < V; i++)
    {
        int u = tour[i];
        int v = tour[(i + 1) % V];
        length += G.get_wt(u, v);
    }

    return length;
}


int tsp(vector<vector<int>>& graph, int start, int mask, vector<vector<int>>& dp, vector<vector<int>>& parent)
{
    int n = graph.size();
    if (mask == (1 << n) - 1)
    {
        return graph[start][0];
    }
    if (dp[start][mask] != -1)
    {
        return dp[start][mask];
    }

    int minCost = INT_MAX;
    int next_node = -1;

    for (int next = 0; next < n; next++)
    {
        if (!(mask & (1 << next)))
        {
            int newMask = mask | (1 << next);
            int cost = graph[start][next] + tsp(graph, next, newMask, dp, parent);
            if (cost < minCost)
            {
                minCost = cost;
                next_node = next;
            }
        }
    }

    dp[start][mask] = minCost;
    parent[start][mask] = next_node;

    return minCost;
}


vector<int> exact_TSP(Graph& G)
{
    int n = G.get_size();
    vector<vector<int>> dp(n, vector<int>(1 << n, -1));
    vector<vector<int>> parent(n, vector<int>(1 << n, -1));
    int start = 0;
    int mask = 1 << start;

    tsp(G.graph, start, mask, dp, parent);

    vector<int> tour;
    tour.push_back(start);
    while (tour.size() < n)
    {
        int Next = parent[start][mask];
        tour.push_back(Next);
        mask |= (1 << Next);
        start = Next;
    }

    return tour;
}


vector<int> PrimMst(Graph &G)
{
    int n = G.get_size();
    vector<int> parent(n, -1);
    vector<int> key(n, INT_MAX);
    vector<bool> inMST(n, false);
    int src = 0;
    key[src] = 0;

    for (int i = 0; i < (n - 1); i++)
    {
        int u;
        int minKey = INT_MAX;
        for (int v = 0; v < n; v++)
        {
            if (!inMST[v] && key[v] < minKey)
            {
                u = v;
                minKey = key[v];
            }
        }
        inMST[u] = true;
        for (int v = 0; v < n; v++)
        {
            int wt = G.get_wt(u, v);
            if (!inMST[v] && wt < key[v])
            {
                parent[v] = u;
                key[v] = wt;
            }
        }
    }
    return parent;
}


vector<int> Eulerian_tour(Graph& G)
{
    int n = G.get_size();
    vector<int> tour;
    stack<int> stack;
    int src = 0;
    stack.push(src);

    while (!stack.empty())
    {
        int u = stack.top();
        int v;
        for (v = 0; v < n; v++)
        {
            if (G.get_wt(u, v) > 0)
            {
                break;
            }
        }

        if (v == n)
        {
            stack.pop();
            tour.push_back(u);
        }
        else
        {

            G.graph[u][v] = 0;
            G.graph[v][u] = 0;
            stack.push(v);
        }
    }
    reverse(tour.begin(), tour.end());
    return tour;
}



vector<int> metric_approximation_TSP(Graph& G)
{
    int n = G.get_size();
    vector<int> MST = PrimMst(G);
    Graph Eul_Graph(n);
    for (int i = 1; i < n; i++)
    {
        int u = MST[i];
        int v = i;
        int wt = G.get_wt(u, v);
        Eul_Graph.add_edge(u, v, wt);
        Eul_Graph.add_edge(v, u, wt);
    }
    vector<int> eulerianTour = Eulerian_tour(Eul_Graph);
    vector<bool> visited(n, false);
    vector<int> C;
    for (int v : eulerianTour)
    {
        if (!visited[v])
        {
            C.push_back(v);
            visited[v] = true;
        }
    }
    return C;
}


int main()
{

    int n;
    cout<<"Enter the number of test cases : ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        int V = 20;
        Graph G = createRandomGraph(V);


        vector<int> optimumTour = exact_TSP(G);
        vector<int> approxTour = metric_approximation_TSP(G);

        int optimumLength = calculateTourLength(optimumTour, G);

        int approxLength = calculateTourLength(approxTour, G);

        cout<<"Optimum tour : "<<optimumLength<<"    approximated tour : "<<approxLength<<endl;

        double approximationRatio = static_cast<double>(approxLength) / optimumLength;
        cout <<"Ratio : " << approximationRatio << endl;
    }

    return 0;
}


