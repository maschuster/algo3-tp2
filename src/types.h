#pragma once
#include <vector>

using namespace std;


// Valor indefinido para un coloreo
constexpr int UNDEFINED = -1;

// Representación del grafo por lista de adyacencia
typedef int Vertice;
typedef vector<vector<Vertice>> Grafo;

typedef int Grado;

// m_ij = (i, j) \in G
// Basta con bool porque son grafos y no multigrafos.
typedef vector<vector<bool>> MatrizAdyacencia;

// Representación de una instancia del problema
struct Instancia {
    Grafo G;
    Grafo H;
};

// Un coloreo asigna un color a cada vertice.
typedef int Color;
typedef vector<Color> Coloreo;

struct Solucion {
    Coloreo coloreo;
    int impacto;
};

struct TabuArgs {
    int memoria;
    int iteraciones;
    int porcentajeVecindad;
    bool aspirar;
    bool debug;
};

struct Args {
    string algoritmo;
    string algoritmoInicial;
    TabuArgs tabu;
};