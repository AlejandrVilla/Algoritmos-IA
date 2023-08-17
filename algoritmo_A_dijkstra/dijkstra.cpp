#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <tuple>
#include <bitset>
#include <iomanip>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <queue>
#include <sstream>
#include <numeric>      // accumulate
#include <fstream>
#define loop(a,b,c) for(int a=b ; a<c ; ++a)
#define vstr vector<string>
#define ll long long int
#define tiii tuple<int,int,int>
#define tdii tuple<double,int,int>
#define pii pair<int,int>
#define vtiii vector<tiii>
#define vtdii vector<tdii>
#define vpii vector<pii>
#define vvpii vector<vpii>
#define vi vector<int>
#define vvi vector<vi>
#define vd vector<double>
#define vbl vector<bool>
#define vvd vector<vd>
#define vll vector<ll>
#define vvll vector<vll>
#define qi queue<int>
#define sti stack<int>
#define pqpii priority_queue<pii,vpii,greater<pair<int,int>>>
#define mimii map<int,map<int,int>>
#define mistr map<int,string>
#define uomstri unordered_map<string,int>
#define mii map<int,int>
#define si set<int>
#define sc set<char>
#define sll set<ll>
#define vsi vector<si>
#define uomisi unordered_map<int,si>
#define msi multiset<int>
#define MAX 22
using namespace std;

enum{inf=INT_MAX};
vvpii adjacent;
vi dist;
pqpii PQ;
mistr leters;
int n;

void dijsktra(int org)
{
    dist[org] = 0;
    PQ.push({0,org}); // peso, vertice
    while(!PQ.empty())
    {
        auto e=PQ.top();
        PQ.pop();
        int w,u;
        w=e.first;
        u=e.second;
        if(w>dist[u]) continue;
        for(auto a:adjacent[u])     // vertice peso
        {   
            int v,d;
            v=a.first;
            d=a.second;
            if(dist[v] <= d+dist[u]) continue;
            dist[v] = d+dist[u];
            PQ.push({dist[v],v});
        }
    }
}

// funcion para graficar el arbol
void graficar(string dir)
{
    vvi visitados(adjacent.size(),vi(adjacent.size(),0));
    ofstream f(dir);
    f<<"graph G{";
    for(int u=0 ; u<adjacent.size() ; ++u)
    {
        for(auto e : adjacent[u])
        {
            int v=e.first;
            int w=e.second;
            if(!visitados[u][v])
            {
                visitados[u][v] = w;
                visitados[v][u] = w;
                f<<leters[u]<<" -- "<<leters[v]<<"[label="<<w<<"];\n";
            }
        }
    }  
    f<<"}";
    f.close();
    std::stringstream dir1(dir);
    std::string name, ext, aux;
    std::getline(dir1,aux,'.');
    name = aux;
    std::getline(dir1,aux,'.');
    ext = aux;
    // system(&(("dot "+ dir + " -o " + name + ".png -Tpng")[0]));
    // system(&((name + ".png")[0]));
    // system("dot grafo.dot -o grafo.png -Tpng");           //dot grafo.dot -o grafo.png -Tpng
    // system("grafo.png");
}

int main()
{
    // adjacent =  { 
    //                 {{1,2},{3,7},{2,6}},     // 0
    //                 {{0,2},{4,9},{3,3}},     // 1
    //                 {{0,6},{4,1}},           // 2
    //                 {{0,7},{4,5},{1,3}},     // 3
    //                 {{1,9},{2,1},{3,5}}      // 4
    //             };
    adjacent = {
                {{1,3},{2,2}},              // 0
                {{0,3},{3,5}},              // 1
                {{0,2},{3,1}},              // 2
                {{1,5},{2,1},{4,2},{5,4}},  // 3
                {{3,2},{5,3},{6,6}},        // 4
                {{3,4},{4,3},{6,5}},        // 5
                {{5,5},{4,6}}               // 6
    };
    leters.insert({0,"V"});
    leters.insert({1,"I"});
    leters.insert({2,"L"});
    leters.insert({3,"A"});
    leters.insert({4,"H"});
    leters.insert({5,"E"});
    leters.insert({6,"R"});
    n = adjacent.size();
    graficar("dijkstra.dot");
    dist.assign(n,inf);
    int org=0;
    dijsktra(org);
    for(int i=0 ; i<n ; ++i)
        cout<<"De "<<leters[org]<<" a "<<leters[i]<<" "<<dist[i]<<"\n";
    cout<<'\n';
}   