#include "BplusTree.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;

int main(){

    // Read file
    std::ifstream texto;
    texto.open("../output.txt");

    vector<int> datos(1000000);
    int element;
    if (texto.is_open()) {
        int i = 0;
        while (texto >> element) {
            datos[i++] = element;
        }
    }

    int64_t tiempoInseccion = 0;
    for (int t=0; t<10; ++t){
        auto tree = new BplusTree();
        
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        for (int i=0; i<1000000; ++i){
            tree->insertar( datos[i] );
            //cout << datos[i] << endl;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        // Calcular tiempo
        auto tiempo = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        tiempoInseccion += tiempo;
    }

    cout << endl << tiempoInseccion << endl;

    // Test Video:

    // auto tree = new BplusTree();

    // int data[12] = {1,4,7,10,17,21,31,25,19,20,28,42};

    // for(int i = 0 ; i < 12 ; i++){
    //     tree->insertar(data[i]);
    // }

    // vector<int> vec = tree->bfs();
    // for(auto a : vec){
    //     cout << a << " ";
    // }

    return 0;
}