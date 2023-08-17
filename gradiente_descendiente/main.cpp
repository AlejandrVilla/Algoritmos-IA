#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector<vi>
#define vd vector<double>
#define vvd vector<vd>
#define pii pair<int,int>
#define pdd pair<double,double>
#define vpii vector<pii>
#define vpdd vector<pdd>
using namespace std;

int n;
double LR = 0.001;  // learning rate
int max_it = 2000;   // max iteraciones
double intercept, slope;
double intercept_size, slope_size;
vpdd puntos;

void print(vpdd& A)
{
    for(int i=0 ; i<A.size() ; ++i)
        cout<<A[i].first<<" "<<A[i].second<<'\n';
}

double h_w(double x)
{
    return intercept + slope * x;
}

void gradiente_descendiente()
{
    int i=1;
    while(max_it--)
    {
        cout<<"Iteracion "<<i++<<'\n';
        cout<<"Anterior intercepto = "<<intercept<<'\n';
        cout<<"Anterior pendiente = "<<slope<<'\n';
        double sum_residual1 = 0, sum_residual2 = 0;
        for(int i=0 ; i<n ; ++i)
        {
            sum_residual1 += -2*( puntos[i].second - h_w(puntos[i].first) );
            sum_residual2 += -2*( puntos[i].second - h_w(puntos[i].first) )*puntos[i].first;
        }
        cout<<"Derivada intercepto = "<<sum_residual1<<'\n';
        cout<<"Derivada pendiente = "<<sum_residual2<<'\n';
        cout<<"Tasa de aprendisaje = "<<LR<<'\n';
        intercept_size = sum_residual1*LR;
        slope_size = sum_residual2*LR;
        // cout<<"step size intercept "<<intercept_size<<'\n';
        // cout<<"step size slope "<<slope_size<<'\n';

        intercept -= intercept_size;
        slope -= slope_size;
        cout<<"Nuevo intercepto = "<<intercept<<'\n';
        cout<<"Nuevo pendiente = "<<slope<<'\n';

        // if(abs(intercept_size) <= 0.001 && abs(slope_size) <= 0.001) break;
        cout<<'\n';
    }
}

int main()
{
    // ios::sync_with_stdio(false);
    // cin.tie(0); cin.tie(0);
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    double a,b;
    while(cin>>a>>b)
        puntos.push_back({a,b});
    n=puntos.size();
    print(puntos);
    cout<<"\nGradiente Descendiente\n";
    cout<<'\n';
    intercept = 100;
    slope = 50;
    cout<<"Intercepto inicial = "<<intercept<<'\n';
    cout<<"Pendiente inicial = "<<slope<<'\n';
    cout<<"Tasa de aprendizaje = "<<LR<<'\n';
    cout<<'\n';
    gradiente_descendiente();
    cout<<'\n';
    cout<<"intercepto "<<intercept<<'\n';
    cout<<"slope "<<slope<<'\n';
    
    freopen("Regresion.txt","w",stdout);
    cout<<intercept<<'\n';
    cout<<slope<<'\n';
    for(int i=0 ; i<puntos.size() ; ++i)
        cout<<puntos[i].first<<" ";
    cout<<'\n';
    for(int i=0 ; i<puntos.size() ; ++i)
        cout<<puntos[i].second<<" ";
    cout<<'\n';
}