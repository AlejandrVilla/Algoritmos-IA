#ifndef DT_H
#define DT_H
#include "node.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <map>
#include <set>
#define vstr std::vector<std::string>
#define vvstr std::vector<vstr>
#define vd std::vector<double>
#define mstri std::map<std::string, int>
#define mmstri std::multimap<std::string, int>
#define mstrd std::map<std::string, double>
#define sstr std::set<std::string>

class DT
{
private:
    Node* root;
    vstr caracteristicas;
    vvstr atributos;
    vvstr testInfo;
    int n,m;
    double entropia(vd& arr);
    double ganancia(double entrpPadre, vd& arr);
    bool caractRepe(int i, mmstri& atrib);
public:
    DT();
    void trainProcess(Node* N, mmstri atrib);
    void testProcess(Node* N, mstri& indices, int f, double& ac, std::ofstream& F);
    void train(std::string dir);
    void test(std::string dir, std::string dir2);

    void printCaracteristicas();
    void printAtributos();
    void printData(std::string dir);
    void graficar(std::string dir);
};

DT::DT()
{
    root = nullptr;
}

void DT::trainProcess(Node* N, mmstri atrib)
{
    // falta condicion de fin de recursividad
    if(atrib.size() == n-1) return;
    if(N==root)
    {
        double mayor = 0;
        mstrd entropias;
        int _i;
        for(int i=0 ; i<m-1 ; ++i)
        {
            vd arr;
            mstri tmp;      // acumulador segun atributos hijo
            double countPadre=0;    
            mstrd entropiasTmp;     // guarda entropias
            // contador atributos para una caracteristica
            for(int f=0 ; f<n ; ++f)
            {
                tmp[atributos[f][i]]++;
                countPadre++;
            }
            // recorre atributos
            mstri tmp2;     // acumulador de total atributos finales
            for(auto it=tmp.begin() ; it!=tmp.end() ; ++it)
            {
                vd arrHijo;     // guarda probabilidades para entropia del hijo
                mstri tmp3;     // acumulador segun atributos finales
                double countHijo=0;     // contados de atributos
                for(int f=0 ; f<n ; ++f)
                {
                    if(atributos[f][i] == it->first)
                    {
                        tmp2[atributos[f][m-1]]++;
                        tmp3[atributos[f][m-1]]++;
                        countHijo++;
                    }
                }
                for(auto it2=tmp3.begin() ; it2!=tmp3.end() ; ++it2)
                    arrHijo.push_back(it2->second/countHijo);       // agrega probabilidades
                double entroHijo = entropia(arrHijo);
                entropiasTmp[it->first]=entroHijo;
                arr.push_back((countHijo/countPadre)*entroHijo);        // inserta peso*entropiaHijo
            }
            double tmpGanancia = ganancia(root->getEntropia(), arr);
            if(tmpGanancia >= mayor)
            {
                root->setAtrib(tmp);
                root->setGanancia(tmpGanancia);
                root->setCaract(caracteristicas[i]);
                root->setRes(tmp2);
                entropias = entropiasTmp;
                mayor = tmpGanancia;
                _i = i;
            }
        }
        root->setHijos(entropias);
        root->padre = root;
        for(auto it=root->hijos.begin(); it!=root->hijos.end() ; ++it)
        {
            mmstri aux;                     // para guardar recorrido actual en el arbol
            aux.insert({it->first, _i});
            trainProcess(it->second, aux);
        }
    }
    else
    {
        if(N->getEntropia() == 1) return;
        double mayor = 0;
        mstrd entropias;            // guarda entropias
        int _i;
        for(int i=0 ; i<m-1 ; ++i)
        {
            vd arr;
            mstri tmp;      // acumulador segun atributos hijo
            double countPadre=0;
            mstrd entropiasTmp;
            if(caractRepe(i, atrib)) continue;  // no repite columna
            // contador atributos para una caracteristica
            for(int f=0 ; f<n ; ++f)
                // identifica todos sus atributos
                tmp[atributos[f][i]] = 0;            
            for(int f=0 ; f<n ; ++f)
            {
                bool acept = 1;
                for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
                {
                    if(atributos[f][it->second] != it->first)
                        acept = 0;
                }
                if(acept)
                {
                    tmp[atributos[f][i]]++;
                    countPadre++;
                }
            }
            // recorre atributos
            mstri tmp2;         // acumulador de total atributos finales
            for(auto it=tmp.begin() ; it!=tmp.end() ; ++it)
            {
                vd arrHijo;     // guarda probabilidades para entropia del hijo
                mstri tmp3;     // acumulador segun atributos finales
                double countHijo=0;     // contador de atributos
                for(int f=0 ; f<n ; ++f)
                {
                    bool acept = 1;
                    for(auto it2=atrib.begin() ; it2!=atrib.end() ; ++it2)
                    {
                        if(atributos[f][it2->second] != it2->first)
                            acept = 0;
                    }
                    if(atributos[f][i] == it->first && acept)
                    {
                        tmp2[atributos[f][m-1]]++;
                        tmp3[atributos[f][m-1]]++;
                        countHijo++;
                    }
                }
                for(auto it2=tmp3.begin() ; it2!=tmp3.end() ; ++it2)
                    arrHijo.push_back(it2->second/countHijo);           // agrega probabilidades
                double entroHijo = entropia(arrHijo);
                entropiasTmp[it->first]=entroHijo;
                arr.push_back((countHijo/countPadre)*entroHijo);        // inserta peso*entropiaHijo
            }
            double tmpGanancia = ganancia(N->getEntropia(), arr);
            if(N->getEntropia() == 0)
            {
                N->setRes(tmp2);
                return;
            }
            else if(tmpGanancia >= mayor)
            {
                
                N->setAtrib(tmp);
                N->setGanancia(tmpGanancia);
                N->setCaract(caracteristicas[i]);
                entropias = entropiasTmp;
                mayor = tmpGanancia;
                _i = i;
                N->setRes(tmp2);
            }
        }
        N->setHijos(entropias);

        for(auto it=N->hijos.begin(); it!=N->hijos.end() ; ++it)
        {
            mmstri aux = atrib;             // para guardar recorrido actual en el arbol
            aux.insert({it->first, _i});
            trainProcess(it->second, aux);
        }
    }
}

