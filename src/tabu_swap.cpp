#include <set>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>

#include "circular_vector.cpp"
#include "common.cpp"
#include "types.h"

using namespace std;

namespace swap {

    // Representacion de la estructura de un coloreo como tuplas que representan
    // swaps de colores entre vertices.
    typedef pair<Vertice, Vertice> Swap;

    // Solucion vecina
    struct Vecino {
        Solucion sol;
        Swap estr;
    };

    // O(m_H)
    int calcularDeltaImpacto(const Grafo& H, const Coloreo& coloreo, Vertice v, Vertice toSwap, Color c) {
        int delta = 0;

        for (Vertice w : H[v]) {
            Color cw = coloreo[w];

            if (w == toSwap) continue;

            if (cw == coloreo[v]) {
                // Si tiene el mismo color que el viejo de v, perdemos impacto
                delta--;
            }

            if (cw == c) {
                // Si tiene el mismo color que el que le vamos a asignar,
                // sumamos 1.
                // Si el vertice con el que vamos a intercambiar es vecino,
                // no lo consideramos.
                delta++;
            }
        }

        return delta;
    }

    // Devuelve el vecino correspondiente a swapear los colores de v y w.
    //
    // Complejidad temporal: O(m_H)
    Vecino nuevoVecino(const Instancia& I, const Solucion& sol, Vertice v, Vertice w) {
        Coloreo nuevo = sol.coloreo;
        nuevo[v] = sol.coloreo[w];
        nuevo[w] = sol.coloreo[v];

        return Vecino{
            .sol = Solucion{
                .coloreo = nuevo,
                .impacto = sol.impacto
                    + calcularDeltaImpacto(I.H, sol.coloreo, v, w, sol.coloreo[w])
                    + calcularDeltaImpacto(I.H, sol.coloreo, w, v, sol.coloreo[v])
            },
            .estr = Swap(v, w),
        };
    }

    // Retorna si el vertice v tiene un vecino con el color de w, el vertice con
    // el cual hacer el swap.
    // Θ(d_G(v)) = O(m_G)
    bool tieneVecinoConColor(const Grafo& G, const Coloreo& coloreo, Vertice v, Vertice w) {
        for (Vertice u : G[v]) {
            // Si es el vertice con el cual vamos a intercambiar, no lo queremos
            // considerar ya que siempre tendra el mismo color.
            if (u != w && coloreo[u] == coloreo[w]) {
                // Tiene
                return true;
            }
        }

        // No tiene
        return false;
    }

    // Vecinos da la vecindad de una solucion.
    // Cada vecina es generada swapeando colores de dos vertices.
    vector<Vecino> vecinos(const Instancia& I, const Solucion& sol, int percent) {
        vector<Vecino> vecindad;

        // Para cada par de vertices, vemos si swapear sus colores es factible y lo
        // agregamos como vecino.

        for (Vertice v = 1; v < I.G.size(); v++) {
            for (Vertice w = v; w < I.G.size(); w++) {
                if (!tieneVecinoConColor(I.G, sol.coloreo, v, w) &&
                    !tieneVecinoConColor(I.G, sol.coloreo, w, v)) {
                    vecindad.push_back(nuevoVecino(I, sol, v, w));
                }
            }
        }

        return subconjuntoAleatorio(vecindad, percent);
    }

    // Dado un vector de vecinos, devuelve los que no son tabu (i.e los que no estan
    // en memoria), opcionalmente aspirando si el impacto generado es mayor al
    // maximo actual.
    vector<Vecino> noTabuEstructura(vector<Vecino> vecinos, CircularVector<Swap> mem, bool aspirar, int maxActual) {
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
    Solucion tabuEstructura(const Instancia& I, const Solucion& solucionInicial, TabuArgs args) {
        // Encabezado de debug print
        if (args.debug) clog << "Impacto,Mejora" << endl;

        Solucion sol = solucionInicial;
        Solucion best = sol;
        if (args.debug) clog << sol.impacto << ',' << (best.impacto < sol.impacto) << endl;


        // Inicializamos la memoria de tamaño fijo
        auto memoria = CircularVector<Swap>(args.memoria);

        int i;
        for(i = 0; i < args.iteraciones; i++) {
            // Buscamos las soluciones vecinas y nos quedamos con las que no sean
            // tabu.
            vector<Vecino> vecindad = noTabuEstructura(
                vecinos(I, sol, args.porcentajeVecindad),
                memoria,
                args.aspirar,          // Aspiracion
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

            // Linea de debug print
            if (args.debug) clog << sol.impacto << ',' << (best.impacto < sol.impacto) << endl;

            // La marcamos como nueva mejor si lo es
            if (best.impacto < sol.impacto) {
                best = sol;
            }
        }

        clog << "Iteraciones efectivas:" << i << endl;

        return best;
    }

    // Aspiracion
    // TODO

    // Tabu seach con memoria de coloreos
    Solucion tabuColoreo(const Instancia& I, const Solucion& solucionInicial, TabuArgs args) {
        // Encabezado de debug print
        if (args.debug) clog << "Impacto,Mejora" << endl;

        // Obtenemos la solución inicial a partir de una constructiva golosa
        // TODO: cambiar con la experimentalmente mejor
        Solucion sol = solucionInicial;
        Solucion best = sol;
        if (args.debug) clog << sol.impacto << ',' << (best.impacto < sol.impacto) << endl;

        // Inicializamos la memoria de tamaño fijo
        auto memoria = CircularVector<Coloreo>(args.memoria);

        int i;
        for(i = 0; i < args.iteraciones; i++) {
            // Buscamos las soluciones vecinas y nos quedamos con las que no sean
            // tabu.
            vector<Vecino> vecindad = noTabuColoreo(vecinos(I, sol, args.porcentajeVecindad), memoria);

            if(vecindad.empty()) {
                // Si no tenemos vecindad, no hay por donde seguir explorando.
                break;
            }

            // Nos quedamos con la mejor
            Vecino vecino = mejor(vecindad);
            sol = vecino.sol;

            // Almacenamos el coloreo encontrado para no repetirlo
            memoria.push_back(vecino.sol.coloreo);

            // Linea de debug print
            if (args.debug) clog << sol.impacto << ',' << (best.impacto < sol.impacto) << endl;

            // La marcamos como nueva mejor si lo es
            if (best.impacto < sol.impacto) {
                best = sol;
            }
        }

        clog << "Iteraciones efectivas:" << i << endl;

        return best;
    }

}
