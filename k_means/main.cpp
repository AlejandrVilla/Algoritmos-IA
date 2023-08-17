#include <bits/stdc++.h>
#define vd vector<double>
#define vi vector<int>
#define vvd vector<vd>
#define pdd pair<double,double>
#define vpdd vector<pdd>
#define mivi map<int,vi>
using namespace std;

int n=100,m=3;
vpdd table;      // x,y 
vvd dist_centroides(m,vd(n,0.0)); // distancia a los centroides 
vpdd centroides(m);         // centroides
vpdd new_centroides(m);     // centroides nuevos
mivi dist_min;              // id centroide, id de punto
vd d_min(n);                // dist minima

// calcula distancia entre dos puntos
double dist(pdd& a, pdd& b)
{
    return sqrt(pow(a.first-b.first,2)+pow(a.second-b.second,2));
}

// si dos vectores son iguales
bool iguales(vpdd& A, vpdd& B)
{
    for(int i=0 ; i<A.size() ; ++i)
        if(A[i]!=B[i]) return 0;
    return 1;
}

void print(vpdd& A)
{
    for(int i=0 ; i<A.size() ; ++i)
        cout<<A[i].first<<" "<<A[i].second<<'\n';
    cout<<'\n';
}

void print(vvd& A)
{
    for(int i=0 ; i<A.size() ; ++i)
    {
        cout<<A[i].size()<<'\n';
        for(int j=0 ; j<A[i].size() ; ++j)
            cout<<A[i][j]<<' ';
        cout<<'\n';
    }
    cout<<'\n';
}

void print(mivi& A)
{
    for(auto a: A)
    {
        cout<<a.second.size()<<'\n';
        for(int i=0 ; i<a.second.size() ; ++i)
            cout<<a.second[i]<<" ";
        cout<<'\n';
    }
    cout<<'\n';
}

void Kmeans()
{
    int c=0;
    do
    {
        centroides=new_centroides;
        cout<<"ITERACION "<<c+1<<'\n';
        // calculo de las distancias a cada centroide
        for(int i=0 ; i<m ; ++i)
            for(int j=0 ; j<n ; ++j)
                dist_centroides[i][j]=dist(table[j],centroides[i]);
        // menor distancia a un centroide
        for(int i=0 ; i<n ; ++i)
        {
            if(dist_centroides[0][i]<=dist_centroides[1][i] && dist_centroides[0][i]<=dist_centroides[2][i])
                dist_min[0].push_back(i);
            else if(dist_centroides[1][i]<dist_centroides[0][i] && dist_centroides[1][i]<dist_centroides[2][i])
                dist_min[1].push_back(i);
            else if(dist_centroides[2][i]<dist_centroides[0][i] && dist_centroides[2][i]<dist_centroides[1][i])
                dist_min[2].push_back(i);
            d_min[i]=min(dist_centroides[0][i],min(dist_centroides[1][i],dist_centroides[2][i]));
        }
        // nuevos centroides
        for(int i=0 ; i<m ; ++i)
        {
            double acx=0,acy=0;
            for(int j=0 ; j<dist_min[i].size() ; ++j)
            {
                acx+=table[dist_min[i][j]].first;
                acy+=table[dist_min[i][j]].second;
            }
            acx=(acx!=0.0)?acx/dist_min[i].size():centroides[i].first;
            acy=(acy!=0.0)?acy/dist_min[i].size():centroides[i].second;
            new_centroides[i]={acx,acy};
        }
        // TABLA
        int ancho_col=-15;
        int ancho_id=-4;
        printf("|%*s|%*s|%*s|%*s|%*s|%*s|%*s|\n",ancho_id,"ID",ancho_col,"X",ancho_col,"Y",ancho_col,"C1",ancho_col,"C2",ancho_col,"C3",ancho_col,"MIN");
        for(int i=0 ; i<n ; ++i)
        {
            printf("|%*d",ancho_id,i+1);
            printf("|%*f|%*f",ancho_col,table[i].first,ancho_col,table[i].second);
            for(int j=0 ; j<m ; ++j)
                printf("|%*f",ancho_col,dist_centroides[j][i]);
            printf("|%*f|\n",ancho_col,d_min[i]);
        }
        cout<<'\n';
        for(int i=0 ; i<m ; ++i)
            printf("|%s%*d|%*f|%*f|%*f|%*f|\n","C",ancho_id+1,i+1,ancho_col,centroides[i].first,ancho_col,centroides[i].second,ancho_col,new_centroides[i].first,ancho_col,new_centroides[i].second);
        cout<<'\n';
        c++;
        dist_min.clear();   // limpia el mapa de distancias minimas
    } while(!iguales(centroides,new_centroides) && c<=1000);
}

int main()
{
    freopen("input.txt","r",stdin);     // leer archivo con las coordenadas
    freopen("output.txt","w",stdout);   // escribir en el archivo las tablas
    while(n--)
    {
        double a,b;cin>>a>>b;
        table.push_back({a,b});
    }
    n=table.size();
    srand(time(NULL));
    // centroides
    for(int i=0 ; i<m ; ++i)
    {
        //leatorio = rand()%(HASTA-DESDE+1)+DESDE;
        double x,y;
        int minx,maxx,miny,maxy;
        minx=-10;
        maxx=10;
        miny=-12;
        maxy=0;
        x=double(rand()%(maxx - minx+1)+minx);
        y=double(rand()%(maxy - miny+1)+miny);
        centroides[i]={x,y};
    }
    new_centroides=centroides;
    Kmeans();
}