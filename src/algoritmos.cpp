#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <string>

#include "circular_vector.cpp"

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
bool tieneVecinoConColor(Grafo G, Vertice v, Coloreo coloreo, Color c) {
    for (Vertice u : G[v]) {
        if (coloreo[u] == c) {
            // Tiene
            return true;
        }
    }

    // No tiene
    return false;
}

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
        for (int i = 1; i < I.H[v].size() && coloreo[v] == UNDEFINED; i++) {
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
        for (int i = 1; i < I.H[v].size() && coloreo[v] == UNDEFINED; i++) {
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

// Representacion de la estructura de un coloreo como tuplas que representan
// reemplazos de color para un vertice. (v, c) = cambiar el color de v por c
typedef pair<Vertice, Color> CambioEstructural;

// Solucion vecina
struct Vecino {
    Solucion sol;
    CambioEstructural estr;
};

template<typename T>
void print_set(set<T> s){
    for(auto x: s){
        cout << x << " ";
    }
    cout << endl;
}


// Devuelve el vecino correspondiente a cambiar el color del vertice v por c,
// recalculando el impacto de forma eficiente.
//
// Complejidad temporal: Θ(d_H(v)) = O(m_H) (en el peor caso, pero en la
// practica no.)
Vecino nuevoVecino(const Instancia& I, const Solucion& sol, Color c, Vertice v) {
    // Para calcular el nuevo impacto, tenemos que revisar los vecinos de v en
    // H. Sera la diferencia entre el impacto del vertice v con el color
    // original y el nuevo color.
    int delta = 0;
    for(Vertice w : I.H[v]) {
        Color cw = sol.coloreo[w];
        if (cw == sol.coloreo[v]) {
            // Si tiene el mismo color que el viejo de v, perdemos impacto
            delta--;
        } else if(cw == c) {
            // Si tiene el mismo color que el que le vamos a asignar,
            // sumamos 1.
            delta++;
        }
    }

    Coloreo nuevo = sol.coloreo;
    nuevo[v] = c;
    return Vecino{
        .sol = Solucion{
            .coloreo = nuevo,
            .impacto = sol.impacto + delta
        },
        .estr = CambioEstructural(v, c),
    };
}

// Vecinos da la vecindad de una solucion.
// Cada vecina es generada cambiando el color de un vertice por otro ya usado en
// el coloreo.
vector<Vecino> vecinos(const Instancia& I, const Solucion& sol) {
    vector<Vecino> vecindad;

    set<Color> coloresSol;
    for (Color c : sol.coloreo) {
        // Esto es porque el primer color no está definido para indexar desde 1
        if (c != UNDEFINED) coloresSol.insert(c);
    }

    for (Vertice v = 1; v < I.G.size(); v++) {
        // Almacenamos los colores utilizados en los adyacentes
        set<Color> coloresAdy;
        for (Vertice w : I.G[v]) {
            coloresAdy.insert(sol.coloreo[w]);
        }

        // Agregamos el color actual para no repetirlo
        coloresAdy.insert(sol.coloreo[v]);

        // Los colores disponibles seran los de la solucion \ los de la ady.
        set<Color> coloresDisponibles;
        set_difference(
            coloresSol.begin(), coloresSol.end(),
            coloresAdy.begin(), coloresAdy.end(),
            inserter(coloresDisponibles, coloresDisponibles.end())
        );

        // print_set(coloresDisponibles);

        // Por cada color disponible formamos una solucion
        // Optimizar
        for (Color c : coloresDisponibles) {
            vecindad.push_back(nuevoVecino(I, sol, c, v));
        }
    }

    return vecindad;
}

// Dado un vector de vecinos, devuelve los que no son tabu (i.e los que no estan
// en memoria), opcionalmente aspirando si el impacto generado es mayor al
// maximo actual.
vector<Vecino> noTabuEstructura(vector<Vecino> vecinos, CircularVector<CambioEstructural> mem, bool aspirar, int maxActual) {
    vector<Vecino> factibles;

    for(Vecino v : vecinos) {
        // TODO: tambien se podria ver que sea mayor a la ultima vez que la
        // viste, en vez de mayor al max actual. Pero esto haria que aspires
        // muchisimo mas, queda para ver mas adelante.
        if (mem.contains(v.estr)) {
            if (aspirar && v.sol.impacto > maxActual) {
                factibles.push_back(v);
            }
        } else {
            factibles.push_back(v);
        }
    }

    return factibles;
}

// Dado un vector de vecinos, devuelve los que no son tabu (i.e los que no estan
// en memoria), opcionalmente aspirando si el impacto generado es mayor al
// maximo actual.
vector<Vecino> noTabuColoreo(vector<Vecino> vecinos, CircularVector<Coloreo> mem) {
    vector<Vecino> factibles;

    for(Vecino v : vecinos) {
        // Nota: No tiene sentido aspirar para la memoria de soluciones, ya que
        // una solucion que esta en la memoria nunca sera mejor porque ya fue
        // vista y contemplada en su totalidad.
        if (!mem.contains(v.sol.coloreo)) {
            factibles.push_back(v);
        }
    }

    return factibles;
}

// Devuelve el mejor vecino de un vector no vacio.
Vecino mejor(vector<Vecino> vecinos) {
    Vecino mejor = vecinos[0];
    for(Vecino v : vecinos) {
        if(v.sol.impacto > mejor.sol.impacto) {
            mejor = v;
        }
    }

    return mejor;
}

// Tabu search con memoria de estructura
Solucion tabuEstructura(const Instancia& I, int memorySize, int iterations, const Solucion& solucionInicial) {
    // Obtenemos la solución inicial a partir de una constructiva golosa
    Solucion sol = solucionInicial;
    Solucion best = sol;

    // Inicializamos la memoria de tamaño fijo
    auto memoria = CircularVector<CambioEstructural>(memorySize);

    for(int i = 0; i < iterations; i++) {
        // Buscamos las soluciones vecinas y nos quedamos con las que no sean
        // tabu.
        vector<Vecino> vecindad = noTabuEstructura(
            vecinos(I, sol),
            memoria,
            false,          // Aspiracion
            best.impacto
        );

        if(vecindad.empty()) {
            // Si no tenemos vecindad, no hay por donde seguir explorando.
            break;
        }

        // Nos quedamos con la mejor
        Vecino vecino = mejor(vecindad);
        sol = vecino.sol;

        // Almacenamos el cambio estructural realizado para asi no repetirlo
        memoria.push_back(vecino.estr);

        // La marcamos como nueva mejor si lo es
        if (best.impacto < sol.impacto) {
            best = sol;
        }
    }

    return sol;
}

// Aspiracion
// TODO

// Tabu seach con memoria de coloreos
Solucion tabuColoreo(const Instancia& I, int memorySize, int iterations, const Solucion& solucionInicial) {
    // Obtenemos la solución inicial a partir de una constructiva golosa
    // TODO: cambiar con la experimentalmente mejor
    Solucion sol = solucionInicial;
    Solucion best = sol;

    // Inicializamos la memoria de tamaño fijo
    auto memoria = CircularVector<Coloreo>(memorySize);

    for(int i = 0; i < iterations; i++) {
        // Buscamos las soluciones vecinas y nos quedamos con las que no sean
        // tabu.
        vector<Vecino> vecindad = noTabuColoreo(vecinos(I, sol), memoria);

        if(vecindad.empty()) {
            // Si no tenemos vecindad, no hay por donde seguir explorando.
            break;
        }

        // Nos quedamos con la mejor
        Vecino vecino = mejor(vecindad);
        sol = vecino.sol;

        // Almacenamos el coloreo encontrado para no repetirlo
        memoria.push_back(vecino.sol.coloreo);

        // La marcamos como nueva mejor si lo es
        if (best.impacto < sol.impacto) {
            best = sol;
        }
    }

    return sol;
}
