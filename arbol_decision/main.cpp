#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include "DT.h"
using namespace std;

int main()
{
    DT dt;
    dt.train("virus_train.csv");
    // dt.printCaracteristicas();
    // dt.printAtributos();
    dt.test("virus_test.csv", "test.txt");
    dt.printData("train.txt");
    dt.graficar("DT.dot");
    cout<<"fin\n";
}