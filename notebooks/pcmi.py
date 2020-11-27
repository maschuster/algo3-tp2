import subprocess
import collections

from datetime import datetime
from typing import List

Color = int
Result = collections.namedtuple('Result', 'impacto coloreo')
Debug = collections.namedtuple('Result', 'tiempo optimo it_efectivas')

def run(
        algorithm: str,
        instance_path: str,
        salida: str,
        initialAlgorithm: str = None,
        it: int = None,
        mem_size: int = None,
        percent: int = None,
        aspirar: bool = None,
        debug: bool = None,
        quiet: bool = None,
    ) -> (Result, Debug):
    """
    Corre el programa para el algoritmo e instancia dadas y devuelve el
    resultado junto con el tiempo de ejecucion.
    """
    args = {
        "-i": initialAlgorithm,
        "-m": mem_size,
        "-n": it,
        "-p": percent,
        "-a": aspirar,
        "-d": debug,
        "-q": quiet,
    }

    call = f"../build/main {algorithm}"
    for k, v in args.items():
        if v is not None:
            if type(v) is bool:
                v = "true" if v else "false"
            call += f" {k} {v}"
            
    result = subprocess.run(
        f"{call} < {instance_path}",
        shell=True, capture_output=True, text=True, check=True,
    )

    with open(salida, "r") as file:
        optimo = int(file.read().split("\n")[0])
    
    stderr = str(result.stderr).split('\n')
    it_efectivas = int(stderr[-3].split(":")[1])
    tiempo = float(stderr[-2].split(":")[1])

    if debug:
        iteraciones  = stderr[:-3]
        timestamp    = datetime.now().strftime("%d%m%Y%H%M%S")
        
        with open(f"outputs/{algorithm}-{initialAlgorithm}-{timestamp}.csv", "w") as file:
            file_lines = "\n".join(iteraciones)
            file.write(file_lines)

    return _read_output(result.stdout.rstrip(), quiet), Debug(tiempo=tiempo, optimo=optimo, it_efectivas=it_efectivas)

def _read_output(out: str, quiet: bool) -> Result:
    """Lee un output del programa, por ej.
        1 
        4 1 6 1 4 3 
    """
    sout = out.split("\n")
    if quiet:
        # no se printea el coloreo
        return Result(impacto=int(sout[0]), coloreo=[])

    return Result(impacto=int(sout[0]), coloreo=list(map(int, sout[1].rstrip().split(" "))))

def printear():
    print("Hola")
    
    return 0