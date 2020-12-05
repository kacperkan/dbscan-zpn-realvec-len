#include "dbscan.hpp"

#include <algorithm>
#include <numeric>

namespace dbscan {

// General
void AbstractDBSCAN::fit_predict(
    Dataset& dataset, std::vector<int>& labels, std::vector<int>& point_types,
    std::vector<std::unordered_set<int>>& cluster_assignments) {
    this->fit(dataset);
    for (int i = 0; i < m_num_data; ++i) {
        labels.push_back(m_labels[i]);
        point_types.push_back(m_point_types[i]);

        std::unordered_set<int> cluster_assigns = m_cluster_assignments[i];
        cluster_assignments.push_back(cluster_assigns);
    }
}

void AbstractDBSCAN::prefilter_possible_neighbours(
    const std::vector<DBSCANPoint>& points,
    const std::vector<int>& sorted_indices,
    std::vector<int>& prefiltered_indices_of_sorted_indices,
    int point_index_in_sorted) {
    BaseFilterer* filterer;
    switch (m_prefiltering_type) {
        case NeighbourPrefiltering::realveclen:
            filterer = new RealVecLenFilterer();
            break;
        case NeighbourPrefiltering::zpnveclen:
            filterer = new ZPNVecLenFilterer();
            break;
        default:
            filterer = new DummyFilterer();
            break;
    }
    filterer->prefilter_possible_neighbours(
        points, sorted_indices, prefiltered_indices_of_sorted_indices,
        point_index_in_sorted, eps());
    delete filterer;
}

AbstractDBSCAN& AbstractDBSCAN::fit(Dataset& dataset) {
    int current_cluster_id = -1;
    convert_to_dbscan_points(dataset);

    std::vector<int> sorted_indices;
    sort_points(m_data_points, sorted_indices);

    int i;
    for (int index = 0; index < m_data_points.size(); ++index) {
        i = sorted_indices[index];
        if (m_data_points[i].label != UNDEFINED_LABEL) {
            continue;
        }

        std::deque<int> neighbours;
        get_neighbours(m_data_points, index, sorted_indices, neighbours);

        if (neighbours.size() < min_pts()) {
            m_data_points[i].label = NOISE_LABEL;
            m_data_points[i].cluster_assignments.emplace(NOISE_LABEL);
            m_data_points[i].is_noise_point = true;
            continue;
        }
        current_cluster_id++;
        m_data_points[i].label = current_cluster_id;
        m_data_points[i].cluster_assignments.emplace(current_cluster_id);
        m_data_points[i].is_core_point = true;

        while (!neighbours.empty()) {
            int seed = neighbours[0];
            int proper_seed = sorted_indices[seed];
            neighbours.pop_front();
            if (m_data_points[proper_seed].label == NOISE_LABEL) {
                m_data_points[proper_seed].label = current_cluster_id;
                m_data_points[proper_seed].cluster_assignments.erase(
                    NOISE_LABEL);
                m_data_points[proper_seed].cluster_assignments.emplace(
                    current_cluster_id);
            }

            if (m_data_points[proper_seed].label != UNDEFINED_LABEL) {
                m_data_points[proper_seed].cluster_assignments.emplace(
                    current_cluster_id);
                continue;
            }
            m_data_points[proper_seed].label = current_cluster_id;
            m_data_points[proper_seed].cluster_assignments.emplace(
                current_cluster_id);

            std::deque<int> new_neighbours;
            get_neighbours(m_data_points, seed, sorted_indices,
                           new_neighbours);
            if (new_neighbours.size() >= min_pts()) {
                m_data_points[proper_seed].is_core_point = true;
                m_data_points[proper_seed].is_border_point = false;
                neighbours.insert(neighbours.end(), new_neighbours.begin(),
                                  new_neighbours.end());
            } else {
                m_data_points[proper_seed].is_border_point = true;
                m_data_points[proper_seed].is_core_point = false;
            }
        }
    }

    convert_points_to_labels(m_data_points);
    m_num_data = m_data_points.size();
    m_num_labels = current_cluster_id + 1;
    was_fitted = true;
    return *this;
}

std::vector<int> AbstractDBSCAN::labels() {
    std::vector<int> labels_vector(m_num_data);
    memcpy(labels_vector.data(), m_labels, m_num_data * sizeof(int));
    return labels_vector;
}

std::vector<int> AbstractDBSCAN::point_types() {
    std::vector<int> point_types_vector(m_num_data);
    memcpy(point_types_vector.data(), m_point_types, m_num_data * sizeof(int));
    return point_types_vector;
}

std::vector<std::unordered_set<int>> AbstractDBSCAN::cluster_assignments() {
    return m_cluster_assignments;
}

void AbstractDBSCAN::convert_to_dbscan_points(Dataset& dataset) {
    for (int i = 0; i < dataset.data().size(); ++i) {
        DBSCANPoint point;
        point.point = std::make_shared<VecXd>(dataset.data()[i]);
        point.length = dataset.data()[i].length();
        m_data_points.push_back(point);
    }
}

void AbstractDBSCAN::sort_points(const std::vector<DBSCANPoint>& points,
                                 std::vector<int>& indices) {
    indices.resize(points.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::stable_sort(indices.begin(), indices.end(),
                     [&points](size_t index_1, size_t index_2) {
                         return points[index_1].length <
                                points[index_2].length;
                     });
}

void AbstractDBSCAN::convert_points_to_labels(
    std::vector<DBSCANPoint>& points) {
    m_labels = new int[points.size()];
    m_point_types = new int[points.size()];
    for (int i = 0; i < points.size(); ++i) {
        m_labels[i] = points[i].label;
        if (points[i].is_core_point) {
            m_point_types[i] = CORE_POINT;
        } else if (points[i].is_border_point) {
            m_point_types[i] = BORDER_POINT;
        } else {
            m_point_types[i] = OUTLIER_POINT;
        }

        std::unordered_set<int> cluster_assins = points[i].cluster_assignments;
        m_cluster_assignments.push_back(cluster_assins);
    }
}

// DBSCAN
void DBSCAN::get_neighbours(std::vector<DBSCANPoint>& points,
                            int point_index_in_sorted,
                            const std::vector<int>& sorted_indices,
                            std::deque<int>& container) {
    int point_index = sorted_indices[point_index_in_sorted];
    int proper_i;
    for (int i = 0; i < sorted_indices.size(); i++) {
        proper_i = sorted_indices[i];
        points[point_index].num_calculations++;
        if (get_distance(points[point_index], points[proper_i]) <= eps()) {
            container.push_back(i);
        }
    }
}

double DBSCAN::get_distance(const DBSCANPoint& point_1,
                            const DBSCANPoint& point_2) {
    return std::sqrt((*point_1.point - *point_2.point).pow(2.0).sum());
}

// Tanimoto
double TanimotoDBSCAN::get_distance(const DBSCANPoint& point_1,
                                    const DBSCANPoint& point_2) {
    double u_dot_v = (*point_1.point).dot(*point_2.point);
    double u_len_sq = (*point_1.point).pow(2.0).sum();
    double v_len_sq = (*point_2.point).pow(2.0).sum();
    double result = u_dot_v / (u_len_sq + v_len_sq - u_dot_v);
    return result;
}
void TanimotoDBSCAN::get_neighbours(std::vector<DBSCANPoint>& points,
                                    int point_index_in_sorted,
                                    const std::vector<int>& sorted_indices,
                                    std::deque<int>& container) {
    int point_index = sorted_indices[point_index_in_sorted];
    int proper_i;
    std::vector<int> prefiltered_indices_of_sorted_indices;
    prefilter_possible_neighbours(points, sorted_indices,
                                  prefiltered_indices_of_sorted_indices,
                                  point_index_in_sorted);

    for (int i = 0; i < prefiltered_indices_of_sorted_indices.size(); i++) {
        proper_i = sorted_indices[prefiltered_indices_of_sorted_indices[i]];
        points[point_index].num_calculations++;
        if (get_distance(points[point_index], points[proper_i]) >= eps()) {
            container.push_back(prefiltered_indices_of_sorted_indices[i]);
        }
    }
}

// Filterer
bool BaseFilterer::is_in_bounds(const DBSCANPoint& point, double lower_bound,
                                double upper_bound) {
    return lower_bound <= point.length && point.length <= upper_bound;
}

void DummyFilterer::prefilter_possible_neighbours(
    const std::vector<DBSCANPoint>& points,
    const std::vector<int>& sorted_indices,
    std::vector<int>& new_sorted_indices, int point_index_in_sorted,
    double eps) {
    new_sorted_indices = sorted_indices;
}

// RealVecLen
void RealVecLenFilterer::prefilter_possible_neighbours(
    const std::vector<DBSCANPoint>& points,
    const std::vector<int>& sorted_indices,
    std::vector<int>& prefiltered_indices_of_sorted_indices,
    int point_index_in_sorted, double eps) {
    int point_index = sorted_indices[point_index_in_sorted];
    double coeff = 1.0 + 1.0 / eps;
    double alpha = 0.5 * (coeff + std::sqrt(std::pow(coeff, 2.0) - 4.0));
    double lower_bound = points[point_index].length / alpha;
    double upper_bound = points[point_index].length * alpha;

    int proper_index;
    int up_index = point_index_in_sorted + 1,
        down_index = point_index_in_sorted - 1;

    bool stop_going_up = false;
    bool stop_going_down = false;
    while (true) {
        if (up_index < sorted_indices.size() && !stop_going_up) {
            proper_index = sorted_indices[up_index];
            if (is_in_bounds(points[proper_index], lower_bound, upper_bound)) {
                prefiltered_indices_of_sorted_indices.push_back(up_index);
            } else {
                stop_going_up = true;
            }
            up_index++;
        }

        if (down_index >= 0 && !stop_going_down) {
            proper_index = sorted_indices[down_index];
            if (is_in_bounds(points[proper_index], lower_bound, upper_bound)) {
                prefiltered_indices_of_sorted_indices.push_back(down_index);
            } else {
                stop_going_down = false;
            }
            down_index--;
        }

        stop_going_up = up_index >= sorted_indices.size() || stop_going_up;
        stop_going_down = down_index < 0 || stop_going_down;

        if (stop_going_up && stop_going_down) {
            break;
        }
    }
}

// ZPN
void ZPNVecLenFilterer::prefilter_possible_neighbours(
    const std::vector<DBSCANPoint>& points,
    const std::vector<int>& sorted_indices,
    std::vector<int>& prefiltered_indices_of_sorted_indices,
    int point_index_in_sorted, double eps) {
    int point_index = sorted_indices[point_index_in_sorted];
    eps = std::sqrt(eps);
    double lower_bound = points[point_index].length * eps;
    double upper_bound = points[point_index].length / eps;

    int proper_index;
    int up_index = point_index_in_sorted + 1,
        down_index = point_index_in_sorted - 1;
    bool stop_going_up = false;
    bool stop_going_down = false;

    while (true) {
        if (up_index < sorted_indices.size() && !stop_going_up) {
            proper_index = sorted_indices[up_index];
            if (is_in_bounds(points[proper_index], lower_bound, upper_bound)) {
                prefiltered_indices_of_sorted_indices.push_back(up_index);
            } else {
                stop_going_up = true;
            }
            up_index++;
        }

        if (down_index >= 0 && !stop_going_down) {
            proper_index = sorted_indices[down_index];
            if (is_in_bounds(points[proper_index], lower_bound, upper_bound)) {
                prefiltered_indices_of_sorted_indices.push_back(down_index);
            } else {
                stop_going_down = false;
            }
            down_index--;
        }
        stop_going_up = up_index >= sorted_indices.size() || stop_going_up;
        stop_going_down = down_index < 0 || stop_going_down;

        if (stop_going_up && stop_going_down) {
            break;
        }
    }
}

}  // namespace dbscan
