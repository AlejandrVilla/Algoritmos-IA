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

double LR = 0.001;  // learning rate
double e = 2.7182818284;
int max_it = 100000;   // max iteraciones
int n_train;    // dato que lee del txt de entrada
int n_test;     // dato que lee del txt de entrada
double intercept, slope;
double intercept_size, slope_size;
vpdd punt_train;
vpdd punt_test;

void print(vpdd& A)
{
    for(int i=0 ; i<A.size() ; ++i)
        cout<<A[i].first<<" "<<A[i].second<<'\n';
}

double h_w(double x)
{
    double z = intercept + slope * x;
    return 1/(1+pow(e,-z));
}

// Binary Cross Entropy
double setError(double x, double y, double y_)
{   
    return -(y*log(y_)+(1-y)*log(1-y_));
}

void gradiente_descendiente()
{
    int i=1;
    while(max_it--)
    {
        cout<<"Iteracion "<<i++<<'\n';
        cout<<"Pendiente anterior = "<<slope<<'\n';
        cout<<"Intercepto anterior = "<<intercept<<'\n';
        double sum_residual1 = 0, sum_residual2 = 0, error = 0;
        for(int i=0 ; i<n_train ; ++i)
        {
            sum_residual1 += h_w(punt_train[i].first) - punt_train[i].second;
            sum_residual2 += (h_w(punt_train[i].first) - punt_train[i].second) * punt_train[i].first;
            error += setError(punt_train[i].first, punt_train[i].second, h_w(punt_train[i].first) );
        }
        // mostrar error
        cout<<"Error = "<<error<<'\n'; 
        cout<<"Tasa de aprendizaje = "<<LR<<'\n';
        cout<<"Derivada pendiente = "<<sum_residual2<<'\n';
        cout<<"Derivada intercepto = "<<sum_residual1<<'\n';
        intercept_size = sum_residual1*LR;
        slope_size = sum_residual2*LR;
        // cout<<"step size intercept "<<intercept_size<<'\n';
        // cout<<"step size slope "<<slope_size<<'\n';

        intercept -= intercept_size;
        slope -= slope_size;
        cout<<"Nuevo pendiente = "<<slope<<'\n';
        cout<<"Nuevo intercepto = "<<intercept<<'\n';

        // if(abs(intercept_size) <= 0.001 && abs(slope_size) <= 0.001) break;
        cout<<'\n';
    }
}

void test()
{
    double ac=0;
    //cout<<n_test<<'\n';
    for(int i=0 ; i<n_test ; ++i)
    {
        double esperado = h_w(punt_test[i].first);
        cout<<"Dato "<<punt_test[i].first<<" = "<<h_w(punt_test[i].first)<<", aprovado estimado = "<<punt_test[i].second<<", ";
        if(esperado<0.5)
            esperado = floor(esperado);
        else
            esperado = ceil(esperado);
        if(esperado == punt_test[i].second)
        {
            cout<<"correcto\n";
            ac++;
        }
        else
            cout<<"incorrecto\n";
    }
    double porc = (ac/double(n_test))*100;
    cout<<setprecision(2)<<fixed<<porc<<"%\n";
}

int main()
{
    // ios::sync_with_stdio(false);
    // cin.tie(0); cin.tie(0);
    // freopen("input.txt","r",stdin);
    freopen("input2.txt","r",stdin);
    freopen("output.txt","w",stdout);
    cin>>n_train>>n_test;
    double a,b;
    while(n_train-- && cin>>a>>b)
        punt_train.push_back({a,b});
    while(n_test-- && cin>>a>>b)
        punt_test.push_back({a,b});
    n_train=punt_train.size();
    n_test=punt_test.size();

    cout<<"Alejandro Villa Herrera\n";
    intercept = -2.1789;
    slope = -0.9874;
    cout<<"Pendiente inicial = "<<slope<<'\n';
    cout<<"Intercepto inicial = "<<intercept<<'\n';
    cout<<"Tasa de aprendizaje = "<<LR<<'\n';
    cout<<"Cantidad de Iteraciones = "<<max_it<<'\n';
    cout<<"Umbral = "<<0.5<<'\n';
    cout<<'\n';
    cout<<"Datos de entrenamiento\n";
    print(punt_train);
    cout<<"Datos de test\n";
    print(punt_test);
    //cout<<"\nGradiente Descendiente\n";
    cout<<'\n';
    // entrenamiento
    gradiente_descendiente();
    cout<<'\n';
    cout<<"intercepto "<<intercept<<'\n';
    cout<<"slope "<<slope<<'\n';
    cout<<'\n';
    
    // test
    cout<<"Test\n";
    test();
    
    freopen("Regresion.txt","w",stdout);
    cout<<intercept<<'\n';
    cout<<slope<<'\n';
    // for(int i=0 ; i<punt_train.size() ; ++i)
    //     cout<<punt_train[i].first<<" ";
    for(int i=0 ; i<punt_test.size() ; ++i)
        cout<<punt_test[i].first<<" ";
    cout<<'\n';
    // for(int i=0 ; i<punt_train.size() ; ++i)
    //     cout<<punt_train[i].second<<" ";
    for(int i=0 ; i<punt_test.size() ; ++i)
        cout<<punt_test[i].second<<" ";
    cout<<'\n';
}