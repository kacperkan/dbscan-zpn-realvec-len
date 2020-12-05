import argparse
from typing import List

from functional import seq


class Parser:
    def __init__(self, thresholds: List[float], labels: List[int]) -> None:
        super().__init__()

        self.thresholds = thresholds
        self.labels = labels
        self.lines: List[str] = []

    def parse_lines(self, path: str, has_labels: bool):
        new_lines = []

        with open(path, "r") as f:
            lines = seq(f.read().split("\n")).map(str.strip).list()

        for line in lines:
            if line.startswith("@") or line.startswith("%") or len(line) == 0:
                new_lines.append(line)
            else:
                comps = line.split(",")
                data_slice = slice(
                    0, len(comps) - 1 if has_labels else len(comps)
                )
                new_features = []
                for feat in comps[data_slice]:
                    feat_num = float(feat)
                    if feat_num < self.thresholds[0]:
                        new_feat_num = str(float(self.labels[0]))
                    elif self.thresholds[0] <= feat_num < self.thresholds[1]:
                        new_feat_num = str(float(self.labels[1]))
                    else:
                        new_feat_num = str(float(self.labels[2]))
                    new_features.append(new_feat_num)

                comps[data_slice] = new_features
                new_lines.append(",".join(comps))
        self.lines = new_lines

    def output_to_file(self, path: str):
        with open(path, "w") as f:
            f.write("\n".join(self.lines))


def convert_dataset(
    input_path: str,
    output_path: str,
    thresholds: List[float],
    labels: List[int],
    has_labels: bool,
):
    data_parser = Parser(thresholds, labels)
    data_parser.parse_lines(input_path, has_labels)
    data_parser.output_to_file(output_path)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("dataset_path")
    parser.add_argument("output_path")
    parser.add_argument(
        "--thresholds", nargs=2, help="Splits of the data", type=float
    )
    parser.add_argument(
        "--labels", nargs=3, help="Labels for each of the split", type=int
    )
    parser.add_argument("--has_labels", action="store_true")

    args = parser.parse_args()

    convert_dataset(
        args.dataset_path,
        args.output_path,
        list(args.thresholds),
        list(args.labels),
        args.has_labels,
    )


if __name__ == "__main__":
    main()
