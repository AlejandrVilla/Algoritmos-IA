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
enum{novisitado=0, visitado=1, inf=INT32_MAX};

mistr cities;   // mapa que asigna un numero a cada ciudad
vvi adjacent;   // lista de adyacencia
vi visited;     // ciudades visitadas para dfs
vi lvl;         // nivel de las ciudades en el arbol para bfs
int n;          // total de ciudades

// dfs recursivo
void dfs(int u, int k, bool& f)
{
    cout<<cities[u]<<" ";
    if(u==k)
        f=1;
    visited[u]=visitado;
    for(auto v:adjacent[u])
        if(visited[v]==novisitado)
            dfs(v,k,f);
}   

// dfs con uso de una pila
void dfs_stack(int org, int k, int& l, ofstream& file)
{
    file<<"DFS - Nodo seleccionado "<<cities[k]<<'\n';
    file<<"Alejandro Villa Herrera\n\n";
    vi frontier;
    vi reached;
    frontier.push_back(org);
    // inicio dfs
    sti S;      // pila
    S.push(org);
    lvl[org]=0;
    int i=1;
    while(!S.empty())
    {
        int u=S.top(); S.pop();
        cout<<cities[u]<<" ";
        file<<"Paso "<<i++<<'\n';
        file<<"frontier = {";
        for(auto a:frontier)
            file<<cities[a]<<" ";
        file<<"}\n";
        file<<"reached = {";
        for(auto a:reached)
            file<<cities[a]<<" ";
        file<<"}\n";
        file<<"____________________________________________\n\n";
        frontier.pop_back();
        reached.push_back(u);
        for(auto v:adjacent[u])
        {
            if(v==k) 
            {   
                l=lvl[u]+1;
                file<<cities[u]<<"->"<<cities[k]<<'\n';
                return;
            }
            if(lvl[v]!=inf) continue;
            S.push(v);
            lvl[v]=lvl[u]+1;
            frontier.push_back(v);
        }
    }
    cout<<'\n';
}

// bfs con uso de una cola
void bfs(int org, int k, int& l, ofstream& file)
{
    file<<"BFS - Nodo seleccionado "<<cities[k]<<'\n';
    file<<"Alejandro Villa Herrera\n\n";
    vi frontier;
    vi reached;
    frontier.push_back(org);
    reached.push_back(org);
    // inicio bfs
    qi Q;       // cola
    Q.push(org);
    lvl[org]=0;
    int i=1;
    while(!Q.empty())
    {
        int u=Q.front(); Q.pop();
        cout<<cities[u]<<" ";
        file<<"Paso "<<i++<<'\n';
        file<<"frontier = {";
        for(auto a:frontier)
            file<<cities[a]<<" ";
        file<<"}\n";
        frontier.erase(frontier.begin());
        file<<"reached = {";
        for(auto a:reached)
            file<<cities[a]<<" ";
        file<<"}\n";
        file<<"____________________________________________\n\n";
        for(auto v:adjacent[u])
        {
            if(v==k)
            {
                l=lvl[u]+1; 
                file<<cities[u]<<"->"<<cities[k]<<'\n';
                return;
            }
            if(lvl[v]!=inf) continue;
            Q.push(v);
            lvl[v]=lvl[u]+1;
            frontier.push_back(v);
            reached.push_back(v);
        }
    }
    cout<<'\n';
}

// funcion para graficar el arbol
void graficar(string dir)
{
    vvi visitados(adjacent.size(),vi(adjacent.size(),0));
    ofstream f(dir);
    f<<"graph G{";
    for(int u=0 ; u<adjacent.size() ; ++u)
    {
        for(auto v : adjacent[u])
        {
            if(!visitados[u][v])
            {
                visitados[u][v] = 1;
                visitados[v][u] = 1;
                f<<cities[u]<<" -- "<<cities[v]<<";\n";
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
    system(&(("dot "+ dir + " -o " + name + ".png -Tpng")[0]));
    system(&((name + ".png")[0]));
    // system("dot grafo.dot -o grafo.png -Tpng");           //dot grafo.dot -o grafo.png -Tpng
    // system("grafo.png");
}

int main()
{
    cities.insert(make_pair(0,"Lim"));
    cities.insert(make_pair(1,"Anc"));
    cities.insert(make_pair(2,"Jun"));
    cities.insert(make_pair(3,"Ica"));
    cities.insert(make_pair(4,"Lali")); 
    cities.insert(make_pair(5,"SanM"));
    cities.insert(make_pair(6,"Pasco"));
    cities.insert(make_pair(7,"Cusco"));
    cities.insert(make_pair(8,"Ayac"));
    cities.insert(make_pair(9,"Aqp"));
    cities.insert(make_pair(10,"Cajam"));
    cities.insert(make_pair(11,"Lamb"));
    cities.insert(make_pair(12,"Amz"));
    cities.insert(make_pair(13,"Lor"));
    cities.insert(make_pair(14,"Huan"));

    // llenando lista de adyacencia
    adjacent =  { 
                    {1, 2, 3},      // 0
                    {0, 4, 5},      // 1
                    {0, 6, 7},      // 2
                    {0, 8, 9},      // 3
                    {1, 10, 11},    // 4
                    {1, 12, 13},    // 5
                    {2, 14},        // 6
                    {2},            // 7
                    {3},            // 8
                    {3},            // 9
                    {4},            // 10
                    {4},            // 11
                    {5},            // 12
                    {5},            // 13
                    {6}             // 14
                };
        
    graficar("grafo.dot");
    n = adjacent.size();
    int find=14;

    // dfs
    visited.resize(n,novisitado);
    bool encontro=0;
    cout<<"DFS\n";
    dfs(0,find,encontro);
    cout<<'\n';
    if(encontro)
        cout<<cities[find]<<" encontrado\n";
    else
        cout<<cities[find]<<" no encontrado\n";

    // dfs con pila
    ofstream f1("dfs.txt");
    cout<<"DFS con pila\n";
    lvl.assign(n,inf);
    int nivel=-1;
    dfs_stack(0,find,nivel,f1);
    cout<<'\n';
    f1.close();
    if(nivel!=-1)
        cout<<cities[find]<<" encontrado en el nivel "<<nivel<<'\n';
    else
        cout<<cities[find]<<" no encontrado\n";

    // bfs
    lvl.assign(n,inf);
    nivel=-1;
    cout<<"BFS\n";
    ofstream f2("bfs.txt");
    bfs(0,find,nivel,f2);
    cout<<'\n';
    f2.close();
    if(nivel!=-1)
        cout<<cities[find]<<" encontrado en el nivel "<<nivel<<'\n';
    else
        cout<<cities[find]<<" no encontrado\n";
}