void DT::train(std::string dir)
{
    // lectura de datos para train
    std::ifstream F(dir);
    std::string linea;
    std::getline(F, linea);
    std::stringstream aux(linea);
    std::string palabra;
    while(std::getline(aux, palabra, ','))
        caracteristicas.push_back(palabra);
    while(std::getline(F, linea))
    {
        std::stringstream aux2(linea);
        vstr tmp;
        while(std::getline(aux2, palabra, ','))
            tmp.push_back(palabra);
        atributos.push_back(tmp);
    }
    F.close();
    // asigna datos a la raiz
    n=atributos.size();
    m=atributos[0].size();
    // datos raiz
    root = new Node();
    mstri tmp;
    double count=0;
    // acumula resultados finales
    for(int f=0 ; f<n ; ++f)
    {
        tmp[atributos[f][m-1]]++;
        count++;
    }
    // acumulados
    vd arr;
    // probabilidades
    for(auto it=tmp.begin() ; it!=tmp.end() ; ++it)
        arr.push_back(it->second/count);
    root->setEntropia(entropia(arr));   // entropia de la raiz
    mmstri A;
    // proceso de entrenamiento para generar el arbol
    trainProcess(root, A);
}

void DT::testProcess(Node* N, mstri& indices, int f, double& ac, std::ofstream& F)
{
    if(N->getCaract() == "")
    {
        // std::cout<<"comprobando\n";
        auto it=N->getRes().begin();
        F<<"Para test "<<f+1<<'\n';
        F<<"Valor predicho "<<it->first<<'\n';
        F<<"Valor real "<<testInfo[f][m-1]<<'\n';
        if(testInfo[f][m-1] == it->first)
            ac++;
        return;
    }
    int c = indices[N->getCaract()];
    std::string atrib = testInfo[f][c];
    testProcess(N->hijos[atrib], indices, f, ac, F);
}

void DT::test(std::string dir, std::string dir2)
{
    // lectura de datos para train
    mstri indices;
    std::ifstream F(dir);
    std::string linea;
    std::getline(F, linea);
    std::stringstream aux(linea);
    std::string palabra;
    int i=0;
    while(std::getline(aux, palabra, ','))
        indices[palabra]=i++;
    while(std::getline(F, linea))
    {
        std::stringstream aux2(linea);
        vstr tmp;
        while(std::getline(aux2, palabra, ','))
            tmp.push_back(palabra);
        testInfo.push_back(tmp);
    }
    F.close();

    std::ofstream F2(dir2);
    double ac=0;
    for(int f=0 ; f<testInfo.size() ; ++f)
        testProcess(root, indices, f, ac, F2);
    F2<<"\nTasa de exito = "<<ac/double(testInfo.size())*100<<"%\n";
    F2.close();
}

double DT::entropia(vd& arr)
{
    double ac=0;
    for(int i=0 ; i<arr.size() ; ++i)
    {
        ac += -arr[i]*log2(arr[i]);
    }
    return ac;
}

double DT::ganancia(double entrpPadre, vd& arr)
{
    double ac=0;
    for(int i=0 ; i<arr.size() ; ++i)
        ac += arr[i];
    return entrpPadre-ac;
}

bool DT::caractRepe(int i, mmstri& atrib)
{
    for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
        if(i==it->second) return true;
    return false;
}

void DT::printCaracteristicas()
{
    for(int i=0 ; i<caracteristicas.size() ; ++i)
        std::cout<<caracteristicas[i]<<' ';
    std::cout<<'\n';
}

void DT::printAtributos()
{
    for(int i=0 ; i<n ; ++i)
    {
        for(int j=0 ; j<m ; ++j)
            std::cout<<atributos[i][j]<<' ';
        std::cout<<'\n';
    }
}

void DT::printData(std::string dir)
{
    std::ofstream F(dir);
    F<<"ALEJANDRO VILLA HERRERA\n";
    F<<"AGORITMO ID3\n";
    F<<"\nCONJUNTO DE ENTRENAMIENTO\n";
    for(int i=0 ; i<caracteristicas.size() ; ++i)
        F<<caracteristicas[i]<<' ';
    F<<'\n';
    for(int i=0 ; i<n ; ++i)
    {
        for(int j=0 ; j<m ; ++j)
            F<<atributos[i][j]<<' ';
        F<<'\n';
    }
    F<<"\nCONJUNTO DE TEST\n";
    for(int i=0 ; i<testInfo.size() ; ++i)
    {
        for(int j=0 ; j<testInfo[i].size() ; ++j)
            F<<testInfo[i][j]<<' ';
        F<<'\n';
    }
    F<<'\n';
    root->printData(F);
    F.close();
}

void DT::graficar(std::string dir)
{
    std::ofstream F(dir);
    F<<"digraph G{\n";
    F<<"center=true\n";
    F<<"node [shape=record];\n";
    int nam=0;
    root->graficar(root, F, nam);
    root->unir(F);
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

#endif