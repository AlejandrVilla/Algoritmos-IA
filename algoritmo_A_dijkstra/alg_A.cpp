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
#define vvvi vector<vvi>
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
#define mmii multimap<int,int>
#define mivvi map<int,vvi>
#define sstr set<string>
#define si set<int>
#define sc set<char>
#define sll set<ll>
#define vsi vector<si>
#define uomisi unordered_map<int,si>
#define msi multiset<int>
#define MAX 22
using namespace std;

vi ud = {0,-1,0,1};
vi lr = {-1,0,1,0};

struct RompCab
{
    int g; // profundidad
    int h; // mal colocados
    int f; 
};

map<int,RompCab> sol;   // indice, inf matrix
vi par;                 // parents
int curr_tree=0;        // indices del nodo actual
mmii open_list;         // f, indice
mmii closed_list;       // f, indice

vvi matrix_res;         // matriz solucion
vvi matrix;             // matriz actual
sstr vis;               // matrices ya usadas
mivvi matrices;         // mapa de matrices: indice, matriz
mivvi matrices_sol;     // mapa de matrices sol: indice, matriz

int goal;               // objetivo
int i_father;           // indice del nodo padre

// devuelve el padre de un nodo
int parent(int a)
{
    return par[a];
}

// une nodo con su padre
void Union(int a, int b)
{
    par[b] = a;
}

// devuelve numero de coincidencias
int get_h(vvi& matrix_aux)
{
    int cont=0;
    for(int i=0 ; i<matrix_aux.size() ; ++i)
        for(int j=0 ; j<matrix_aux.size() ; ++j)
            if(matrix_aux[i][j] != matrix_res[i][j] && matrix_res[i][j] != -1)
                cont++;
    return cont;
}

void print(vvi& M)
{
    for(int i=0 ; i<M.size() ; ++i)
    {
        for(int j=0 ; j<M.size() ; ++j)
            cout<<M[i][j]<<' ';
        cout<<'\n';
    }
    cout<<'\n';
}

// genera key segun los elementos de una matriz
string get_key(vvi& M)
{
    string key;
    for(int i=0 ; i<M.size() ; ++i)
        for(int j=0 ; j<M.size() ; ++j)
            key+=to_string(M[i][j]);
    return key;
}

int get_suc()
{
    int cont=0;     // cant de nodos sucesores
    int e_i=0;      // i para fila vacia
    int e_j=0;      // j para columna vacia
    for(int i=0 ; i<matrix.size() ; ++i)
        for(int j=0 ; j<matrix.size() ; ++j)
            if(matrix[i][j] == -1)
                e_i=i,e_j=j;
    
    for(int i=0 ; i<4 ; ++i)
    {
        if(e_i+ud[i]<0 || e_i+ud[i]>=matrix.size()) continue;
        if(e_j+lr[i]<0 || e_j+lr[i]>=matrix.size()) continue;
        // para cambiar espacios vacios de la matriz
        vvi matrix_aux = matrix;    
        swap(matrix_aux[e_i+ud[i]][e_j+lr[i]], matrix_aux[e_i][e_j]);
        // key de la matriz
        string key = get_key(matrix_aux);
        // si la key se repite, nodo repetido, continua
        if(find(vis.begin(), vis.end(), key) != vis.end()) continue;
        vis.insert(key);
        /*print(matrix_aux);*/
        // aumenta profundidad
        int g=sol[i_father].g+1;      
        // cantidad de errores 
        int h=get_h(matrix_aux);       
        int f=g+h;
        // aumenta para el indice de un nuevo nodo
        curr_tree++;
        // padre del nodo
        par.push_back(curr_tree);
        Union(i_father,curr_tree);
        // inf de nuevo nodo
        RompCab nodo;
        nodo.g=g;
        nodo.h=h;
        nodo.f=f;
        // nueva inf del nodo segun su indice en el arbol
        sol.insert({curr_tree,nodo});
        // inserta matriz a la lista de todas las matrices generadas
        matrices.insert({curr_tree,matrix_aux});
        // inserta a openlist
        open_list.insert({f,curr_tree});
        if(h==goal) return -1;             // respuesta, termina
        cont++;
    }
    return cont;
}

