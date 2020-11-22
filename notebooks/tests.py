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
            },
            {
                "algoritmo": "C",
                "instancia": "CMI_n16",
                "impacto": 0,
            },

            # wyrna
            {
                "algoritmo": "W",
                "instancia": "CMI_n6",
                "impacto": 1,
            },
            {
                "algoritmo": "W",
                "instancia": "CMI_n8",
                "impacto": 3,
            },
            {
                "algoritmo": "W",
                "instancia": "CMI_n10",
                "impacto": 2,
            },

            # wyrna P
            {
                "algoritmo": "WP",
                "instancia": "CMI_n6",
                "impacto": 1,
            },
            {
                "algoritmo": "WP",
                "instancia": "CMI_n8",
                "impacto": 4,
            },
            {
                "algoritmo": "WP",
                "instancia": "CMI_n10",
                "impacto": 2,
            },

            # SLF
            {
                "algoritmo": "S-LF",
                "instancia": "CMI_n6",
                "impacto": 1,
            },
            {
                "algoritmo": "S-LF",
                "instancia": "CMI_n12",
                "impacto": 7,
            },

            # TSC-E
            {
                "algoritmo": "TSC-E",
                "instancia": "CMI_n6",
                "impacto": 1,
            },
            {
                "algoritmo": "TSC-E",
                "instancia": "CMI_n12",
                "mem_size": 1,
                "it": 1000,
                "impacto": 12,
            },

            # TSC-C
            {
                "algoritmo": "TSC-C",
                "instancia": "CMI_n6",
                "impacto": 1,
            },
            {
                "algoritmo": "TSC-C",
                "instancia": "CMI_n12",
                "mem_size": 1,
                "it": 1000,
                "impacto": 12,
            },
        ]

        for test in tests:
            name = f'{test["algoritmo"]}-{test["instancia"]}'
            with self.subTest(name):
                res, _, _ = pcmi.run(
                    test["algoritmo"],
                    f"../instancias/{test['instancia']}.in",
                    f"../instancias/{test['instancia']}.out",
                    initialAlgorithm=test["inicial"] if "inicial" in test else None,
                    mem_size=test["mem"] if "mem" in test else None,
                    it=test["it"] if "it" in test else None,
                )
                impacto, _ = res
                assert impacto == test["impacto"], f"want: {test['impacto']}, got: {impacto}"

if __name__ == "__main__":
    unittest.main()