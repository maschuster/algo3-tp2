# algo3-tp2

## Integrantes

| Integrante             | LU     | Mail                     |
| ---------------------- | ------ | ------------------------ |
| Cerdeira, Elías Nahuel | 692/12 | eliascerdeira@gmail.com  |
| Gianatiempo, Octavio   | 280/10 | ogianatiempo@gmail.com   |
| Panichelli, Manuel     | 72/18  | panicmanu@gmail.com      |
| Schuster, Martín       | 208/18 | m.a.schuster98@gmail.com |

## Implementación

La implementación de los algoritmos en c++ se encuentra en [`src/`](src/)

- Para compilar, basta con hacer `make`. El binario se guarda en `build/main`.
- Para correr los [tests](notebooks/tests.py), `make test`.

La interfaz del programa es

```bash
build/main <algorithm> <tabu_args>
```

Donde el algoritmo puede ser:

- `C`
- `S-LF`
- `W`
- `WP`
- `TSC-C`
- `TSC-E`
- `TSS-C`
- `TSS-E`

Y los parametros para los algoritmos tabu son

- `-i`: Algoritmo goloso inicial
- `-m`: Tamaño de la memoria
- `-n`: Cantidad de iteraciones
- `-p`: Porcentaje de la veciundad
- `-a` (true / false): Si aspirar o no
- `-d` (true / false): Si prender el modo debug (printear el impacto y coloreo
  por iteración).
- `-q` (true / false): Quiet, no imprimir el coloreo (por eficiencia)

Espera que le llegue la instancia por stdin, con el formato de las instancias de
ejemplo en [`instancias/`](instancias/)

Por ej,

```bash
$ build/main TSC-E -i WP -n 100 -m 75 < instancias/CMI_n30.in
Iteraciones efectivas:58
Tiempo de ejecucion:2.59066
42
1 1 3 3 1 6 11 1 3 3 11 25 1 11 16 16 16 25 6 11 11 16 1 25 25 16 11 11 25 16
```

## Experimentación y análisis

Dentro del [notebook de la experimentación](notebooks/experimentacion.ipynb) se
corren los algoritmos y se recolectan los datos para luego ser
[analizados](analisis/).

Además, para ver el comportamiento de los algoritmos, se puede usar el programa
`gv` que dibuja un grafo coloreado:

```bash
build/main W < patologicos/wyrna.in | python3 src/gv.py view --graph patologicos/wyrna.in -o patologicos/wyrna.out -l circular --img patologicos/wyrna.png
```
