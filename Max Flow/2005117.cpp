#include<bits/stdc++.h>
#define INF INT_MAX
using namespace std;


bool already_eliminated(vector<int>&w, int pw,vector<int>&res_teams)
{
    bool flag = false;
    int most_win = *max_element(w.begin(),w.end());
    int indx = find(w.begin(),w.end(),most_win) - w.begin();
    if(most_win> pw)
    {
        flag = true;
        res_teams.push_back(indx);
    }
    return flag;
}


int bfs(int s, int t, int n,vector<int>& parent,vector<vector<int>>& graph)
{
    fill(parent.begin(),parent.end(),-1);
    parent[s] = -2;
    queue<int> q;
    q.push(s);
    int cf = INF;
    while(!q.empty())
    {
        int u  = q.front();
        q.pop();
        for(int v =0; v<n; v++)
        {
            if(v!=u && graph[u][v]>0 && parent[v]==-1)
            {
                parent[v] = u;
                cf = min(cf,graph[u][v]);
                if(v==t)
                {
                    return cf;
                }
                q.push(v);
            }
        }
    }
    return 0;
}


void res_teams_finder(int s,int n,vector<bool>& visited,vector<vector<int>>& graph)
{
    fill(visited.begin(),visited.end(),false);
    visited[s] = true;
    queue<int> q;
    q.push(s);
    while(!q.empty())
    {
        int u  = q.front();
        q.pop();
        for(int v =0; v<n; v++)
        {
            if(v!=u && graph[u][v]>0 && visited[v]==false)
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

int Edmond_Karp(int s, int t, int n,vector<vector<int>>& graph)
{
    int max_flow = 0;
    vector<int> parent(n,-1);
    while(true)
    {
        int cf = bfs(s,t,n,parent,graph);
        if(cf==0) break;
        max_flow+=cf;
        int v = t;
        while(v!=s)
        {
            int u = parent[v];
            graph[u][v]-=cf;
            graph[v][u]+=cf;
            v = u;
        }
    }
    return max_flow;
}

bool flow_network_check(int n,int this_team, vector<int>&w,vector<int>&r,vector<vector<int>>&g,vector<int>&res_teams)
{
    bool flag = false;
    int total_games = ((n-1)*(n-2))/2;
    int Size = 2 + total_games + n;
    vector<vector<int>> graph(Size,vector<int>(Size,0));
    int game_index = 1;
    for(int i =0; i<n; i++)
    {
        if(i==this_team) continue;
        for(int j =i+1; j<n; j++)
        {
            if(j==this_team) continue;
            graph[0][game_index] = g[i][j];
            graph[game_index][total_games+1+i] = INF;
            graph[game_index][total_games+1+j] = INF;
            game_index++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (i == this_team) continue;
        graph[total_games+1+i][Size-1] = max(w[this_team]+r[this_team] - w[i],0);
    }
    int max_flow = Edmond_Karp(0,Size-1,Size,graph);
    vector<bool>visited(Size,false);
    for(int k =1; k<=total_games; k++)
    {
        if(graph[0][k]>0)
        {
            flag = true;
            break;
        }
    }
    if(flag)
    {
        res_teams_finder(0,Size,visited,graph);
        for(int i =0; i<n; i++)
        {
            if(visited[total_games+1+i]==true && find(res_teams.begin(),res_teams.end(),i)==res_teams.end())
            {
                res_teams.push_back(i);
            }
        }
    }

    return flag;
}



void explain_reasons(vector<int> &res_teams, vector<int> &w, vector<vector<int> > &g, vector<string> &teams)
{
    int n = res_teams.size();
    int total_won = 0,f2f = 0;
    for (int i = 0; i < n; i++) total_won += w[res_teams[i]];

    cout << teams[res_teams[0]];
    for (int i = 1; i < n-1; i++) cout << ", " << teams[res_teams[i]];
    if (n > 1) cout << " and " << teams[res_teams[n-1]] << " have ";
    else cout << " has ";
    cout << "won a total of " << total_won << " games"<<endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = i+1; j < n; j++)
        {
            f2f += g[res_teams[i]][res_teams[j]];
        }
    }
    cout << "They play each other " << f2f << " times"<<endl;

    float avg = float((total_won + f2f))/n;

    cout << "So on average, each of the teams in this group wins ";
    cout << total_won+f2f << '/' << n << " = " << avg << " games."<<endl;
}



int main()
{
    int n;
    cin>>n;
    vector<string>teams(n);
    vector<int>w(n);
    vector<int>l(n);
    vector<int>r(n);
    vector<vector<int>>g(n,vector<int>(n));
    vector<int> eliminated_teams;
    vector<int> res_teams;
    vector<vector<int>>reasons;
    for(int i = 0; i<n; i++)
    {
        cin>>teams[i]>>w[i]>>l[i]>>r[i];
        for(int j =0; j<n; j++)
        {
            cin>>g[i][j];
        }
    }

    for(int i =0; i<n; i++)
    {
        bool eliminated = false;
        eliminated = already_eliminated(w,w[i]+r[i],res_teams);
        if(eliminated)
        {
            eliminated_teams.push_back(i);
            reasons.push_back(res_teams);
            res_teams.clear();
            continue;
        }
        eliminated = flow_network_check(n,i,w,r,g,res_teams);
        if(eliminated)
        {
            eliminated_teams.push_back(i);
            reasons.push_back(res_teams);
            res_teams.clear();
        }

    }
    cout<<endl;
    for(int i =0; i<eliminated_teams.size(); i++)
    {
        cout<<teams[eliminated_teams[i]]<<" is eliminated"<<endl<<"They can win at most "<<w[eliminated_teams[i]]<<" + "<<r[eliminated_teams[i]]<<" = "<<w[eliminated_teams[i]]+r[eliminated_teams[i]]<<" games"<<endl;
        explain_reasons(reasons[i], w, g, teams);
        cout <<endl;
    }
    return 0;
}

