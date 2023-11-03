#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <cstdlib>
#include <utility>
using namespace std;

ifstream ifile;
ofstream ofile;

class Vertex
{
public:
    int val;
    int dis;
    bool inSp;
    int parent;
    vector<pair<int,int>> adj;
    Vertex()
    {
        val = -1;
        parent = -1;
        dis = INT_MAX;
        inSp = false;
    }
    Vertex operator=(Vertex obj);
};

Vertex Vertex::operator=(Vertex obj)
{
    this->val =obj.val;
    this->dis = obj.dis;
    this->inSp = obj.inSp;
    this->adj = obj.adj;
    return *this;
}

class Edge
{
public:
    int src;
    int des;
    int wt;
    Edge() {}
};

class Graph
{
public:
    int v ;
    vector<Vertex> graph;
    Graph(int v)
    {
        this->v = v;
        graph.resize(v);
    }
    void addEdge(int u,int v, int wt);
    void Bellman_Ford(int s,int d, vector<Edge>& edges);
    void Dijkstra(int s, int d);
};

void Graph::addEdge(int u,int v, int wt)
{
    graph[u].val = u;
    graph[v].val = v;
    graph[u].adj.push_back(make_pair(v,wt));
}

struct comparator
{
    bool operator()(const Vertex& obj1, const Vertex& obj2)
    {
        return obj1.dis > obj2.dis;
    }
};




void Graph::Bellman_Ford(int s, int d, vector<Edge>& edges)
{
    for(int i =0; (i<graph.size()-1); i++)
    {
        graph[s].dis = 0;
        for (int j = 0; j < edges.size(); j++)
        {
            int src = edges[j].src;
            int des = edges[j].des;
            int wt = edges[j].wt;
            if (graph[src].dis != INT_MAX && (graph[src].dis + wt) < graph[des].dis)
            {
                graph[des].dis = graph[src].dis + wt;
                graph[des].parent = src;
            }

        }
    }

    //Checking for negative cycle
    int currDis = graph[d].dis;
    for (int j = 0; j < edges.size(); j++)
        {
            int src = edges[j].src;
            int des = edges[j].des;
            int wt = edges[j].wt;
            if (graph[src].dis != INT_MAX && (graph[src].dis + wt) < graph[des].dis)
            {
                graph[des].dis = graph[src].dis + wt;
                graph[des].parent = src;
            }

        }
    ofile<<"Bellman Ford Algorithm:"<<endl;
    if(currDis>graph[d].dis)
    {
        ofile<<"Negative weight cycle present"<<endl<<endl;
        return;
    }


    ofile<<graph[d].dis<<endl;
    vector<int>path;
    int index = d;
    while(index!=s)
    {
        path.push_back(index);
        index = graph[index].parent;
    }
    path.push_back(index);
    for(int i = path.size()-1; i>0; i--)
    {
        ofile<<path[i]<<" -> ";
    }
    ofile<<path[0]<<endl<<endl;
}




void Graph::Dijkstra(int s,int d)
{
    priority_queue<Vertex, vector<Vertex>, comparator> pq;
    int src = s;
    graph[src].dis = 0;
    pq.push(graph[src]);
    while(!pq.empty())
    {
        Vertex u = pq.top();
        pq.pop();
        if (u.inSp==true)
        {
            continue;
        }
        graph[u.val].inSp = true;
        for(int i =0; i<u.adj.size(); i++)
        {
            int v = u.adj[i].first;
            int wt = abs(u.adj[i].second);
            if(!graph[v].inSp && (wt+graph[u.val].dis)<graph[v].dis)
            {
                graph[v].dis = wt+graph[u.val].dis;
                graph[v].parent = u.val;
                pq.push(graph[v]);
            }
        }
    }
    ofile<<"Dijkstra Algorithm:"<<endl<<graph[d].dis<<endl;
    vector<int>path;
    int index = d;
    while(index!=s)
    {
        path.push_back(index);
        index = graph[index].parent;
    }
    path.push_back(index);
    for(int i = path.size()-1; i>0; i--)
    {
        ofile<<path[i]<<" -> ";
    }
    ofile<<path[0]<<endl;
}





int main()
{
    ifile.open("input.txt");
    ofile.open("output.txt");
    int n, m;
    ifile >> n >> m;
    Graph g(n);
    vector<Edge> edges(m);
    int u, v,w;
    for (int i = 0; i < m; i++)
    {
        ifile>>u>>v>>w;
        g.addEdge(u,v,w);
        edges[i].src = u;
        edges[i].des = v;
        edges[i].wt = w;
    }
    int s,d;
    ifile>>s>>d;
    g.Bellman_Ford(s,d,edges);
    //refreshing the changed values
    for(int i = 0;i<n;i++)
    {
        g.graph[i].dis = INT_MAX;
        g.graph[i].parent = -1;
    }
    g.Dijkstra(s,d);
    ifile.close();
    ofile.close();
    return 0;
}


