#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector<vi>
#define qi queue<int>   
#define si set<int>
#define vd vector<double>
#define vvd vector<vd>
#define pdd pair<double,double>
#define vpdd vector<pdd>
#define mivd map<int,vd>
using namespace std;

enum{novisitado=0, visitado=1};

int n, min_N=2;
double epsilon = 2.0;
vi visit;
vvi C_Res;
vi clustered;
vpdd puntos;
vd dist_curr;
double p_curr;

double dist(pdd& A, pdd& B)
{
    return sqrt(pow(A.first-B.first,2)+pow(A.second-B.second,2));
}

void get_neighbors(si& A, pdd& P)
{
    for(int i=0 ; i<n ; ++i)
    {
        double dist_P = dist(puntos[i], P);
        dist_curr[i] = dist_P;
        if(dist_P < epsilon)
            A.insert(i);
    }
}

void DBSCAN()
{
    int c=1;
    for(int i=0 ; i<n ; ++i)
    {
        if(visit[i] == novisitado)
        {
            si N,N_;   
            cout<<"Puntos no visitados: ";
            for(int i=0 ; i<n ; ++i)
                if(visit[i] == novisitado)
                    cout<<i+1<<" ";
            cout<<'\n';
            vi C;
            visit[i] = visitado;
            cout<<"Punto visitado: "<<i+1<<'\n';
            get_neighbors(N, puntos[i]);
            if(N.size() < min_N) continue;
            cout<<"Lista de vecinos: ";
            for(auto it=N.begin() ; it!=N.end() ; ++it)
                if(*it != i)
                    cout<<(*it)+1<<" ";
            cout<<'\n';
            C.push_back(i);
            clustered[i] = visitado;
            cout<<"Grupo "<<c<<": ";
                for(int a:C)
                    cout<<a+1<<' ';
            cout<<"\n\n";
            for(auto it=N.begin() ; it!=N.end() ; )
            {
                int j=*it;
                ++it;
                if(j==i) continue;
                N.erase(j);
                if(visit[j] == novisitado)
                {
                    cout<<"Analizando: "<<j+1<<'\n';
                    visit[j] = visitado;
                    get_neighbors(N_, puntos[j]);
                    if(N_.size() >= min_N)
                        for(auto it2=N_.begin() ; it2!=N_.end(); ++it2)
                            if(visit[*it2] == novisitado)
                                N.insert(*it2);
                    cout<<"Lista vecinos: ";
                    for(auto a:N)
                        if(a!=j && a!=i)
                            cout<<a+1<<" ";
                    cout<<'\n';
                    cout<<"Lista vecinos auxiliar: ";
                    for(auto a:N_)
                        if(a!=j)
                            cout<<a+1<<" ";
                    cout<<'\n';
                    if(clustered[j] == novisitado)
                    {
                        clustered[j] = visitado;
                        C.push_back(j);
                    }
                    cout<<"Grupo "<<c<<": ";
                    for(int a:C)
                        cout<<a+1<<' ';
                    cout<<"\n\n";
                }
                else
                {
                    cout<<"Analizando: "<<j+1<<'\n';
                    cout<<"Grupo "<<c<<": ";
                    for(int a:C)
                        cout<<a+1<<' ';
                    cout<<"\n\n";
                }
            }
            C_Res.push_back(C);
            c++;
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(0); cin.tie(0);
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    n = 100;
    cout<<"Parametros\n";
    cout<<"Epsilon "<<epsilon<<'\n';
    cout<<"MinPts "<<min_N<<'\n';
    cout<<"\nPuntos\n";
    while(n--)
    {
        double a,b;cin>>a>>b;
        puntos.push_back({a,b});
    }
    n=puntos.size();
    visit.assign(n,novisitado);
    dist_curr.assign(n,0.0);
    clustered.assign(n,novisitado);
    for(int i=0 ; i<n ; ++i)
        cout<<i+1<<". ("<<puntos[i].first<<", "<<puntos[i].second<<")\n";
    cout<<'\n';
    DBSCAN();

    cout<<"Grupos formados\n";
    for(int i=0 ; i<C_Res.size() ; ++i)
    {
        cout<<"C"<<i+1<<": ";
        for(int j=0 ; j<C_Res[i].size() ; ++j)
            cout<<C_Res[i][j]+1<<" ";
        cout<<'\n';
    }
}
