#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

#include "types.h"
#include "golosos.cpp"
#include "tabu_change.cpp"
#include "tabu_swap.cpp"

using namespace std;

// Formato de entrada:
//
// n, Mg, Mh
// Mg líneas con 2 enteros indicando las aristas de G,
// Mh líneas, para H.
Instancia leerGrafos() {
    int n, Mg, Mh;
    cin >> n >> Mg >> Mh;
    Grafo G(n+1, vector<Vertice>());
    for (int i = 0; i < Mg; i++) {
        int v, w;
        cin >> v >> w;
        G[v].push_back(w);
        G[w].push_back(v);
    }

    Grafo H(n+1, vector<Vertice>());
    for (int i = 0; i < Mh; i++) {
        int v, w;
        cin >> v >> w;
        H[v].push_back(w);
        H[w].push_back(v);
    }

    return Instancia{.G = G, .H = H};
}

void printGrafo(const Grafo& G) {
    for (int i = 1; i < G.size(); i++) {
        cout << "vecinos de " << i << ": ";
        for (int val : G[i]) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Imprime la solucion por stdout con formato
// maximo impacto
// colores de los vertices
void printSolucion(const Solucion& s) {
    cout << s.impacto << endl;
    for(int i = 1; i < s.coloreo.size(); i++) {
        cout << s.coloreo[i] << " ";
    }
    cout << endl;
}

// Dada una instancia y una selección de dos algoritmos (con entrada de memoria e iteraciones opcional) devuelve la
// solución obtenida.
Solucion getAlgorithmSolution(const Instancia& I, string algoritmo, string algoritmoInicial, int memoria, int iteraciones) {
    Solucion s;
    if (algoritmo == "W") {
        s = wyrnisticaDiferencialGolosa(I);
    } else if (algoritmo == "S-LF") {
        s = golosaSecuencialLF(I);
    } else if (algoritmo == "WP") {
        s = wyrnowerGolosa(I);
    } else if (algoritmo == "TSC-C") {
        Solucion solucionInicial = getAlgorithmSolution(I, algoritmoInicial, "", memoria, iteraciones);
        s = change::tabuColoreo(I, memoria, iteraciones, solucionInicial);
    } else if (algoritmo == "TSC-E") {
        Solucion solucionInicial = getAlgorithmSolution(I, algoritmoInicial, "", memoria, iteraciones);
        s = change::tabuEstructura(I, memoria, iteraciones, solucionInicial);
    } else if (algoritmo == "TSS-C") {
        Solucion solucionInicial = getAlgorithmSolution(I, algoritmoInicial, "", memoria, iteraciones);
        s = swap::tabuColoreo(I, memoria, iteraciones, solucionInicial);
    } else if (algoritmo == "TSS-E") {
        Solucion solucionInicial = getAlgorithmSolution(I, algoritmoInicial, "", memoria, iteraciones);
        s = swap::tabuEstructura(I, memoria, iteraciones, solucionInicial);
    } else if (algoritmo == "C") {
        s = pcmiConstructivaControl(I);
    }

    return s;
}

int main (int argc, char** argv) {
    map<string, string> algorithms = {
        {"C", "Control"},
        {"S-LF", "Secuencial LF"},
		{"W", "Wyrnistica"},
        {"WP", "Wyrnower - Wyrna Power"},
        {"TSC-C", "Tabú search con vecindad change memoria basada en coloreos"},
		{"TSC-E", "Tabú search con vecindad change memoria basada en estructura (vértices)"},
        {"TSS-C", "Tabú search con vecindad swap memoria basada en coloreos"},
		{"TSS-E", "Tabú search con vecindad swap memoria basada en estructura (vértices)"},
	};

    // Leo el argumento que indica el algoritmo. Tiene que haber al menos dos
	// porque el primero es el nombre del programa.
    if (argc < 2) {
        cout << "Uso:\n\t$ ./tp2 <algorithm>" << endl;
        return -1;
    }

    string algoritmo = argv[1];
    string algoritmoGoloso = "W";
    int memoria = 10;
    int iteraciones = 100;

    if (argc >= 3) {
        // Si es un algoritmo de tabu search, leo el algoritmo utilizado
        // para obtener la solución inicial
        algoritmoGoloso = argv[2];
        if (argc == 5) {
            memoria = stoi(argv[3]);
            iteraciones = stoi(argv[4]);
        }
    }

    if(algorithms.find(algoritmo) == algorithms.end()) {
        cout << "Algoritmo no encontrado: " << algoritmo << endl;
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algorithms) cerr << "- " << alg_desc.first << "\t" << alg_desc.second << endl;
		return -1;
    }

    // Leemos el input
    Instancia I = leerGrafos();

    auto start = chrono::steady_clock::now();

    // Principal
    Solucion s = getAlgorithmSolution(I, algoritmo, algoritmoGoloso, memoria, iteraciones);

    auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();    
    // Imprimimos el tiempo de ejecucion por stderr
    clog << total_time << endl;

    // Imprimimos la solucion
    printSolucion(s);

    return 0;
} 