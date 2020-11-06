#include <vector>
#include <iostream>

using namespace std;

// Representación del grafo por lista de adyacencia
typedef int Vertice;
typedef vector<vector<Vertice>> Grafo;

// Representación de una instancia del problema
struct Instancia {
    Grafo G;
    Grafo H;
};