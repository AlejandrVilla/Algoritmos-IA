#include <bits/stdc++.h>
#define loop(a,b,c) for(int a=b; a<c; ++a)
#define vd vector<double>
#define vi vector<int>
#define vvd vector<vd>
#define pdd pair<double,double>
#define mistr map<int, string>
#define vpdd vector<pdd>
#define mivi map<int,vi>
#define vstr vector<string>
using namespace std;
using namespace std;

ifstream F;
int n_datos, m_centr = 3;
int k;
vvd table;                  // atributos, SL, SW, PL, PW
// posicion de los 3 centroides
// 0 setosa
// 1 versicolor
// 2 virginica
vvd centroides(m_centr,vd(4,0));    // centroides obtenidos por datos de entrenamiento
mistr tipo_iris;                    // indice -> nombre
vd precisiones;                     // guarda precisiones de cada conjunto de datos
vvd inf1, inf2;                     // informacion para 5-fold y 10-fold
vi total_datos1, total_datos2;      // datos para cada conjunto de archivos
vd total_med1, total_med2;
vd total_desv_est1, total_desv_est2;
double media, desv_estandar;
vstr archivos_5_fold;               // guarda los nombre de los archivos para 5-fold
vstr archivos_10_fold;              // guarda los nombre de los archivos para 10-fold

double dist(vd& A, vd& B)
{
    return sqrt( pow(A[0]-B[0],2) + pow(A[1]-B[1],2) + pow(A[2]-B[2],2) + pow(A[3]-B[3],2) );
}

void setMedia()
{
    double ac = 0;
    loop(i,0,precisiones.size())
        ac+=precisiones[i];
    media = ac/precisiones.size();
}

void setDesvEstandar()
{
    double aux = 1/(double(precisiones.size())-1);
    double tmp = 0;
    loop(i,0,precisiones.size())
        tmp += pow(precisiones[i] - media, 2);
    // cout<<aux<<" "<<tmp<<'\n';
    desv_estandar = sqrt(aux*tmp);
}

void cluster()
{
    int n_test = n_datos / k;
    int n_train = n_datos - n_test;
    int s_test, e_test;
    // para los k folds
    for(int i=0 ; i<k ; ++i)
    {
        // cout<<"Iteracion "<<i+1<<'\n';
        centroides.assign(m_centr,vd(4,0));
        s_test = i*n_test;
        e_test = i*n_test + n_test;
        vi n_tipo_iris(m_centr,0);
        // usa los datos
        for(int j=0 ; j<n_datos ; ++j)
        {
            if(!(j >= s_test && j<e_test)) // train
            {
                if(tipo_iris[j] == "Iris-setosa")
                {
                    centroides[0][0] += table[j][0];
                    centroides[0][1] += table[j][1];
                    centroides[0][2] += table[j][2];
                    centroides[0][3] += table[j][3];
                    n_tipo_iris[0]++;
                }
                else if(tipo_iris[j] == "Iris-versicolor")
                {
                    centroides[1][0] += table[j][0];
                    centroides[1][1] += table[j][1];
                    centroides[1][2] += table[j][2];
                    centroides[1][3] += table[j][3];
                    n_tipo_iris[1]++;
                }
                else if(tipo_iris[j] == "Iris-virginica")
                {
                    centroides[2][0] += table[j][0];
                    centroides[2][1] += table[j][1];
                    centroides[2][2] += table[j][2];
                    centroides[2][3] += table[j][3];
                    n_tipo_iris[2]++;
                }
            }
        }
        // define los centroides con los datos de entrenamiento
        // cout<<"intervalo para test ["<<s_test<<", "<<e_test<<"[\n";
        for(int j=0 ; j<m_centr ; ++j)
        {
            centroides[j][0] /= n_tipo_iris[j];
            centroides[j][1] /= n_tipo_iris[j];
            centroides[j][2] /= n_tipo_iris[j];
            centroides[j][3] /= n_tipo_iris[j];
            // cout<<"C"<<j<<" "<<centroides[j][0]<<" "<<centroides[j][1]<<" "<<centroides[j][2]<<" "<<centroides[j][3]<<'\n';
        }
        // cout<<'\n';
        // test
        double ac=0;
        for(int j=s_test ; j<e_test ; ++j)
        {
            // definir la menor dist a un centroide para aproximar
            double dist_set, dist_vers, dist_virg;
            dist_set = dist(centroides[0], table[j]);
            dist_vers = dist(centroides[1], table[j]);
            dist_virg = dist(centroides[2], table[j]);

            if(dist_set < dist_vers && dist_set < dist_virg)
            {
                // cout<<"cluster: Iris-setosa, original: "<<tipo_iris[j]<<'\n';
                if(tipo_iris[j] == "Iris-setosa")
                    ac++;
            }
            else if(dist_vers < dist_set && dist_vers < dist_virg)
            {
                // cout<<"cluster: Iris-versicolor, original: "<<tipo_iris[j]<<'\n'; 
                if(tipo_iris[j] == "Iris-versicolor")
                    ac++;
            }
            if(dist_virg < dist_vers && dist_virg < dist_set)
            {
                // cout<<"cluster: Iris-virginica, original: "<<tipo_iris[j]<<'\n';
                if(tipo_iris[j] == "Iris-virginica")
                    ac++;
            }
        }
        // cout<<'\n';
        double precision = ac/(n_datos/k) * 100;
        // cout<<"Precision "<< precision <<"%\n\n";
        precisiones.push_back(precision);
    }
}

void showTable()
{
    loop(i,0,n_datos)
    {
        cout<<tipo_iris[i]<<": ";
        loop(j,0,4)
            cout<<table[i][j]<<" ";
        cout<<'\n';
    }
}

