#include<bits/stdc++.h>
using namespace std;

ifstream ifile;
ofstream ofile;

class Vertice
{
public:
    int val;
    float key;
    int parent;
    bool inMst;
    vector<pair<int,int>> adj;
    Vertice()
    {
        val = -1;
        key = INT_MAX;
        parent =-1;
        inMst = false;
    }
    Vertice operator=(Vertice obj);
};

Vertice Vertice::operator=(Vertice obj)
{
    this->val =obj.val;
    this->key = obj.key;
    this->parent = obj.parent;
    this->inMst = obj.inMst;
    this->adj = obj.adj;
    return *this;
}

class Graph
{
public:
    int v ;
    vector<Vertice> graph;
    Graph(int v)
    {
        this->v = v;
        graph.resize(v);
    }
    void addEdge(int u,int v, int wt);
    void primMST();
};

void Graph::addEdge(int u,int v, int wt)
{
    graph[u].val = u;
    graph[v].val = v;
    graph[u].adj.push_back(make_pair(v,wt));
    graph[v].adj.push_back(make_pair(u,wt));
}

struct comparator
{
    bool operator()(const Vertice& obj1, const Vertice& obj2)
    {
        return obj1.key > obj2.key;
    }
};


void Graph::primMST()
{
    priority_queue<Vertice, vector<Vertice>, comparator> pq;
    int src = graph[0].val;
    graph[src].key = 0;
    pq.push(graph[src]);
    while(!pq.empty())
    {
        Vertice u = pq.top();
        pq.pop();
        if (u.inMst==true)
        {
            continue;
        }
        graph[u.val].inMst = true;
        for(int i =0; i<u.adj.size(); i++)
        {
            int v = u.adj[i].first;
            int wt = u.adj[i].second;
            if(!graph[v].inMst && wt<graph[v].key)
            {
                graph[v].key = wt;
                graph[v].parent = u.val;
                pq.push(graph[v]);
            }
        }
    }
    float totalWt = 0;
    for(int i =1; i<v; i++)
    {
        totalWt+=graph[i].key;
    }
    ofile<<"Total Weight :"<<totalWt<<endl;
    ofile<<"Root node :"<<src<<endl;
    for(int i =1; i<v; i++)
    {
        ofile<<graph[i].parent<<" "<<i<<endl;

    }
}

int main()
{
    ifile.open("input.txt");
    ofile.open("prim.txt");
    int n, m;
    ifile >> n >> m;
    Graph g(n);
    int u, v;
    float d;
    for (int i = 0; i < m; i++)
    {
        ifile>>u>>v>>d;
        g.addEdge(u,v,d);
    }
    g.primMST();
    ifile.close();
    ofile.close();
    return 0;
}
