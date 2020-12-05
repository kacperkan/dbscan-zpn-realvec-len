import matplotlib

matplotlib.use("Agg")

import logging
from pathlib import Path

import click
import matplotlib.pyplot as plt
import numpy as np
from sklearn import metrics
from sklearn.cluster import DBSCAN
from sklearn.datasets import make_blobs
from sklearn.preprocessing import StandardScaler


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


def produce(out: str):
    # Generate sample data
    out_path = Path(out)

    centers = [[1, 1], [-1, -1], [1, -1]]
    X, labels_true = make_blobs(
        n_samples=750, centers=centers, cluster_std=0.4, random_state=0
    )

    X = StandardScaler().fit_transform(X)

    db = DBSCAN(eps=0.3, min_samples=10).fit(X)
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
    logger.info(
        "Homogeneity: %0.3f" % metrics.homogeneity_score(labels_true, labels)
    )
    logger.info(
        "Completeness: %0.3f" % metrics.completeness_score(labels_true, labels)
    )
    logger.info(
        "V-measure: %0.3f" % metrics.v_measure_score(labels_true, labels)
    )
    logger.info(
        "Adjusted Rand Index: %0.3f"
        % metrics.adjusted_rand_score(labels_true, labels)
    )
    logger.info(
        "Adjusted Mutual Information: %0.3f"
        % metrics.adjusted_mutual_info_score(labels_true, labels)
    )
    logger.info(
        "Silhouette Coefficient: %0.3f" % metrics.silhouette_score(X, labels)
    )

    # Black removed and is used for noise instead.
    unique_labels = set(labels)
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

    with open(out_path / "data.arff", "w") as f:
        f.write(to_arff(X, labels))


@click.command()
@click.argument("out_path")
def main(out_path: str):
    produce(out_path)


if __name__ == "__main__":
    main()
