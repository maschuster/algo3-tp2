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

- secuencial simple ordenando por grados

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
  - swap
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

- Para visualizar el recorrido del algoritmo (extra: comparar con uno que se quede
en locales, como hill climber):
  - Guardar en cada iteracion el coloreo que fue el maximo junto con el impacto
  - Calcular una mat. de distancias entre todos los coloreos
  - Graficarlos en 3d. (magia)

### Metricas

Usar gap relativo / absoluto del impacto obtenido con el maximo conocido.

### Ideas random

- Agregar un "reset" a random en tabu search
- **Vecindad alternativa**: Tabu: solo achicar numero de colores para vecindad te limita, buscar otra forma
que no lo haga y ver si es mejor

### Exp 1 - Elección de goloso

#### Parte A

- Probar los dos golosos y la random (control) solos para todos
  - eficiencia
  - tiempo
- Usar con ambas tabu search
  - ver si se come todo o se nota la diferencia
  - Gap por iteraciones  

#### Parte B

Encontrar el mejor goloso para usar con tabu search.

Preguntas:

- Mayor impacto en solucion inicial mejora solucion final? O te condiciona mucho
- Hace diferencia en la eficiencia o el grueso del tiempo se lo lleva tabu?

Hip: El que de mejor impacto va a hacer que funcione mejor tabu search, y no va
a cambiar el tiempo.

### Exp 2 - Optimizacion de tabu

Optimizar meta-parametros de tabu search con alguna heurística (hill climber)

- Tamaño de la vecindad que se considera
- Iteraciones
- Tamaño memoria
- Tipo de la memoria
- Aspirar (estr)

Probar combinaciones arrancando desde algo (justificadamente) razonable.

Intuicion / hipotesis

- Mas iteraciones van a dar mejores resultados hasta un punto que no cambia
- Aspirar haria que explore mas para estructural

Visualizaciones:

- Evolucion de impacto segun iteracion

### Exp 3 (punto 5) - Nuevas instancias

- Elegir el mejor de cada uno y correrlo para nuevas, comparando eficiencia y
  tiempo de ejecucion.

- Exacto para chiquitas/medianas


## Optimos

build/main TSC-C W 300 100 35 < instancias/CMI_n30.in

## TODO

- Informe
- Definir rangos
- Visualizar por iteraciones
