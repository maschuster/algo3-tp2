// nombre de archivo horrible.
#pragma once
#include "types.h"
#include <random>

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

// Retorna si el vertice v tiene un vecino en G con color c.
// Θ(d_G(v)) = O(m_G)
bool tieneVecinoConColor(const Grafo& G, Vertice v, const Coloreo& coloreo, Color c) {
    for (Vertice u : G[v]) {
        if (coloreo[u] == c) {
            // Tiene
            return true;
        }
    }

    // No tiene
    return false;
}

// Retorna un vector de tamaño <percent>% del vector original
// con elementos tomados al azar sin reposición.
//
// Complejidad temporal: O(n)
template<typename T>
vector<T> subconjuntoAleatorio(vector<T> &vec, int percent) {
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(vec), std::end(vec), rng);

    return vector<T>(
        vec.begin(),
        vec.begin() + ceil(vec.size()*((float) percent/100))
    );
}
