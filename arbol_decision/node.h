#ifndef NODE_H
#define NODE_H
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#define mstri std::map<std::string, int>
#define mstrd std::map<std::string, double>

class Node
{
    std::string caract;
    std::string aux;
    // atributo, puntero
    mstri atrib;
    // atributo final, contador
    mstri res;
    double ganancia;
    double entropia;
    double peso;
public:
    Node* padre;
    std::map<std::string, Node*> hijos;
    Node(Node* _padre=nullptr):ganancia(0),entropia(0),peso(0),padre(_padre){}
    Node(Node* _padre, double _ganancia, double _entropia, double _peso)
    :ganancia(_ganancia),entropia(_entropia),peso(_peso),padre(_padre){}
    void setEntropia(double _entropia){ entropia = _entropia; }
    double getEntropia(){ return entropia; }
    void setGanancia(double _ganancia){ ganancia = _ganancia; }
    double getGanancia(){ return ganancia; }
    void setCaract(std::string _caract){ caract = _caract; }
    std::string getCaract(){ return caract; }
    void setAtrib(mstri& _atrib){ atrib=_atrib; }
    mstri getAtrib(){ return atrib; }
    void setRes(mstri& _res){ res=_res; }
    mstri getRes(){ return res; }

    void setHijos(mstrd& entropias);
    void printData(std::ofstream& F);
    void graficar(Node* N, std::ofstream& F, int& name);
    void unir(std::ofstream& F);
};

// void Node::printData()
// {
    // std::cout<<"Nodo "<<getCaract()<<'\n';
    // std::cout<<"Entropia "<<getEntropia()<<'\n';
    // std::cout<<"Ganancia "<<getGanancia()<<'\n';
    // std::cout<<"size "<<atrib.size()<<'\n';
    // for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
    // {
    //     std::cout<<it->first<<" "<<it->second<<'\n';
    // }
// }

void Node::printData(std::ofstream& F)
{
    F<<"caracteristica: "<<caract<<'\n';
    F<<"padre: "<<padre->caract<<'\n';
    F<<"entropia: "<<entropia<<'\n';
    F<<"ganancia: "<<ganancia<<'\n';
    F<<"distribucion = "<<res.size()<<"\n";
    for(auto it=res.begin() ; it!=res.end() ; ++it)
    {
        F<<it->first<<" "<<it->second<<'\n';
    }
    F<<"hijos = "<<hijos.size()<<"\n";
    for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
    {
        F<<it->first<<" "<<it->second<<'\n';
    }
    F<<'\n';
    for(auto it=hijos.begin() ; it!=hijos.end() ; ++it)
    {
        F<<"atributo "<<it->first<<'\n';
        it->second->printData(F);
    }
}

void Node::setHijos(mstrd& entropias)
{
    for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
    {
        hijos[it->first] = new Node(this);
        hijos[it->first]->setEntropia(entropias[it->first]);
    }
}

void Node::graficar(Node* N, std::ofstream& F, int& name)
{
    if(caract == "")
    {
        aux = std::to_string(name);
        F<<aux;
        name++;
    }
    else
        F<<caract;
    F<<" [label=\" {";
    F<<caract;
    F<<"|";
    for(auto it=res.begin() ; it!=res.end() ; ++it)
    {
        F<<it->first<<" = "<<it->second<<" ";
    }
    F<<"|{";
    for(auto it=atrib.begin() ; it!=atrib.end() ; ++it)
    {
        F<<it->first; 
        if(it!=--atrib.end())
            F<<"|";
    }
    F<<"}}\", fontcolor=blue, fontsize=\"15pt\"];\n";
    if(caract == "")
        return;
    for(auto it=hijos.begin() ; it!=hijos.end() ; ++it)
        it->second->graficar(it->second, F, name);
}

void Node::unir(std::ofstream& F)
{
    if(caract == "")
    {
        F<<aux<<'\n';
        return;
    }
    {
        for(auto it=hijos.begin() ; it!=hijos.end() ; ++it)
        {
            F<<caract<<" -> ";
            it->second->unir(F);
        }
    }
}

#endif