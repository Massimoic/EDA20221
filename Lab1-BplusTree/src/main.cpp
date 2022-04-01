#include <iostream>
#include "BplusTree.h"
#include "BplusTree.cpp"
#include <fstream>
#include <chrono>
using namespace std;

int main(){

    // Read file
    // std::ifstream texto;
    // texto.open("./output.txt");

    // int datos[ 1000000 ];
    // int element;
    // if (texto.is_open()) {
    //     int i = 0;
    //     while (texto >> element) {
    //         datos[i++] = element;
    //     }
    // }

    // int64_t tiempoInseccion = 0;
    // for (int t=0; t<10; ++t){
    //     auto tree = new BplusTree();
        
    //     std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //     for (int i=0; i<1000000; ++i){
    //         tree->insertar( datos[i] );
    //     }
    //     std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //     // Calcular tiempo
    //     auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
    //     tiempoInseccion += tiempo;
    // }

    auto tree = new BplusTree();
    tree->insertar(1);
    tree->insertar(2);
    tree->insertar(3);
    tree->insertar(4);
    tree->insertar(5);
    tree->insertar(6);
    tree->insertar(7);
    tree->insertar(8);
    tree->insertar(9);
    tree->insertar(10);
    tree->insertar(11);
    tree->insertar(12);
    tree->insertar(13);
    tree->insertar(14);
    tree->insertar(15);
    tree->insertar(16);
    tree->insertar(17);
    tree->insertar(18);
    tree->insertar(19);
    tree->insertar(20);
    tree->insertar(21);
    tree->insertar(22);
    tree->insertar(23);
    tree->insertar(24);
    tree->insertar(25);
    tree->insertar(26);

    tree->search(20);

    return 0;
}