void process(string&& dir, vstr& files)
{
    F.open(dir);
    string dir_f;
    while(F>>dir_f)     // lee nombre de los archivos a usar
    {
        files.push_back(dir_f);
        // cout<<dir_f<<'\n';
    }
    F.close();
    loop(j,0,files.size())
    {
        F.open(files[j]);
        if(!F)
            cout<<"no se pudo abrir el archivo\n";
        // cout<<"Archivo "<<j+1<<'\n';
        // cout<<files[j]<<'\n';
        string linea;
        // limpia parametros iniciales
        while(getline(F,linea) && linea != "@data");
            // cout<<linea<<'\n';
        n_datos = 0;
        table.clear();
        precisiones.clear();
        tipo_iris.clear();
        string a,b,c,d,tipo;
        int i=0;
        while(F>>a>>b>>c>>d>>tipo)
        {
            a.pop_back();
            b.pop_back();
            c.pop_back();
            d.pop_back();
            // cout<<a<<" "<<b<<" "<<c<<" "<<d<<" "<<tipo<<'\n';
            vd atr;
            atr.push_back(stod(a));
            atr.push_back(stod(b));
            atr.push_back(stod(c));
            atr.push_back(stod(d));
            table.push_back(atr);
            tipo_iris[i] = tipo;            // guarda el tipo segun su indice
            i++;
        }
        n_datos = table.size();
        // cout<<"Cantidad de datos "<<n_datos<<'\n';
        // showTable();
        cluster();
        // cout<<"Fold \t\t\tprecision\n";
        // loop(i, 0, precisiones.size())
            // cout<<i<<"\t\t\t\t"<<precisiones[i]<<'\n';
        setMedia();
        setDesvEstandar();
        // guarda informacion
        if(k==5)
        {
            inf1.push_back(precisiones);
            total_datos1.push_back(n_datos);
            total_med1.push_back(media);
            total_desv_est1.push_back(desv_estandar);
        }
        else
        {
            inf2.push_back(precisiones);
            total_datos2.push_back(n_datos);
            total_med2.push_back(media);
            total_desv_est2.push_back(desv_estandar);
        }
        // cout<<"media\t\t\t"<<media<<'\n';
        // cout<<"Desv. estandar\t"<<desv_estandar<<'\n';
        // cout<<'\n';
        F.close();
    }
}

int main()
{
    // freopen("output.txt","w",stdout);
    // para el primer conjunto de archivos iris-5-fold
    k=5;
    freopen("output1.txt","w",stdout);
    process("input1.txt", archivos_5_fold);
    cout<<"\t\t\t\t\t\t\t\t\t\tPrecisiones\n";
    cout<<"Fold\t\tF1\t\tF2\t\tF3\t\tF4\t\tF5\t\tF6\t\tF7\t\tF8\t\tF9\t\tF10\n";
    loop(i,0,k) 
    {
        cout<<i<<"\t\t\t";
        loop(j,0,archivos_5_fold.size())
            cout<<setprecision(2)<<fixed<<inf1[j][i]<<"\t";
        cout<<'\n';
    }
    cout<<"media\t\t";
    loop(i,0,total_med1.size())
        cout<<total_med1[i]<<'\t';
    cout<<'\n';
    cout<<"desv est\t";
    loop(i,0,total_desv_est1.size())
        cout<<total_desv_est1[i]<<'\t';
    cout<<'\n';
    cout<<"total dat\t";
    loop(i,0,total_datos1.size())
        cout<<total_datos1[i]<<"\t\t";

    // para el segundo conjunto de archivos iris-10-fold
    k=10;
    freopen("output2.txt","w",stdout);
    process("input2.txt", archivos_10_fold);
    cout<<"\t\t\t\t\t\t\t\t\t\tPrecisiones\n";
    cout<<"Fold\t\tF1\t\tF2\t\tF3\t\tF4\t\tF5\t\tF6\t\tF7\t\tF8\t\tF9\t\tF10\n";
    loop(i,0,k) 
    {
        cout<<i<<"\t\t\t";
        loop(j,0,archivos_10_fold.size()/2)
            cout<<setprecision(2)<<fixed<<inf2[j][i]<<"\t";
        cout<<'\n';
    }
    cout<<"media\t\t";
    loop(i,0,total_med2.size()/2)
        cout<<total_med2[i]<<'\t';
    cout<<'\n';
    cout<<"desv est\t";
    loop(i,0,total_desv_est2.size()/2)
        cout<<total_desv_est2[i]<<'\t';
    cout<<'\n';
    cout<<"total dat\t";
    loop(i,0,total_datos2.size()/2)
        cout<<total_datos2[i]<<"\t\t";

    cout<<"\n\n";
    cout<<"\t\t\t\t\t\t\t\t\t\tPrecisiones\n";
    cout<<"Fold\t\tF11\t\tF12\t\tF13\t\tF14\t\tF15\t\tF16\t\tF17\t\tF18\t\tF19\t\tF20\n";
    loop(i,0,k) 
    {
        cout<<i<<"\t\t\t";
        loop(j,archivos_10_fold.size()/2,archivos_10_fold.size())
            cout<<setprecision(2)<<fixed<<inf2[j][i]<<"\t";
        cout<<'\n';
    }
    cout<<"media\t\t";
    loop(i,total_med2.size()/2,total_med2.size())
        cout<<total_med2[i]<<'\t';
    cout<<'\n';
    cout<<"desv est\t";
    loop(i,total_desv_est2.size()/2,total_desv_est2.size())
        cout<<total_desv_est2[i]<<'\t';
    cout<<'\n';
    cout<<"total dat\t";
    loop(i,total_datos2.size()/2,total_datos2.size())
        cout<<total_datos2[i]<<"\t\t";
    
}