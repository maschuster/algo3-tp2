#include <algorithm> // sort
#include "types.h"
#include "common.cpp"

// Golosas

// Le asigna un color diferente a cada vertice, el coloreo trivial.
Solucion pcmiConstructivaControl(const Instancia& I) {
    Coloreo coloreo(I.G.size(), UNDEFINED);
    for(int i = 1; i < coloreo.size(); i++) {
        coloreo[i] = i;
    }

    return Solucion {
        .coloreo = coloreo,
        .impacto = calcularImpacto(I, coloreo),
    };
}

// Funcion de comparacion de pares de vertices y grados
// Ordena de mayor a menor.
bool cmp(pair<Grado, Vertice> a, pair<Grado, Vertice> b) { 
    return (a.first > b.first); 
}

// Busca un coloreo usando la heurisica secuencial LF (largest first), que
// ordena los nodos de mayor a menor segun su grado y luego le asigna a cada uno
// el minimo color que pueda.
// Busca colorear de forma buena G, pero no le presta atencion a H.
//
// Complejidad temporal: O(n^2 * m_G)
Solucion golosaSecuencialLF(const Instancia& I) {
    // Ordenamos los vertices segun su grado
    vector<pair<Grado, Vertice>> vertices;

    // O(n)
    for(Vertice v = 1; v < I.G.size(); v++) {
        vertices.push_back(make_pair(I.G[v].size(), v));
    }

    // O(n lg(n))
    sort(vertices.begin(), vertices.end(), cmp);

    // Los coloreamos
    Coloreo coloreo(I.G.size(), UNDEFINED);
    // O(n^2 m_G)
    for (pair<Grado, Vertice> p : vertices) {
        Vertice v = p.second;
        // Le asignamos el minimo color tal que no tiene un vecino que lo
        // comparta.
        // O(n * m_G)
        for (Color c = 1; c < I.G.size(); c++) {
            // O(m_G)
            if (!tieneVecinoConColor(I.G, v, coloreo, c)) {
                coloreo[v] = c;
                break;
            }
        }
    }

    return Solucion {
        .coloreo = coloreo,
        .impacto = calcularImpacto(I, coloreo), // O(m_H)
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
// O(m_H + m_G + n)
Solucion wyrnisticaDiferencialGolosa(const Instancia& I) {
    // 1. Buscar W (wyrna) = H - G, las aristas de H que no estan en G
    // 2. Pintar u y v tq (u, v) \in W del mismo color
    Coloreo coloreo(I.G.size(), UNDEFINED);

    // Transformamos la representacion de lista de adyacencia a matriz de
    // incidencia.
    MatrizAdyacencia MatG = listaAMatDeAdyacencia(I.G);
    for (Vertice v = 1; v < I.H.size(); v++) {
        for (int i = 0; i < I.H[v].size() && coloreo[v] == UNDEFINED; i++) {
            Vertice w = I.H[v][i];

            // Si (v, w) \in G, no pueden tener el mismo color porque no seria
            // un coloreo valido
            if (MatG[v][w]) continue;

            // Si w tiene color, lo salteamos
            if (coloreo[w] != UNDEFINED) continue;

            // Les asignamos a ambos el color del minimo
            int color = min(v, w);
            coloreo[w] = color;
            coloreo[v] = color;
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

// Algoritmo Wyrnower - Wyrna Power. Con chequeo de colores.
// La diferencia principal con el algoritmo Wyrna es que cuando se encuentra con
// un vertice vecino que ya esta coloreado, en vez de saltearlo, se fija si
// puede tomar el mismo color chequeando que no tenga otro con él en su
// vecindad.
// O(m_H + m_G + n)
Solucion wyrnowerGolosa(const Instancia& I) {
    // 1. Buscar W (wyrna) = H - G, las aristas de H que no estan en G
    // 2. Pintar u y v tq (u, v) \in W del mismo color
    Coloreo coloreo(I.G.size(), UNDEFINED);

    // Transformamos la representacion de lista de adyacencia a matriz de
    // incidencia.
    MatrizAdyacencia MatG = listaAMatDeAdyacencia(I.G);
    for (Vertice v = 1; v < I.H.size(); v++) {
        for (int i = 0; i < I.H[v].size() && coloreo[v] == UNDEFINED; i++) {
            Vertice w = I.H[v][i];
            // Si (v, w) \in G, no pueden tener el mismo color porque no seria
            // un coloreo valido
            if (MatG[v][w]) continue;

            // Si w tiene color, le queremos asignar a v el mismo pero para eso
            // no se tiene que usar en su vecindad.
            if (coloreo[w] != UNDEFINED) {
                if (!tieneVecinoConColor(I.G, v, coloreo, coloreo[w])) {
                    coloreo[v] = coloreo[w];
                }
            } else {
                // Les asignamos a ambos el color del minimo
                int color = min(v, w);
                coloreo[w] = color;
                coloreo[v] = color;
            }
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
