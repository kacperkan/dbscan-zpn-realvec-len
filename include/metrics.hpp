#pragma once
#include <string>
#include <unordered_set>
#include <vector>

#include "math.hpp"

namespace dbscan {

class Metric {
   public:
    Metric(std::string name) : m_name(name) {}
    ~Metric() {}

    inline std::string name() { return m_name; }

    virtual double get_metric(
        const std::vector<int>& true_labels,
        const std::vector<int>& pred_labels,
        const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
        const std::vector<VecXd>& data_points) = 0;

   private:
    std::string m_name;
};

class Purity : Metric {
   public:
    Purity() : Metric("purity") {}
    double get_metric(
        const std::vector<int>& true_labels,
        const std::vector<int>& pred_labels,
        const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
        const std::vector<VecXd>& data_points);
};

class Rand : Metric {
   public:
    Rand() : Metric("rand") {}
    double get_metric(
        const std::vector<int>& true_labels,
        const std::vector<int>& pred_labels,
        const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
        const std::vector<VecXd>& data_points);
};

class DaviesBouldin : Metric {
   public:
    DaviesBouldin() : Metric("davies-bouldin") {}
    double get_metric(
        const std::vector<int>& true_labels,
        const std::vector<int>& pred_labels,
        const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
        const std::vector<VecXd>& data_points);
};

class Silhouette : Metric {
   public:
    Silhouette() : Metric("silhouette") {}
    double get_metric(
        const std::vector<int>& true_labels,
        const std::vector<int>& pred_labels,
        const std::vector<std::unordered_set<int>>& pred_cluster_assignments,
        const std::vector<VecXd>& data_points);
};

}  // namespace dbscan
