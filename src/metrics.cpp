#include "metrics.hpp"

#include <boost/foreach.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/combine.hpp>
#include <exception>
#include <map>
#include <set>
#include <unordered_set>

#include "dbscan.hpp"
#include "math.hpp"
#include "utils.hpp"

namespace dbscan {
double Purity::get_metric(
    const std::vector<int>& true_labels, const std::vector<int>& pred_labels,
    const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
    const std::vector<VecXd>& data_points) {
    std::unordered_map<int, std::vector<int>> pred_clusters;

    for (const auto& elem : pred_labels | boost::adaptors::indexed(0)) {
        pred_clusters[elem.value()].push_back(elem.index());
    }

    double metric_value = 0.0;

    BOOST_FOREACH (const auto& pair, pred_clusters) {
        std::unordered_map<int, int> cls_counts;
        BOOST_FOREACH (const auto& index, pair.second) {
            cls_counts[true_labels[index]]++;
        }

        int max_count = 0;

        BOOST_FOREACH (const auto& count_pair, cls_counts) {
            if (count_pair.second > max_count) {
                max_count = count_pair.second;
            }
        }
        metric_value += (double)max_count / ((double)true_labels.size() + EPS);
    }
    return metric_value;
}

double Rand::get_metric(
    const std::vector<int>& true_labels, const std::vector<int>& pred_labels,
    const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
    const std::vector<VecXd>& data_points) {
    int tp = 0, tn = 0;

    for (int i = 0; i < pred_labels.size(); ++i) {
        for (int j = i + 1; j < pred_labels.size(); ++j) {
            std::unordered_set<int> intersection;

            set_intersection(pred_cluster_assignments[i],
                             pred_cluster_assignments[j], intersection);
            if (true_labels[i] == true_labels[j] && intersection.size() > 0) {
                tp++;
            } else if (true_labels[i] != true_labels[j] &&
                       intersection.size() == 0) {
                tn++;
            }
        }
    }

    double metric_value = ((double)tp + (double)tn) /
                          (combinations(true_labels.size(), 2) + EPS);
    return metric_value;
}

double DaviesBouldin::get_metric(
    const std::vector<int>& true_labels, const std::vector<int>& pred_labels,
    const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
    const std::vector<VecXd>& data_points) {
    std::unordered_map<int, std::vector<int>> pred_clusters;
    std::set<int> unique_labels;

    for (const auto& elem : pred_labels | boost::adaptors::indexed(0)) {
        if (elem.value() == dbscan::NOISE_LABEL) {
            continue;
        }
        pred_clusters[elem.value()].push_back(elem.index());
        unique_labels.emplace(elem.value());
    }

    std::unordered_map<int, VecXd*> centroids;
    std::unordered_map<int, int> elem_counts;

    BOOST_FOREACH (const auto& pair, pred_clusters) {
        centroids[pair.first] = new VecXd(data_points[0].size());
        BOOST_FOREACH (const auto& index, pair.second) {
            centroids[pair.first]->add_(data_points[index]);
        }
        elem_counts[pair.first] += pair.second.size();
    }

    BOOST_FOREACH (const auto& pair, centroids) {
        centroids[pair.first]->div_((double)elem_counts[pair.first] + EPS);
    }

    std::unordered_map<int, double> average_distances;
    BOOST_FOREACH (const auto& pair, pred_clusters) {
        BOOST_FOREACH (const auto& index, pair.second) {
            double distance =
                (data_points[index] - (*centroids[pair.first])).length() /
                (elem_counts[pair.first] + EPS);
            average_distances[pair.first] += distance;
        }
    }

    double result = 0.0;
    double max;
    double temp_max;
    for (auto i : unique_labels) {
        max = 0.0;
        for (auto j : unique_labels) {
            if (i >= j) {
                continue;
            }
            temp_max = (average_distances[i] + average_distances[j]) /
                       ((*centroids[i] - *centroids[j]).length() + EPS);
            max = temp_max > max ? temp_max : max;
        }

        result += max;
    }
    BOOST_FOREACH (const auto& pair, centroids) { delete pair.second; }

    return result / ((double)unique_labels.size() + EPS);
}

double Silhouette::get_metric(
    const std::vector<int>& true_labels, const std::vector<int>& pred_labels,
    const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
    const std::vector<VecXd>& data_points) {
    std::unordered_map<int, std::vector<int>> pred_clusters;
    std::set<int> unique_labels;

    for (const auto& elem : pred_labels | boost::adaptors::indexed(0)) {
        if (elem.value() == dbscan::NOISE_LABEL) {
            continue;
        }
        pred_clusters[elem.value()].push_back(elem.index());
        unique_labels.emplace(elem.value());
    }
    if (unique_labels.size() <= 1) {
        return 0.0;
    }

    VecXd a_coeffs(data_points.size());
    VecXd b_coeffs(data_points.size());

    int point_label;
    double min_distance;
    double cur_distance;
    for (int i = 0; i < data_points.size(); ++i) {
        point_label = pred_labels[i];
        std::unordered_map<int, double> per_cluster_distance;
        BOOST_FOREACH (const auto& pair, pred_clusters) {
            if (pair.first == point_label) {
                for (auto other_point_index : pair.second) {
                    if (other_point_index == i) {
                        continue;
                    }
                    a_coeffs.data[i] +=
                        (data_points[i] - data_points[other_point_index])
                            .length() /
                        (pair.second.size() - 1);
                }
            } else {
                for (auto other_point_index : pair.second) {
                    cur_distance =
                        (data_points[i] - data_points[other_point_index])
                            .length() /
                        (pair.second.size() + EPS);
                    per_cluster_distance[pair.first] += cur_distance;
                }
            }
        }
        min_distance = 9999999999999999;
        BOOST_FOREACH (const auto& pair, per_cluster_distance) {
            min_distance =
                pair.second < min_distance ? pair.second : min_distance;
        }

        b_coeffs.data[i] = min_distance;
    }

    double result =
        ((b_coeffs - a_coeffs) / (b_coeffs.max(a_coeffs) + EPS)).mean();
    return result;
}

}  // namespace dbscan
