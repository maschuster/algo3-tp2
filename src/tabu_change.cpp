#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <string>

#include "circular_vector.cpp"
#include "types.h"

using namespace std;

namespace change {

    // Representacion de la estructura de un coloreo como tuplas que representan
    // reemplazos de color para un vertice. (v, c) = cambiar el color de v por c
    typedef pair<Vertice, Color> CambioEstructural;

    // Solucion vecina
    struct Vecino {
        Solucion sol;
        CambioEstructural estr;
    };

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
    vector<Vecino> vecinos(const Instancia& I, const Solucion& sol, int percent) {
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
    Solucion tabuEstructura(const Instancia& I, const Solucion& solucionInicial, TabuArgs args) {
        // Obtenemos la solución inicial a partir de una constructiva golosa
        Solucion sol = solucionInicial;
        Solucion best = sol;

        // Inicializamos la memoria de tamaño fijo
        auto memoria = CircularVector<CambioEstructural>(args.memoria);

        for(int i = 0; i < args.iteraciones; i++) {
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
    Solucion tabuColoreo(const Instancia& I, const Solucion& solucionInicial, TabuArgs args) {
        // Obtenemos la solución inicial a partir de una constructiva golosa
        // TODO: cambiar con la experimentalmente mejor
        Solucion sol = solucionInicial;
        Solucion best = sol;

        // Inicializamos la memoria de tamaño fijo
        auto memoria = CircularVector<Coloreo>(args.memoria);

        for(int i = 0; i < args.iteraciones; i++) {
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

            // La marcamos como nueva mejor si lo es
            if (best.impacto < sol.impacto) {
                best = sol;
            }
        }

        return sol;
    }

}