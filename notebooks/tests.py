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
                "algoritmo": "C",
                "instancia": "CMI_n6",
                "impacto": 0,
                "coloreo": list(range(1, 7)),
            },
            {
                "algoritmo": "C",
                "instancia": "CMI_n16",
                "impacto": 0,
                "coloreo": list(range(1, 17)),
            },

            # wyrna
            {
                "algoritmo": "W",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [8, 2, 10, 11, 12, 2],
            },
            {
                "algoritmo": "W",
                "instancia": "CMI_n8",
                "impacto": 3,
                "coloreo": [1, 11, 1, 4, 4, 15, 7, 7],
            },
            {
                "algoritmo": "W",
                "instancia": "CMI_n10",
                "impacto": 2,
                "coloreo": [1, 13, 1, 4, 16, 17, 18, 19, 20, 4],
            },

            # wyrna P
            {
                "algoritmo": "WP",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [8, 2, 10, 11, 12, 2],
            },
            {
                "algoritmo": "WP",
                "instancia": "CMI_n8",
                "impacto": 4,
                "coloreo": [1, 11, 1, 4, 4, 1, 7, 7],
            },
            {
                "algoritmo": "WP",
                "instancia": "CMI_n10",
                "impacto": 2,
                "coloreo": [1, 13, 1, 4, 16, 17, 18, 19, 20, 4],
            },

            # SLF
            {
                "algoritmo": "S-LF",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [2, 1, 3, 3, 2, 1],
            },
            {
                "algoritmo": "S-LF",
                "instancia": "CMI_n12",
                "impacto": 7,
                "coloreo": [1, 3, 1, 2, 2, 1, 2, 3, 3, 1, 4, 2],
            },

            # TS-E
            {
                "algoritmo": "TS-E",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [12, 2, 10, 11, 12, 2],
            },
            {
                "algoritmo": "TS-E",
                "instancia": "CMI_n12",
                "impacto": 13,
                "coloreo": [1, 5, 5, 4, 5, 3, 3, 1, 5, 5, 5, 3],
            },

            # TS-C
            {
                "algoritmo": "TS-C",
                "instancia": "CMI_n6",
                "impacto": 1,
                "coloreo": [12, 2, 10, 11, 12, 2],
            },
            {
                "algoritmo": "TS-C",
                "instancia": "CMI_n12",
                "impacto": 13,
                "coloreo": [1, 5, 5, 4, 5, 3, 3, 1, 5, 5, 5, 3],
            },
        ]

        for test in tests:
            name = f'{test["algoritmo"]}-{test["instancia"]}'
            with self.subTest(name):
                res, _ = pcmi.run(test["algoritmo"], f"../instancias/{test['instancia']}.in")
                impacto, coloreo = res
                assert impacto == test["impacto"], f"want: {test['impacto']}, got: {impacto}"
                assert coloreo == test["coloreo"], f"want: {test['coloreo']}, got: {coloreo}"

if __name__ == "__main__":
    unittest.main()