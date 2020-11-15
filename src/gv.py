"""gv - Graph Viewer

Permite ver grafos.

gv <file> -
"""

import click
import seaborn as sns
import networkx as nx
import matplotlib.colors
import matplotlib.pyplot as plt

__version__ = "0.0.1"

@click.group() # para poder hacer cli.command()
@click.version_option(__version__)
def cli():
    """gv es un programa para visualizar grafos coloreados."""
    pass

@cli.command()
@click.option("--graph", "-g", type=click.File(mode='r'))
@click.option("--color", "-c", type=click.File(mode='r')) # -: stdin
def view(graph, color):
    # graph y color son archivos ya abiertos.
    graph = graph.read().split('\n')
    colors = color.read().split('\n')

    # graph tiene el formato
    #   n Mg Mh
    #   Mg lineas con aristas u v
    #   Mh lineas con aristas u v

    G = nx.Graph()
    H = nx.Graph()

    # Leemos y agregamos los nodos, ignorando m_H
    n, m_G, _ = map(int, graph[0].split(" "))
    G.add_nodes_from(map(str, range(1, n+1)))
    H.add_nodes_from(map(str, range(1, n+1)))

    # Leemos y agregamos las aristas
    G_edges = map(lambda e: e.split(" "), graph[1:m_G+1])
    H_edges = map(lambda e: e.split(" "), graph[m_G+1:])
    G.add_edges_from(G_edges)
    H.add_edges_from(H_edges)

    # Leemos y agregamos los colores
    # colors tiene el formato
    #   maximo impacto
    #   n colores (1 para cada vert) separados por espacio
    impacto = colors[0]

    coloreo = list(map(int, colors[1].rstrip().split(" ")))
    palette = sns.color_palette(
        palette="pastel",
        n_colors=len(coloreo)+1,
    )
    coloreo = list(map(lambda c: matplotlib.colors.rgb2hex(palette[c]), coloreo))
    options = {
        "font_size": 10,
        "node_size": 1000,
        "node_color": coloreo,
        "pos": nx.circular_layout(G),
        "edgecolors": "black",
        "linewidths": 3,
        "width": 4,
    }

    
    fig, axes = plt.subplots(1, 2, figsize=(10,5))
    nx.draw_networkx(G, **options, ax=axes[0])
    nx.draw_networkx(H, **options, ax=axes[1])
    axes[0].set_title(f"G, impacto: {impacto}")
    axes[1].set_title("H")

    for ax in axes:
        ax.margins(0.20)
        ax.axis("off")

    fig.tight_layout()
    plt.show()


if __name__ == "__main__":
    cli()