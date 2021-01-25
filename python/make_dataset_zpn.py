import argparse
from typing import List, Optional

import numpy as np
from functional import seq


class Parser:
    def __init__(self):
        super().__init__()
        self.lines: List[str] = []
        self.mean: Optional[np.ndarray] = None
        self.std: Optional[np.ndarray] = None

    def get_mean_std(self, path: str, has_labels: bool):
        with open(path, "r") as f:
            lines = seq(f.read().split("\n")).map(str.strip).list()

        total_feats = []
        for line in lines:
            if line.startswith("@") or line.startswith("%") or len(line) == 0:
                continue
            comps = line.split(",")
            data_slice = slice(0, len(comps) - 1 if has_labels else len(comps))
            new_features = []
            for feat in comps[data_slice]:
                feat_num = float(feat)
                new_features.append(feat_num)

            total_feats.append(new_features)

        data = np.array(total_feats)
        self.mean = np.mean(data, axis=0)
        self.std = np.std(data, axis=0)

    def parse_lines(self, path: str, has_labels: bool):
        self.get_mean_std(path, has_labels)
        assert self.mean is not None and self.std is not None
>>>>>>> f6866d97359aa8b172fcf25ba9f824fc356f3241
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
                for feat_index, feat in enumerate(comps[data_slice]):
                    feat_num = float(feat)
                    min_bound, max_bound = (
                        self.mean[feat_index] - self.std[feat_index] / 2,
                        self.mean[feat_index] + self.std[feat_index],
                    )
                    if feat_num <= (min_bound):
                        new_feat_num = "-1.0"
                    elif min_bound < feat_num <= max_bound:
                        new_feat_num = "0.0"
                    else:
                        new_feat_num = "1.0"
                    new_features.append(new_feat_num)

                comps[data_slice] = new_features
                new_lines.append(",".join(comps))
        self.lines = new_lines

    def output_to_file(self, path: str):
        with open(path, "w") as f:
            f.write("\n".join(self.lines))


def convert_dataset(input_path: str, output_path: str, has_labels: bool):
    data_parser = Parser()
    data_parser.parse_lines(input_path, has_labels)
    data_parser.output_to_file(output_path)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("dataset_path")
    parser.add_argument("output_path")
    parser.add_argument("--has_labels", action="store_true")

    args = parser.parse_args()

    convert_dataset(args.dataset_path, args.output_path, args.has_labels)


if __name__ == "__main__":
    main()
