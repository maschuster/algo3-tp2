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
Solucion getAlgorithmSolution(const Instancia& I, Args args) {
    Solucion s;
    if (args.algoritmo == "W") {
        s = wyrnisticaDiferencialGolosa(I);
    } else if (args.algoritmo == "S-LF") {
        s = golosaSecuencialLF(I);
    } else if (args.algoritmo == "WP") {
        s = wyrnowerGolosa(I);
    } else if (args.algoritmo == "C") {
        s = pcmiConstructivaControl(I);
    } else {
        string algoritmo = args.algoritmo;

        args.algoritmo = args.algoritmoInicial;
        Solucion solucionInicial = getAlgorithmSolution(I, args);

        if (algoritmo == "TSC-C") {
            s = change::tabuColoreo(I, solucionInicial, args.tabu);
        } else if (algoritmo == "TSC-E") {
            s = change::tabuEstructura(I, solucionInicial, args.tabu);
        } else if (algoritmo == "TSS-C") {
            s = swap::tabuColoreo(I, solucionInicial, args.tabu);
        } else if (algoritmo == "TSS-E") {
            s = swap::tabuEstructura(I, solucionInicial, args.tabu);
        }
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

    Args args = Args {
        .algoritmo = argv[1],
        .algoritmoInicial = "W",
        .tabu = TabuArgs{
            .memoria = 10,
            .iteraciones = 100,
            .porcentajeVecindad = 100,
            .aspirar = false,
            .debug = false
        }
    };

    if (argc >= 3) {
        // Si es un algoritmo de tabu search, leo el algoritmo utilizado
        // para obtener la solución inicial
        // args.algoritmoInicial = argv[2];
        // if (argc == 7) {
        //     args.tabu.memoria = stoi(argv[3]);
        //     args.tabu.iteraciones = stoi(argv[4]);
        //     args.tabu.porcentajeVecindad = stoi(argv[5]);
        //     args.tabu.aspirar = (argv[6] == "true");
        // }
        for(int i = 2; i < argc; i+=2) {
            // i   es el arg
            // i+1 el val
            std::string arg = argv[i];
            std::string val = argv[i+1];

            // Como puede ser que c++ no te deje hacer switch en string?
            if (arg == "-i") args.algoritmoInicial = val;
            if (arg == "-m") args.tabu.memoria = stoi(val);
            if (arg == "-n") args.tabu.iteraciones = stoi(val);
            if (arg == "-p") args.tabu.porcentajeVecindad = stoi(val);
            if (arg == "-a") args.tabu.aspirar = (val == "true");
            if (arg == "-d") args.tabu.debug = (val == "true");
        }
    }

    if(algorithms.find(args.algoritmo) == algorithms.end()) {
        cout << "Algoritmo no encontrado: " << args.algoritmo << endl;
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algorithms) cerr << "- " << alg_desc.first << "\t" << alg_desc.second << endl;
		return -1;
    }

    // Leemos el input
    Instancia I = leerGrafos();

    auto start = chrono::steady_clock::now();

    // Principal
    Solucion s = getAlgorithmSolution(I, args);

    auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();    
    // Imprimimos el tiempo de ejecucion por stderr
    clog << "Tiempo de ejecucion:" << total_time << endl;

    // Imprimimos la solucion
    printSolucion(s);

    return 0;
} 