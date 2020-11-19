"""tests para el tepe"""

import os
import unittest
import subprocess
import pcmi

from typing import List


class TestNPM(unittest.TestCase):
    """Tests para NPM. Todos asumen que ya esta buildeado"""

    def tests(self):
        tests = [
            # control
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
            },

            # wyrna
            {
                "name": "wyrna",
                "algoritmo": "W",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [8, 2, 10, 2, 12, 13],
            },
        ]

        for test in tests:
            with self.subTest(test["name"]):
                res, _ = pcmi.run(test["algoritmo"], f"../instancias/{test['instancia']}.in")
                impacto, coloreo = res
                assert impacto == test["impacto"], f"want: {test['impacto']}, got: {impacto}"
                assert coloreo == test["coloreo"], f"want: {test['coloreo']}, got: {coloreo}"
                #assert  == output(test), f"falló el test {test} en el algoritmo {test['algoritmo']}"


if __name__ == "__main__":
    unittest.main()