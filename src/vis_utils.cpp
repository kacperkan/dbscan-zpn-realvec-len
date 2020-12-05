#include "vis_utils.hpp"

#include "dbscan.hpp"
#include "gnuplot-iostream.h"

namespace dbscan {
void visualize(
    const std::vector<VecXd>& data, const std::vector<int>& labels,
    const std::vector<int>& point_types,
    const std::vector<std::unordered_set<int>>& cluster_assignments) {
    Gnuplot gp;

    std::vector<boost::tuple<double, double, int>> pts;

    for (int i = 0; i < data.size(); ++i) {
        pts.push_back(boost::make_tuple(
            data[i][0], data[i][1],
            cluster_assignments[i].size() >= 2 ? 100 : labels[i]));
    }
    gp << "plot '-' u 1:2:3 w points pt 7 ps 1 lc variable title 'pts'\n";
    gp.send1d(pts);
}
}  // namespace dbscan
