#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "math.hpp"

namespace dbscan {
class Dataset {
   public:
    Dataset() : m_rows(0), m_cols(0) {}
    ~Dataset() {}

    inline int rows() { return m_rows; }
    inline int cols() { return m_cols; }
    std::vector<VecXd>& data() { return m_data; }
    const std::vector<int>& labels() { return m_labels; }

    void set_data(std::vector<VecXd>& data) {
        this->m_data = data;
        this->m_rows = data.size();
        if (data.size() > 0) {
            this->m_cols = data[0].size();
        }
    }

    void set_labels(std::vector<int>& labels) { this->m_labels = labels; }
    void set_label_mapping(
        std::unordered_map<std::string, int>& label_mapping) {
        this->m_label_mapping = label_mapping;
    }
    void standardize(bool unbiased);
    double operator()(int y, int x) { return m_data[y][x]; }

   private:
    int m_rows;
    int m_cols;

    std::vector<VecXd> m_data;
    std::vector<int> m_labels;
    std::unordered_map<std::string, int> m_label_mapping;
};
}  // namespace dbscan
