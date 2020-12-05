#include "dataset.hpp"

namespace dbscan {

void Dataset::standardize(bool unbiased) {
    VecXd mean(cols());
    VecXd std(cols());

    for (auto& row : m_data) {
        mean += row;
    }
    mean /= rows();
    for (auto& row : m_data) {
        std += (row - mean).pow(2.0);
    }
    if (unbiased) {
        std /= rows() - 1;
    } else {
        std /= rows();
    }

    std = std.sqrt();
    for (int i = 0; i < rows(); ++i) {
        m_data[i] = (m_data[i] - mean) / std;
    }
}

}  // namespace dbscan
