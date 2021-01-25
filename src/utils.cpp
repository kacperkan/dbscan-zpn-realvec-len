#include "utils.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include "dataset.hpp"

namespace dbscan {
void set_intersection(const std::unordered_set<int>& set1,
                      const std::unordered_set<int>& set2,
                      std::unordered_set<int>& intersection) {
    for (auto i = set1.begin(); i != set1.end(); i++) {
        if (set2.find(*i) != set2.end()) {
            intersection.emplace(*i);
        }
    }
}

double combinations(int n, int k) {
    // based on
    // https://www.geeksforgeeks.org/space-and-time-efficient-binomial-coefficient/
    int res = 1;

    // Since C(n, k) = C(n, n-k)
    if (k > n - k) k = n - k;

    // Calculate value of
    // [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }

    return (double)res;
}

void dump_experiment(const std::string name, const std::string file_path,
                     const std::vector<DBSCANPoint>& point_stats,
                     const MetricStats& metric_stats, Dataset& dataset,
                     double epsilon, int min_pts) {
    std::filesystem::path experiment_path("experiments");
    experiment_path /= name;
    std::filesystem::create_directories(experiment_path);

    std::unordered_set<int> unique_labels;
    int num_core_points = 0;
    int num_border_points = 0;
    int num_noise_points = 0;
    int total_calculations = 0;
    int total_cluster_assignments = 0;

    std::ofstream out_file;
    out_file.open(experiment_path / "OUT");
    for (const auto& point_stat_with_index :
         point_stats | boost::adaptors::indexed(0)) {
        out_file << point_stat_with_index.index() << ",";
        for (int i = 0; i < dataset.cols(); ++i) {
            out_file << dataset(point_stat_with_index.index(), i) << ",";
        }
        out_file << point_stat_with_index.value().num_calculations << ",";
        if (point_stat_with_index.value().is_border_point) {
            out_file << 0 << ",";
            num_border_points++;
        } else if (point_stat_with_index.value().is_core_point) {
            out_file << 1 << ",";
            num_core_points++;
        } else if (point_stat_with_index.value().is_noise_point) {
            out_file << -1 << ",";
            num_noise_points++;
        }

        for (const auto& label_index :
             point_stat_with_index.value().cluster_assignments |
                 boost::adaptors::indexed(0)) {
            out_file << label_index.value();
            if (label_index.index() <
                point_stat_with_index.value().cluster_assignments.size() - 1) {
                out_file << ",";
            }
        }

        if (point_stat_with_index.index() < point_stats.size()) {
            out_file << "\n";
        }

        if (point_stat_with_index.value().label != NOISE_LABEL) {
            unique_labels.emplace(point_stat_with_index.value().label);
        }
        total_calculations += point_stat_with_index.value().num_calculations;
        total_cluster_assignments +=
            point_stat_with_index.value().cluster_assignments.size();
    }

    out_file.close();

    std::ofstream stat_file;

    stat_file.open(experiment_path / "STAT");

    stat_file << "input_file: " << file_path << "\n";
    stat_file << "Eps: " << epsilon << "\n";
    stat_file << "minPts: " << min_pts << "\n";
    stat_file << "runtimes_miliseconds:\n";

    for (int i = 0; i < TimeStats::get_instance().size(); ++i) {
        const std::pair<std::string, int> elem = TimeStats::get_instance()(i);
        stat_file << "\t-" << elem.first << ": " << elem.second << "\n";
    }

    stat_file << "total_runtime_miliseconds: "
              << TimeStats::get_instance().total_runtime() << "\n";

    stat_file << "num_discovered_clusters: " << unique_labels.size() << "\n";
    stat_file << "nun_discovered_core_points: " << num_core_points << "\n";
    stat_file << "nun_discovered_border_points: " << num_border_points << "\n";
    stat_file << "num_discovered_noise_points: " << num_noise_points << "\n";
    stat_file << "avg_num_calculations_of_similarity: "
              << (double)total_calculations / point_stats.size() << "\n";
    stat_file << "avg_num_cluster_assignments: "
              << (double)total_cluster_assignments / point_stats.size()
              << "\n";

    int tp = 0, tn = 0;

    for (int i = 0; i < point_stats.size(); ++i) {
        for (int j = i + 1; j < point_stats.size(); ++j) {
            std::unordered_set<int> intersection;

            set_intersection(point_stats[i].cluster_assignments,
                             point_stats[j].cluster_assignments, intersection);
            if (dataset.labels()[i] == dataset.labels()[j] &&
                intersection.size() > 0) {
                tp++;
            } else if (dataset.labels()[i] != dataset.labels()[j] &&
                       intersection.size() == 0) {
                tn++;
            }
        }
    }
    stat_file << "num_true_positives: " << tp << "\n";
    stat_file << "num_true_negatives: " << tn << "\n";
    stat_file << "purity: " << metric_stats.purity << "\n";
    stat_file << "rand: " << metric_stats.rand << "\n";
    stat_file << "silhouette: " << metric_stats.silhouette << "\n";
    stat_file << "davis_bouldin: " << metric_stats.davisbouldin << "\n";

    stat_file.close();
}

}  // namespace dbscan
