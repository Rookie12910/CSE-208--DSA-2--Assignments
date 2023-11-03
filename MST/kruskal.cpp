#include<bits/stdc++.h>

using namespace std;

class Edge
{
public:
    int src;
    int des;
    float wt;
    Edge() {}
    Edge operator=(Edge obj);
};

Edge Edge::operator=(Edge obj)
{
    this->src = obj.src;
    this->des = obj.des;
    this->wt = obj.wt;
    return *this;
}

bool compareByWt(const Edge& ob1, const Edge& ob2)
{
    return ob1.wt < ob2.wt;
}

int Find(vector<int>& parent, int i)
{
    if (parent[i] == i)
        return i;
    return Find(parent, parent[i]);
}

void updateParent(vector<int>& parent, int i, int j)
{
    int root1 = Find(parent, i);
    int root2 = Find(parent, j);
    parent[root2] = root1;
}

vector<Edge> kruskal(vector<Edge>& edges, int n)
{
    sort(edges.begin(), edges.end(), compareByWt);
    vector<Edge> ans;
    vector<int> parent(n);
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }

    int i = 0;
    while (i < edges.size())
    {
        int srcRoot = Find(parent, edges[i].src);
        int desRoot = Find(parent, edges[i].des);
        if (srcRoot != desRoot)
        {
            ans.push_back(edges[i]);
            updateParent(parent, srcRoot, desRoot);
        }
        i++;
    }
    return ans;
}

int main()
{
    ifstream ifile;
    ofstream ofile;
    ifile.open("input.txt");
    ofile.open("kruskal.txt");
    int n, m;
    ifile >> n >> m;
    vector<Edge> edges(m);
    int u, v;
    float d;
    for (int i = 0; i < m; i++)
    {
        ifile >> u >> v >> d;
        edges[i].src = u;
        edges[i].des = v;
        edges[i].wt = d;
    }

    vector<Edge> ans = kruskal(edges, n);
    int totalWt = 0;

    for (int i = 0; i < ans.size(); i++)
    {
        totalWt+=ans[i].wt;
    }
    ofile<<"Total weight : "<<totalWt<<endl;
    for (int i = 0; i < ans.size(); i++)
    {
        ofile << ans[i].src << " " << ans[i].des << endl;
    }
    ifile.close();
    ofile.close();
    return 0;
}

