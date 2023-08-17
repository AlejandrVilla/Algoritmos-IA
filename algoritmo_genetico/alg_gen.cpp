#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <tuple>
#include <bitset>
#include <iomanip>
#include <ctime>
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
#define vc vector<char>
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
#define mci map<char,int>
#define sstr set<string>
#define si set<int>
#define sc set<char>
#define sll set<ll>
#define vsi vector<si>
#define vstr vector<string>
#define pstrstr pair<string,string>
#define uomisi unordered_map<int,si>
#define msi multiset<int>
#define MAX 22
using namespace std;

/*
    Parámetros:
    - Cantidad de Individuos: 10
    - Cantidad de Genes por Individuo: 8
    - Selección por Ruleta
    - Probabilidad de Cruzamiento: 0.7
    - Cruzamiento OBX
    - Probabilidad de Mutación: 0.2
    - Mutación de Intercambio
    - Cantidad de Iteraciones: 50
*/

int n=10,m;
vstr poblacion(n);
vstr matin_pool(n);
vvi dist;   // matriz de distancias
mci letra;  // letra, pos
vi fitness(n);  // fitness de cada individio
vd inv(n);      // calculo inv propor a cada individuo
vd prob(n);     // prob de cada individuo
vd prob_ac(n);  // prob acumulada
vd aletorios(n); // numeros generados aleatoriamente para escoger padres
double prob_cruz = 0.7;
double prob_mut = 0.2;
vstr padres(2); 
vstr hijos(2);

// obtiene el fitness de cada individuo segun la matriz
int get_fitness(string& ind)
{
    int ac=0;
    loop(i,0,ind.size()-1)
        ac+=dist[letra[ind[i]]][letra[ind[i+1]]];
    return ac;
}

// obtiene dos padres aleatorios de la mating_pool
void get_padres()
{
    // srand( time(nullptr) );
    // int num = rand() % (max - min) + min;  
    int pos1 = 0;
    int pos2 = 0;
    while(pos1 == pos2)
    {
        pos1 = rand() % (n - 0) + 0;
        pos2 = rand() % (n - 0) + 0;
    }
    cout<<pos1<<" "<<pos2<<'\n';
    padres[0] = matin_pool[pos1];
    padres[1] = matin_pool[pos2];
}

// genera la mating_pool
void get_matin_pool()
{
    // srand( time(nullptr) );
    for(int i=0 ; i<n ; )
    {
        double num = (rand() % 1000)/1000.0;
        // cout<<num*1000<<" ";
        // num = rand() % (max - min) + min;
        double lim_inf, lim_sup;
        loop(j,0,n)
        {
            lim_inf=(j==0)? 0 : lim_sup;
            lim_sup=prob_ac[j];
            if(num>=lim_inf && num<=lim_sup)
            {
                matin_pool[i] = poblacion[j];
                aletorios[i] = num;
                // cout<<num<<" limites: "<<lim_inf<<" "<<lim_sup<<'\n';
                i++;
                break;
            }
        }
    }
}

// genera dos hijos por cruzamiento de dos padres, prob=0.7
void cruzamiento()
{

    hijos[0]=padres[0];
    hijos[1]=padres[1];
    vi pos(4,0);
    // srand( time(nullptr) );
    while(pos[0]==pos[1] || pos[0]==pos[2] || pos[0]==pos[3] ||
          pos[1]==pos[2] || pos[1]==pos[3] || pos[2]==pos[3])
    {
        pos[0]=rand()%(m);
        pos[1]=rand()%(m);
        pos[2]=rand()%(m);
        pos[3]=rand()%(m);
    }
    sort(pos.begin(),pos.end());
    cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" "<<pos[3]<<'\n';
    for(int i=0 ; i<2 ; ++i)
    {
        // srand( time(nullptr) );
        double num = (rand() % 100)/100.0;
        if(num<prob_cruz)
        {
            vc cromosomas = {padres[i][pos[0]],padres[i][pos[1]],padres[i][pos[2]],padres[i][pos[3]]};
            int k=0;
            for(int j=0 ; j<padres[!i].size() ; ++j)
            {
                if(find(cromosomas.begin(),cromosomas.end(),padres[!i][j])!=cromosomas.end())
                    hijos[i][pos[k++]]=padres[!i][j];
            }
        }
    }
    cout<<hijos[0]<<" - "<<hijos[1]<<'\n';
}

