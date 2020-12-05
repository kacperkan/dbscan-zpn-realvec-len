#pragma once
#include <unordered_set>
#include <vector>

#include "math.hpp"

namespace dbscan {
void visualize(
    const std::vector<VecXd>& data, const std::vector<int>& labels,
    const std::vector<int>& point_types,
    const std::vector<std::unordered_set<int>>& cluster_assignments);
}  // namespace dbscan
