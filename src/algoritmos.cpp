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

// Un color (int) para cada vertice.
typedef vector<int> Coloreo;

struct Solucion {
    Coloreo coloreo;
    int impacto;
};

// Calcula el impacto de un coloreo sobre una instancia, definido como la
// cantidad de vertices adyacentes en H que tienen el mismo color en G.
int calcularImpacto(const Instancia& I, const Coloreo& coloreo) {
    int impacto = 0;
    
    // Para cada vertice en el grafo
    for (Vertice v = 1; v < I.H.size(); v++) {

        // Recorremos sus vecinos viendo si tienen el mismo color.
        for (Vertice w : I.H[v]) {
            if (coloreo[w] == coloreo[v]) {
                impacto++;
            }
        }
    }

    // Se cuenta dos veces para cada arista (cada extremo)
    return impacto / 2;
}

// Le asigna un color diferente a cada vertice, el coloreo trivial.
Solucion pcmiConstructivaControl(const Instancia& I) {
    Coloreo coloreo(I.G.size(), 0);
    for(int i = 1; i < coloreo.size(); i++) {
        coloreo[i] = i;
    }

    return Solucion {
        .coloreo = coloreo,
        .impacto = calcularImpacto(I, coloreo),
    };
}