#pragma once

#include <deque>
#include <memory>
#include <unordered_set>

#include "dataset.hpp"

namespace dbscan {

enum NeighbourPrefiltering { none, realveclen, zpnveclen };

const int UNDEFINED_LABEL = -1;
const int NOISE_LABEL = -2;

const int BORDER_POINT = -3;
const int CORE_POINT = -4;
const int OUTLIER_POINT = -5;

struct DBSCANPoint {
    std::shared_ptr<VecXd> point;
    int label = UNDEFINED_LABEL;
    bool is_border_point = false;
    bool is_core_point = false;
    bool is_noise_point = false;
    double length = 0.0;
    int num_calculations = 0;
    std::unordered_set<int> cluster_assignments;
};

class BaseFilterer {
   public:
    BaseFilterer() {}
    ~BaseFilterer() {}
    virtual void prefilter_possible_neighbours(
        const std::vector<DBSCANPoint>& points,
        const std::vector<int>& sorted_indices,
        std::vector<int>& new_sorted_indices, int point_index_in_sorted,
        double eps) = 0;

   protected:
    bool is_in_bounds(const DBSCANPoint& point, double lower_bound,
                      double upper_bound);
};
class RealVecLenFilterer : public BaseFilterer {
   public:
    RealVecLenFilterer() : BaseFilterer() {}
    void prefilter_possible_neighbours(const std::vector<DBSCANPoint>& points,
                                       const std::vector<int>& sorted_indices,
                                       std::vector<int>& new_sorted_indices,
                                       int point_index_in_sorted, double eps);
};

class DummyFilterer : public BaseFilterer {
   public:
    DummyFilterer() : BaseFilterer() {}
    void prefilter_possible_neighbours(const std::vector<DBSCANPoint>& points,
                                       const std::vector<int>& sorted_indices,
                                       std::vector<int>& new_sorted_indices,
                                       int point_index_in_sorted, double eps);
};

class ZPNVecLenFilterer : public BaseFilterer {
   public:
    ZPNVecLenFilterer() : BaseFilterer() {}
    void prefilter_possible_neighbours(const std::vector<DBSCANPoint>& points,
                                       const std::vector<int>& sorted_indices,
                                       std::vector<int>& new_sorted_indices,
                                       int point_index_in_sorted, double eps);
};

class AbstractDBSCAN {
   public:
    AbstractDBSCAN(double eps, int min_pts,
                   NeighbourPrefiltering prefilter_type)
        : m_eps(eps),
          m_min_pts(min_pts),
          m_prefiltering_type(prefilter_type) {}
    ~AbstractDBSCAN() {
        if (was_fitted) {
            delete[] m_labels;
            delete[] m_point_types;
        }
    }

    AbstractDBSCAN& fit(Dataset& dataset);

    void fit_predict(
        Dataset& dataset, std::vector<int>& out_labels,
        std::vector<int>& point_types,
        std::vector<std::unordered_set<int>>& cluster_assignments);

    int num_labels() { return m_num_labels; }
    double eps() { return m_eps; }
    int min_pts() { return m_min_pts; }

    const std::vector<DBSCANPoint>& data_points() { return m_data_points; }

    std::vector<int> labels();
    std::vector<int> point_types();
    std::vector<std::unordered_set<int>> cluster_assignments();

   protected:
    int m_num_labels = 0;
    int m_num_data = 0;
    int* m_labels;
    int* m_point_types;
    std::vector<std::unordered_set<int>> m_cluster_assignments;
    std::vector<DBSCANPoint> m_data_points;

    double m_eps;
    int m_min_pts;
    bool was_fitted;

    NeighbourPrefiltering m_prefiltering_type;

    void convert_to_dbscan_points(Dataset& dataset);
    void sort_points(const std::vector<DBSCANPoint>& container,
                     std::vector<int>& indices);
    void convert_points_to_labels(std::vector<DBSCANPoint>& points);

    virtual void get_neighbours(std::vector<DBSCANPoint>& points,
                                int point_index_in_sorted,
                                const std::vector<int>& sorted_indices,
                                std::deque<int>& container) = 0;
    virtual double get_distance(const DBSCANPoint& point_1,
                                const DBSCANPoint& point_2) = 0;

    void prefilter_possible_neighbours(
        const std::vector<DBSCANPoint>& points,
        const std::vector<int>& sorted_indices,
        std::vector<int>& prefiltered_indices_of_sorted_indices,
        int point_index_in_sorted);
};

class DBSCAN : public AbstractDBSCAN {
   public:
    DBSCAN(double eps, int min_pts, NeighbourPrefiltering prefilter_type)
        : AbstractDBSCAN(eps, min_pts, prefilter_type) {}

   protected:
    void get_neighbours(std::vector<DBSCANPoint>& points,
                        int point_index_in_sorted,
                        const std::vector<int>& sorted_indices,
                        std::deque<int>& container);
    double get_distance(const DBSCANPoint& point_1,
                        const DBSCANPoint& point_2);
};

class TanimotoDBSCAN : public AbstractDBSCAN {
   public:
    TanimotoDBSCAN(double eps, int min_pts,
                   NeighbourPrefiltering prefilter_type)
        : AbstractDBSCAN(eps, min_pts, prefilter_type) {}

   protected:
    void get_neighbours(std::vector<DBSCANPoint>& points,
                        int point_index_in_sorted,
                        const std::vector<int>& sorted_indices,
                        std::deque<int>& container);
    double get_distance(const DBSCANPoint& point_1,
                        const DBSCANPoint& point_2);
};

}  // namespace dbscan
