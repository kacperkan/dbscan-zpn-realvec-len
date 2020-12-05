from pathlib import Path
from typing import Tuple

import matplotlib.pyplot as plt
import numpy as np
import skimage.io
import skimage.transform
from matplotlib import cm
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas

BORDER_POINT = "border"
NOISE_POINT = "noise"


def read_data(path: str) -> Tuple[np.ndarray, np.ndarray]:
    with open(path) as f:
        lines = [line for line in f.read().split("\n") if len(line) > 0]
    coords = []
    labels = []
    for line in lines:
        components = line.split(",")
        x, y = float(components[1]), float(components[2])
        point_type = int(components[4])
        assignments = components[5:]
        if point_type == -1:
            labels.append(NOISE_POINT)
        elif point_type == 0:
            labels.append(BORDER_POINT)
        else:
            labels.append(assignments[-1])

        coords.append((x, y))

    return np.array(coords), np.array(labels)


def visualize(path: str):
    plt.switch_backend("agg")
    x, y = read_data(path)

    colormap = cm.get_cmap("tab10")
    fig, ax = plt.subplots(1, 1, figsize=(8, 6))
    canvas = FigureCanvas(fig)

    for label in np.unique(y):
        ix = np.where(y == label)[0]
        ax.scatter(x[ix, 0], x[ix, 1], s=15, label=label, cmap=colormap)
    ax.legend()
    canvas.draw()
    image = np.frombuffer(canvas.tostring_rgb(), dtype=np.uint8).reshape(
        canvas.get_width_height()[::-1] + (3,)
    )
    skimage.io.imsave(Path(path).parent / "vis.png", image)


def main():
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("path")

    args = parser.parse_args()

    visualize(args.path)


if __name__ == "__main__":
    main()
