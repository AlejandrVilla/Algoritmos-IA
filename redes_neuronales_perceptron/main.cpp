#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector<vi>
#define vd vector<double>
using namespace std;

vvi info = {
    {0,0,0},
    {0,1,0},
    {1,0,0},
    {1,1,1}
};
double bias = 0.2;
double TA = 0.2;
vd X(3), W(3);


int f(double x)
{
    if(x>0) return 1;
    return 0;
}

double f_escalon()
{
    double ac=0;
    for(int i=0 ; i<X.size() ; ++i)
        ac+=X[i]*W[i];
    return f(ac);
}

int error(int a, int b)
{
    return a-b;
}

void ajuste(int a, int b)
{
    int err = error(a,b);
    cout<<"Error = "<<err<<'\n';
    cout<<"Tasa de aprendizaje = "<<TA<<'\n';
    for(int i=0 ; i<W.size() ; ++i)
        W[i] = W[i] + (TA*X[i]*err);
}

int main()
{
    freopen("output.txt","w",stdout);
    cout<<"ALEJANDRO ANTONIO VILLA HERRERA\n";
    cout<<"REDES NEURONALES - PERCEPTRON\n";
    cout<<"CONJUNTO DE DATOS\n";
    cout<<"X1\tX2\ty\n";
    for(int f=0 ; f<info.size() ; ++f)
    {
        for(int c=0 ; c<info[f].size() ; ++c)
            cout<<info[f][c]<<'\t';
        cout<<'\n';
    }
    cout<<'\n';
    int y_pron, y_real;
    bool state=1;
    W[0] = bias;
    W[1] = -0.1;
    W[2] = 0.3;
    X[0] = 1;
    int epoca=0;
    while(state)
    {
        state=0;
        cout<<"EPOCA "<<++epoca<<'\n';
        cout<<"Pesos\n";
        cout<<"W0 = "<<W[0]<<'\n';
        cout<<"W1 = "<<W[1]<<'\n';
        cout<<"W2 = "<<W[2]<<'\n';
        for(int f=0 ; f<info.size() ; ++f)
        {
            cout<<"\nDato "<<f+1<<'\n';
            X[1] = info[f][0];
            X[2] = info[f][1];
            y_real = info[f][2];
            y_pron = f_escalon();

            cout<<"x1 = "<<X[1]<<'\n';
            cout<<"x2 = "<<X[2]<<'\n';
            cout<<"X1 AND X2 = "<<y_real<<'\n';
            cout<<"Funcion escalon = "<<y_pron<<'\n';
            if(y_pron != y_real)
            {
                cout<<"Salida incorrecta, se ajustan los pesos\n";
                ajuste(y_real, y_pron);
                state=1;
                cout<<"Nuevos pesos\n";
                cout<<"W0 = "<<W[0]<<'\n';
                cout<<"W1 = "<<W[1]<<'\n';
                cout<<"W2 = "<<W[2]<<'\n';
            }
            else
                cout<<"Salita correcta, los pesos no cambian\n";
        }
        cout<<"Final de epoca\n\n";
    }
}