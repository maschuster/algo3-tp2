"""tests para el tepe"""

import os
import unittest
import subprocess

from typing import List

def read_output(out: str) -> (int, List[int]):
    """Lee un output del programa, por ej.
        1 
        4 1 6 1 4 3 
    """
    sout = out.split("\n")
    return int(sout[0]), list(map(int, sout[1].rstrip().split(" ")))

class TestNPM(unittest.TestCase):
    """Tests para NPM. Todos asumen que ya esta buildeado"""

    def test_control(self):
        tests = [
            {
                "name": "control facil",
                "algoritmo": "C",
                "instancia": "CMI_n6",
                "impacto": 0,
                "coloreo": list(range(1, 7)),
            },
            {
                "name": "control facil",
                "algoritmo": "C",
                "instancia": "CMI_n16",
                "impacto": 0,
                "coloreo": list(range(1, 17)),
            }
        ]

        for test in tests:
            with self.subTest(test["name"]):
                filename = f"../instancias/{test['instancia']}.in"
                res = subprocess.run(f"../build/main {test['algoritmo']} < {filename}", shell=True, capture_output=True)

                impacto, coloreo = read_output(res.stdout.rstrip().decode("utf-8"))
                assert impacto == test["impacto"], f"want: {test['impacto']}, got: {impacto}"
                assert coloreo == test["coloreo"], f"want: {test['coloreo']}, got: {coloreo}"
                #assert  == output(test), f"falló el test {test} en el algoritmo {test['algoritmo']}"


if __name__ == "__main__":
    unittest.main()