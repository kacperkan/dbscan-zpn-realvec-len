import logging
from pathlib import Path
from typing import Tuple

import click
import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import DBSCAN
from sklearn.datasets import make_blobs


def to_arff(data: np.ndarray, labels: np.ndarray) -> str:
    lines = [
        "% Data from J. Handl",
        "%",
        "%",
        "@RELATION 2d-20c-no0",
        "",
        "@ATTRIBUTE x REAL",
        "@ATTRIBUTE y REAL",
        "@ATTRIBUTE class {-1, 0, 1, 2}",
        "",
        "@DATA",
    ]
    for coords, lab in zip(data, labels):
        lines.append("{:.5f},{:.5f},{:d}".format(coords[0], coords[1], lab))
    return "\n".join(lines)


def get_even_simpler_dataset() -> Tuple[np.ndarray, np.ndarray]:
    left_points, right_points = [], []
    num_samples = 20
    y_coords = np.linspace(-1, 1, num=num_samples // 2)
    labels = [0] * (num_samples // 2) + [1] * (num_samples // 2)
    for i in range(num_samples // 2):
        left_points.append((-1.0, y_coords[i]))
        right_points.append((1.0, y_coords[i]))

    return (
        np.concatenate(
            (np.array(left_points), np.array(right_points)), axis=0
        ),
        np.array(labels),
    )


def evaluate(
    X: np.ndarray, labels_true: np.ndarray, out_path: Path, min_samples=10
):
    db = DBSCAN(eps=0.3, min_samples=min_samples).fit(X)
    core_samples_mask = np.zeros_like(db.labels_, dtype=bool)
    core_samples_mask[db.core_sample_indices_] = True
    labels = db.labels_

    # Number of clusters in labels, ignoring noise if present.
    n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
    n_noise_ = list(labels).count(-1)

    logger = logging.getLogger("Python DBSCAN")
    logger.setLevel(logging.INFO)
    logger.addHandler(logging.FileHandler(out_path / "log.log"))
    logger.addHandler(logging.StreamHandler())

    logger.info("Estimated number of clusters: %d" % n_clusters_)
    logger.info("Estimated number of noise points: %d" % n_noise_)

    # Black removed and is used for noise instead.
    unique_labels = set(labels)
    plt.figure()
    colors = [
        plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique_labels))
    ]
    for k, col in zip(unique_labels, colors):
        if k == -1:
            # Black used for noise.
            col = [0, 0, 0, 1]

        class_member_mask = labels == k
        xy = X[class_member_mask]
        plt.plot(
            xy[:, 0],
            xy[:, 1],
            "o",
            markerfacecolor=tuple(col),
            markeredgecolor="k",
            markersize=6,
        )

    plt.title("Estimated number of clusters: %d" % n_clusters_)
    plt.savefig(out_path / "plot.png")
    plt.close()


def produce(out: str):
    plt.switch_backend("agg")
    # Generate sample data
    out_path = Path(out)
    out_path.mkdir(parents=True, exist_ok=True)

    centers = [[1, 1], [-1, -1], [1, -1]]
    X, labels_true = make_blobs(
        n_samples=750, centers=centers, cluster_std=0.4, random_state=0
    )

    evaluate(X, labels_true, out_path)

    with open(out_path / "data.arff", "w") as f:
        f.write(to_arff(X, labels_true))

    simpler_path = out_path.parent / (out_path.name + "_simpler")
    simpler_path.mkdir(parents=True, exist_ok=True)

    X, labels_true = get_even_simpler_dataset()

    evaluate(X, labels_true, simpler_path, min_samples=1)
    with open(simpler_path / "data.arff", "w") as f:
        f.write(to_arff(X, labels_true))


@click.command()
@click.argument("out_path")
def main(out_path: str):
    produce(out_path)


if __name__ == "__main__":
    main()