void solve()
{
    cout<<"Alejandro Antonio Villa Herrera\n";
    cout<<"Search A*\n";
    cout<<"-----------------------------------------------\n";
    cout<<"Estado Inicial\n";
    cout<<"[";
    loop(i,0,matrix.size())
        loop(j,0,matrix.size())
        {
            if(matrix[i][j]==-1)
                cout<<" ";
            else cout<<matrix[i][j];
            if(!(i==matrix.size()-1 && j==matrix.size()-1))
                cout<<", ";
        }
    cout<<"]\n";
    cout<<"-----------------------------------------------\n";
    cout<<"Estado Final\n";
    cout<<"[";
    loop(i,0,matrix_res.size())
        loop(j,0,matrix_res.size())
        {
            if(matrix_res[i][j]==-1)
                cout<<" ";
            else cout<<matrix_res[i][j];
            if(!(i==matrix_res.size()-1 && j==matrix_res.size()-1))
                cout<<", ";
        }
    cout<<"]\n";
    cout<<"-----------------------------------------------\n";

    goal = 0;
    i_father = 0;
    // datos de un nodo
    int g=0;
    int h=get_h(matrix);
    int f=h+g;
    RompCab nodo;
    nodo.g=g;
    nodo.h=h;
    nodo.f=f;

    // inicio
    sol.insert({curr_tree,nodo});
    open_list.insert({f,curr_tree});
    par.push_back(0);
    matrices.insert({curr_tree,matrix});
    string key = get_key(matrix);
    vis.insert(key);

    // impresion de datos
    //openlist
    vvi matrix_aux;
    cout<<"open:\n";
    for(auto it=open_list.begin() ; it!=open_list.end() ; ++it)
    {
        matrix_aux=matrices[it->second];
        cout<<"[";
        loop(i,0,matrix_aux.size())
            loop(j,0,matrix_aux.size())
            {
                if(matrix_aux[i][j]==-1)
                    cout<<" ";
                else cout<<matrix_aux[i][j];
                if(!(i==matrix_aux.size()-1 && j==matrix_aux.size()-1))
                    cout<<", ";
            }
        cout<<"]; ";
        cout<<"g="<<sol[it->second].g<<"; h="<<sol[it->second].h<<"; f="<<sol[it->second].f<<'\n';
    }
    //closedlist
    cout<<"closed:\n";
    for(auto it=closed_list.begin() ; it!=closed_list.end() ; ++it)
    {
        matrix_aux=matrices[it->second];
        cout<<"[";
        loop(i,0,matrix_aux.size())
            loop(j,0,matrix_aux.size())
            {
                if(matrix_aux[i][j]==-1)
                    cout<<" ";
                else cout<<matrix_aux[i][j];
                if(!(i==matrix_aux.size()-1 && j==matrix_aux.size()-1))
                    cout<<", ";
            }
        cout<<"]; ";
        cout<<"g="<<sol[it->second].g<<"; h="<<sol[it->second].h<<"; f="<<sol[it->second].f<<'\n';
    }
    cout<<"-----------------------------------------------\n";
    
    int k=1;
    while(!open_list.empty())
    {
        cout<<"iteracion "<<k++<<"\n";
        // indice del nodo
        int ind_nodo = open_list.begin()->second;   
        // indice del padre
        i_father = ind_nodo;
        //cout<<"in "<<i_father<<'\n';
        // obtiene matriz con menor f
        matrix=matrices[ind_nodo];
        
        // inserta en open list nodo raiz
        open_list.erase(open_list.begin());
        // raiz siempre en la solucion final
        matrices_sol.insert({i_father,matrix});
        // procedimiento
        int mov = get_suc();
        // inserta en closed list
        closed_list.insert({sol[ind_nodo].f,ind_nodo});

        // fase de impresion de datos
        //openlist
        vvi matrix_aux;
        cout<<"open:\n";
        for(auto it=open_list.begin() ; it!=open_list.end() ; ++it)
        {
            matrix_aux=matrices[it->second];
            cout<<"[";
            loop(i,0,matrix_aux.size())
                loop(j,0,matrix_aux.size())
                {
                    if(matrix_aux[i][j]==-1)
                        cout<<" ";
                    else cout<<matrix_aux[i][j];
                    if(!(i==matrix_aux.size()-1 && j==matrix_aux.size()-1))
                        cout<<", ";
                }
            cout<<"]; ";
            cout<<"g="<<sol[it->second].g<<"; h="<<sol[it->second].h<<"; f="<<sol[it->second].f<<'\n';
        }

        // closedlist
        cout<<"closed:\n";
        for(auto it=closed_list.begin() ; it!=closed_list.end() ; ++it)
        {
            matrix_aux=matrices[it->second];
            cout<<"[";
            loop(i,0,matrix_aux.size())
                loop(j,0,matrix_aux.size())
                {
                    if(matrix_aux[i][j]==-1)
                        cout<<" ";
                    else cout<<matrix_aux[i][j];
                    if(!(i==matrix_aux.size()-1 && j==matrix_aux.size()-1))
                        cout<<", ";
                }
            cout<<"]; ";
            cout<<"g="<<sol[it->second].g<<"; h="<<sol[it->second].h<<"; f="<<sol[it->second].f<<'\n';
        }
        cout<<"-----------------------------------------------\n";
    
        if(mov == -1) 
        {
            ind_nodo = open_list.begin()->second;
            matrices_sol.insert({ind_nodo,matrices[ind_nodo]});
            // cout<<"indice "<<ind_nodo<<" f "<<sol[ind_nodo].f<<'\n';
            return;
        }
        
    }
}

void graficar(string dir)
{
    ofstream F(dir);
    F<<"digraph G{\n";
    F<<"center=true\n";
    F<<"node [shape=record];\n";
    for(auto a : matrices)
    {
        F<<a.first<<" [label=\" ";
        for(int f=0 ; f<a.second.size() ; ++f)
        {
            F<<"{";
            for(int c=0 ; c<a.second.size() ; ++c)
            {
                if(a.second[c][f]!=-1)
                    F<<a.second[c][f];
                else
                    F<<" ";
                if(c!=a.second.size()-1)
                    F<<"|";
            }
            F<<"}";
            if(f!=a.second.size()-1)
                F<<"|";
        }
        F<<"\", fontcolor=orange, fontsize=\"15pt\"];\n";
    }

    for(int i=1 ; i<par.size() ; ++i)
    {
        F<<parent(i)<<" -> "<<i<<"\n";
    }

    F<<"}";
    F.close();
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
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    freopen("output.txt","w",stdout);
    matrix_res ={
                {1,2,3},
                {8,-1,4},
                {7,6,5}
    };
    matrix ={
                {2,8,3},
                {1,6,4},
                {7,-1,5}
    };
    solve();
    
    // comprobacion
    // matriz, inf y su padre
    for(auto a:matrices)
    {
        cout<<"indice "<<a.first<<'\n';
        cout<<"g="<<sol[a.first].g<<" h="<<sol[a.first].h<<" f="<<sol[a.first].f<<'\n';
        cout<<"padre de "<<a.first<<" es "<<parent(a.first)<<'\n';
        print(matrices[a.first]);
    }

    graficar("algoritmo_A.dot");
}