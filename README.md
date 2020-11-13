# algo3-tp2

TP2 de Algoritmos y Estructuras de datos III de Computación (Exactas, UBA)

**Entrega**: `29/11/20`

## TODO

### Informe

- Describir problema y dar situaciones de la vida real
- Explicar detalladamente cada algoritmo
- Calcular complejidad en el peor caso
- Experimentación para cada algoritmo
- Experimentación comparando entre algoritmos

### Código

- I/O: `main.cpp` solo para IO y a otros files para algoritmos.
- Algoritmos de PCMI
  - Al menos dos heurísticas constructivas golosas
  - Tabú search con memoria basada en últimas soluciones exploradas
  - Tabú search con memoria basada en estructura (Vértices)

- Repr del grafo:

### Heuristicas constructivas golosas

Construir una sol factible a partir de la nada

Un coloreo valido es solucion factible. Te interesa maximizar el impacto

- Control: 1 color a cada nodo

- Heuristica secuencial
  - A cada nodo aislado ponerle un color diferente, total no nos importa la cantidad de colores.
  - Recorrer c.cs (dfs, bfs) de H e intentar poner el mismo color

- wyrnistica diferencial: Para cada arista (v,w) en H, si (v, w) no pertenece a G entonces pinto v y w de igual color.

### Taboh search

taboh sech

Pseudocodigo

```text
busquedaLocal()
  in: ...
  out: ...

  s = inicial(G)
  while existe s' \in N(s) con f(s') < f(s)
    elegir s'
  
  return s
```

- Criterio de parada: iteraciones
- Vecindad
  - cantidad de elementos a considerar
- Memoria
  - estructura
  - solucion
- Tal vez funcion de aspiracion

Pseudocodigo:

```python
tabuSearch(G: grafo, t: int):
  sol = solucionInicial(g)
  best = sol
  memoria = iniciarMemoria(maxSize=t)

  while criterio de parada:
    # obtener la vecindad
    vecinos = neighbors(sol)

    # el mejor sin contar los ya vistos
    sol = mejor(vecinos, memoria)

    memoria.recordar(sol)
    if costo(s) < costo(best):
      best = sol
  
  return s
```