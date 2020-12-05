#pragma once
#include <math.h>

#include <chrono>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "dbscan.hpp"

namespace dbscan {

const double EPS = 1e-8;

class TimeStats {
   public:
    static TimeStats& get_instance() {
        static TimeStats instance;
        return instance;
    }
    TimeStats(TimeStats const&) = delete;
    void operator=(TimeStats const&) = delete;
    inline void push_time(const std::string name, int time) {
        m_data.push_back(std::make_pair(name, time));
    }
    const std::pair<std::string, int>& operator()(int elem) {
        return m_data[elem];
    }
    inline int size() { return m_data.size(); }
    inline void tick(const std::string name) {
        m_last_name = name;
        m_beg = m_clock::now();
    }
    inline void tock() {
        double duration =
            std::chrono::duration<double, std::milli>(m_clock::now() - m_beg)
                .count();
        m_data.push_back(std::make_pair(m_last_name, duration));
    }

    int total_runtime() {
        int total = 0;
        for (const auto& elem : m_data) {
            total += elem.second;
        }
        return total;
    }

   private:
    TimeStats() {}

    std::vector<std::pair<std::string, int>> m_data;
    std::string m_last_name;

    typedef std::chrono::high_resolution_clock m_clock;

    std::chrono::time_point<m_clock> m_beg;
};

struct MetricStats {
    double rand = 0.0;
    double purity = 0.0;
    double davisbouldin = 0.0;
    double silhouette = 0.0;
};

void set_intersection(const std::unordered_set<int>& set1,
                      const std::unordered_set<int>& set2,
                      std::unordered_set<int>& intersection);
double combinations(int n, int k);
void dump_experiment(const std::string name, const std::string file_path,
                     const std::vector<DBSCANPoint>& point_stats,
                     const MetricStats& metric_stats, Dataset& dataset,
                     double epsilon, int min_pts);
}  // namespace dbscan
