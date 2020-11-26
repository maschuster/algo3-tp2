import subprocess

from datetime import datetime
from typing import List

Color = int
Result = (int, List[Color])

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
    ) -> (Result, float):
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
    
    if debug:
        salida      = str(result.stderr).split("\n")
        iteraciones = salida[:-2]
        resultado   = float(salida[-2])
        timestamp   = datetime.now().strftime("%d%m%Y%H%M%S")
        
        with open(f"outputs/{algorithm}-{initialAlgorithm}-{timestamp}.csv", "w") as file:
            file_lines = "\n".join(iteraciones)
            file.write(file_lines)
    else:
        resultado = float(result.stderr)

    return _read_output(result.stdout.rstrip()), resultado, optimo

def _read_output(out: str) -> Result:
    """Lee un output del programa, por ej.
        1 
        4 1 6 1 4 3 
    """
    sout = out.split("\n")
    return int(sout[0]), list(map(int, sout[1].rstrip().split(" ")))

def printear():
    print("Hola")
    
    return 0