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
                "impacto": 4,
            },
            {
                "algoritmo": "W",
                "instancia": "CMI_n10",
                "impacto": 3,
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
                "impacto": 3,
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
                "impacto": 14,
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
                "impacto": 16,
            },
        ]

        for test in tests:
            name = f'{test["algoritmo"]}-{test["instancia"]}'
            with self.subTest(name):
                getIfExists = lambda d, k: d[k] if k in d else None
                res, _ = pcmi.run(
                    test["algoritmo"],
                    f"../instancias/{test['instancia']}.in",
                    f"../instancias/{test['instancia']}.out",
                    initialAlgorithm=getIfExists(test, "inicial"),
                    mem_size=getIfExists(test, "mem"),
                    it=getIfExists(test, "it"),
                    aspirar=getIfExists(test, "aspirar"),
                    percent=getIfExists(test, "percent"),
                )
                assert res.impacto == test["impacto"], f"want: {test['impacto']}, got: {res.impacto}"

if __name__ == "__main__":
    unittest.main()