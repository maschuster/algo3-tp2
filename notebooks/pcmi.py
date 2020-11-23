import subprocess

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
    }

    call = f"../build/main {algorithm}"
    for k, v in args.items():
        if v is not None:
            call += f" {k} {v}"

    result = subprocess.run(
        f"{call} < {instance_path}",
        shell=True, capture_output=True, text=True, check=True,
    )

    with open(salida, "r") as file:
        optimo = file.read().split("\n")[0]

    return _read_output(result.stdout.rstrip()), float(result.stderr), optimo

def _read_output(out: str) -> Result:
    """Lee un output del programa, por ej.
        1 
        4 1 6 1 4 3 
    """
    sout = out.split("\n")
    return int(sout[0]), list(map(int, sout[1].rstrip().split(" ")))