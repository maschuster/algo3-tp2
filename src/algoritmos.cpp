#include <vector>
#include <iostream>

using namespace std;

// Valor indefinido para un coloreo
constexpr int UNDEFINED = -1;

// Representación del grafo por lista de adyacencia
typedef int Vertice;
typedef vector<vector<Vertice>> Grafo;

// m_ij = (i, j) \in G
// Basta con bool porque son grafos y no multigrafos.
typedef vector<vector<bool>> MatrizAdyacencia;

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
//
// Complejidad temporal: O(m_H)
int calcularImpacto(const Instancia& I, const Coloreo& coloreo) {
    int impacto = 0;

    // La complejidad temporal es O(m_H) porque se recorre cada arista una vez.
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

// Golosas

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

// Convierte un grafo representado como lista de adyacencia a una matriz de
// adyacencia, en donde
//
//     m_ij = (i, j) \in G
//
// Complejidad temporal: O(m_G)
MatrizAdyacencia listaAMatDeAdyacencia(Grafo G) {
    MatrizAdyacencia mat(G.size(), vector<bool>(G.size(), false));

    // La complejidad temporal es O(m_G) pues recorro a lo
    for (Vertice v = 1; v < G.size(); v++) {
        for (Vertice w : G[v]) {
            mat[v][w] = true;
        }
    }

    return mat;
}

// Algoritmo Wyrna
Solucion wyrnisticaDiferencialGolosa(const Instancia& I) {
    // 1. Buscar W (wyrna) = H - G, las aristas de H que no estan en G
    // 2. Pintar u y v tq (u, v) \in W del mismo color
    Coloreo coloreo(I.G.size(), UNDEFINED);

    // Transformamos la representacion de lista de adyacencia a matriz de
    // incidencia.
    MatrizAdyacencia MatG = listaAMatDeAdyacencia(I.G);
    for (Vertice v = 1; v < I.H.size(); v++) {
        // Si v esta coloreado, continuo
        if (coloreo[v] != UNDEFINED) continue;

        for (Vertice w : I.H[v]) {
            // Si (v, w) \in G, no pueden tener el mismo color porque no seria
            // un coloreo valido
            if (MatG[v][w]) continue;

            // Si w tiene color, lo salteamos
            if (coloreo[w] != UNDEFINED) continue;

            // Les asignamos a ambos el color del minimo
            int color = min(v, w);
            coloreo[w] = color;
            coloreo[v] = color;
            break;
        }

    }

    // 3. Rellenar el resto con colores distintos a partir de n+1
    //    No se va a pisar, ya que como usamos el numero de cada vertice como
    //    color, a lo sumo es n.
    for(int v = 1; v < coloreo.size(); v++) {
        if (coloreo[v] == UNDEFINED) coloreo[v] = coloreo.size() + v;
    }

    return Solucion {
        .coloreo = coloreo,
        .impacto = calcularImpacto(I, coloreo),
    };
}