// realiza la mutacion de un individuo, prob = 0.2
void mutacion()
{
    for(int i=0 ; i<2 ; ++i)
    {    
        double num = (rand()%100)/100.0;
        if(num<=prob_mut)
        {
            cout<<"mutacion "<<i+1<<'\n';
            vi pos(2,0);
            while(pos[0] == pos[1])
            {
                pos[0] = rand()%(m);
                pos[1] = rand()%(m);
            }
            cout<<"posicion: "<<pos[0]<<" - "<<pos[1]<<" => ";
            swap(hijos[i][pos[0]],hijos[i][pos[1]]);
            cout<<hijos[i]<<'\n';
        }
        else cout<<"sin mutacion "<<i+1<<'\n';
    }
}

// inicializa el fitness, calcula prob y prob acumulada
void ini()
{
    double ac=0.0;
    // fitnes, prob inver prop
    loop(i,0,n)
    {
        int fit = get_fitness(poblacion[i]);
        fitness[i] = fit;
        inv[i] = 1.0/fit;
        ac+=fit;
    }
    // prob
    loop(i,0,n)
    {
        prob[i] = fitness[i]/ac;
        cout<<i+1<<")\t"<<poblacion[i]<<"\t"<<setprecision(5)<<fixed<<fitness[i]<<"\t\t"<<inv[i]<<"\t\t"<<prob[i]<<'\n';
    }
    // prob acumulada
    prob_ac[0] = prob[0];
    loop(i,1,n)
    {
        prob_ac[i]= prob[i]+prob_ac[i-1];
        // cout<<prob_ac[i]<<'\n';
    }
}

// algoritmo genetico
void alg_gen()
{   
    int cond=50;
    int it=1;
    // calculo del fitness, prob inver prop, prob y prob acumulada
    cout<<"\nCalcular la Aptitud para cada Individudo\n";
    ini(); 
    while(cond--)
    {
        srand( time(nullptr) );
        // creacion de la matin pool
        cout<<"\n**** Iteracion "<<it++<<" ****\n";
        cout<<"Creacion de la Mating Pool\n";
        get_matin_pool();
        loop(i,0,n)
            cout<<setprecision(3)<<fixed<<aletorios[i]<<" =>\t"<<i<<"\t=>\t"<<matin_pool[i]<<'\n';
        int k=n/2;
        // srand( time(nullptr) );
        int i=0;
        while(k--)
        {
            // dos padres aleatorios
            cout<<"\nSeleccion de padres\n";
            get_padres();
            cout<<padres[0]<<" "<<padres[1]<<'\n';
            cout<<"cruzamiento\n";
            // cruzamiento
            cruzamiento();
            // mutacion
            mutacion();
            poblacion[i++]=hijos[0];
            poblacion[i++]=hijos[1];
        }
        cout<<"\nNueva Poblacion\n";
        for(int i=0 ; i<n ; ++i)
            cout<<i+1<<")\t"<<poblacion[i]<<'\n';
        cout<<"\nCalcular el Fitness\n";
        ini(); 
    }
}


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    cout<<"Parámetros:\n";
    cout<<"- Cantidad de Individuos: 10\n";
    cout<<"- Cantidad de Genes por Individuo: 8\n";
    cout<<"- Selección por Ruleta\n";
    cout<<"- Probabilidad de Cruzamiento: 0.7\n";
    cout<<"- Cruzamiento OBX\n";
    cout<<"- Probabilidad de Mutación: 0.2\n";
    cout<<"- Mutación de Intercambio\n";
    cout<<"- Cantidad de Iteraciones: 50\n\n";
    loop(i,0,n)
        cin>>poblacion[i];
    cout<<"Poblacion Inicial\n";
    loop(i,0,n)
        cout<<poblacion[i]<<'\n';

    letra['V'] = 0;
    letra['I'] = 1;
    letra['L'] = 2;
    letra['A'] = 3;
    letra['H'] = 4;
    letra['E'] = 5;
    letra['R'] = 6;
    letra['O'] = 7;   

    dist = {//   V  I  L  A  H  E  R   O
                {0, 1, 3, 5, 7, 9, 11, 13},     // V
                {1, 0, 4, 6, 8, 10,12, 14},     // I
                {3, 4, 0, 1, 2, 3, 5, 8},       // L
                {5, 6, 1, 0, 2, 5, 6, 7},       // A
                {7, 8, 2, 2, 0, 4, 7, 9},       // H
                {9, 10,3, 5, 4, 0, 1, 2},       // E
                {11,12,5, 6, 7, 1, 0, 3},       // R
                {13,14,8, 7, 9, 2, 3, 0}        // O
    };
    m=poblacion[0].size();
    alg_gen();
    cout<<"\ntermino\n";
}   