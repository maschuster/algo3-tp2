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
  - cambiar el color de un vertice a uno que ya se use, pero que no lleve a un
    coloreo invalido.
    - La vecindad de las soluciones tiene <= colores
  - incrementar cantidad de colores
- Memoria
  - estructura.
    - (vertice, color) {vertice <- color}
  - solucion
    - representacion del coloreo
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
  
  return sol
```

## Experimentos

### Visualizaciones

- Impacto en cada iteracion (para ver como evoluciona, si sale de max. locales)

Para visualizar el recorrido del algoritmo (extra: comparar con uno que se quede
en locales, como hill climber):

- Guardar en cada iteracion el coloreo que fue el maximo junto con el impacto
- Calcular una mat. de distancias entre todos los coloreos
- Graficarlos en 3d. (magia)

### Exp 1

- Probar los dos golosos y la random (control) solos para todos
  - eficiencia
  - tiempo
- Usar con ambas tabu search
  - ver si se come todo o se nota la diferencia
  - Gap por iteraciones

### Exp 2

Optimizar meta-parametros de tabu search

- Cantidad de vecindad que se considera
- Iteraciones
- Tamaño memoria
- Aspirar

Usar metaheuristicas para establecerlos, por ejemplo hillclimber

### Exp 3

Tabu, solo achicar numero de colores para vecindad te limita, buscar otra forma
que no lo haga y ver si es mejor.

### Exp 4

- Elegir el mejor de cada uno y correrlo para nuevas, comparando eficiencia y
  tiempo de ejecucion.
