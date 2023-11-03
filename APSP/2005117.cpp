#include<bits/stdc++.h>
using namespace std;

#define INF 100000


vector<vector<int>> ESP(vector<vector<int>>&M,vector<vector<int>>&W)
{
    int n = M.size();
    vector<vector<int>> M_new(n,vector<int>(n,INF));
       for(int i =0;i<n;i++)
    {
        for(int j =0;j<n;j++)
        {
            for(int k = 0;k<n;k++)
            {
                    M_new[i][j] = min(M_new[i][j],(M[i][k]+W[k][j]));
            }
        }
    }
   return M_new;

}

vector<vector<int>> Mat_mul(vector<vector<int>>&W)
{
    int n = W.size();
    vector<vector<int>> apsp = W;
    for(int e = 2;e<=n-1;e++)
    {
        apsp = ESP(apsp,W);
    }
    return apsp;
}

vector<vector<int>> Fast_Mat_mul(vector<vector<int>>&W)
{
    int n = W.size();
    vector<vector<int>> apsp = W;
    for(int i = 1;i<n;i*=2)
    {
        apsp = ESP(apsp,apsp);
    }
    return apsp;
}



vector<vector<int>> Floyd_Warshall(vector<vector<int>>&W)
{
    int n = W.size();
    for(int i =0;i<n;i++)
    {
        for(int j =0;j<n;j++)
        {
            for(int k = 0;k<n;k++)
            {
                    W[j][k] = min(W[j][k],(W[j][i]+W[i][k]));
            }
        }
    }
    return W;
}


int main()
{
    int n,m,u,v,wt;
    cin>>n>>m;
    vector<vector<int>>W(n,vector<int>(n));
    vector<vector<int>>apsp;
    for(int i = 0;i<n;i++)
    {
        for(int j =0;j<n;j++)
        {
            if(i==j)
                W[i][j] = 0;
            else
                W[i][j] = INF;
        }

    }

    for(int i =0; i<m; i++)
    {
        cin>>u>>v>>wt;
            W[u-1][v-1] = wt;
    }

    apsp = Fast_Mat_mul(W);
    //apsp = Mat_mul(W);
    //apsp = Floyd_Warshall(W);

    cout<<"Shortest distance"<<endl;
    for(int i = 0;i<n;i++)
    {
        for(int j =0;j<n;j++)
        {
            if(apsp[i][j]==INF)
                cout<<"INF"<<"  ";
            else
                cout<<apsp[i][j]<<"  ";
        }
        cout<<endl;
    }
    return 0;
}

