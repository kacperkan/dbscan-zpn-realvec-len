#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>
#include <exception>
#include <iostream>
#include <string>

#include "CLI11.hpp"
#include "dataset.hpp"
#include "dbscan.hpp"
#include "metrics.hpp"
#include "reader.hpp"
#include "utils.hpp"
#include "vis_utils.hpp"

void create_algorithm(dbscan::AbstractDBSCAN*& algorithm, std::string name,
                      double eps, int min_pts,
                      dbscan::NeighbourPrefiltering prefilter_type) {
    boost::to_lower(name);
    if (name == "base") {
        algorithm = new dbscan::DBSCAN(eps, min_pts, prefilter_type);
    } else if (name == "tanimoto") {
        algorithm = new dbscan::TanimotoDBSCAN(eps, min_pts, prefilter_type);
    } else {
        std::invalid_argument(std::string("Unknown algorithm type! ") + name);
    }
}

dbscan::NeighbourPrefiltering get_prefilter_type(std::string name) {
    boost::to_lower(name);
    if (name == "none") {
        return dbscan::NeighbourPrefiltering::none;
    } else if (name == "realveclen") {
        return dbscan::NeighbourPrefiltering::realveclen;
    } else if (name == "zpnveclen") {
        return dbscan::NeighbourPrefiltering::zpnveclen;
    } else {
        std::invalid_argument(std::string("Unknown filtering type! ") + name);
    }
    return dbscan::NeighbourPrefiltering::none;
}

int main(int argc, char** argv) {
    CLI::App app{"DBSCAN+ Testing utility"};
    std::string dataset_path = "data/cluto-t7-10k/data.arff";

    double eps = 0.9;
    int min_pts = 10;
    bool has_labels = true;
    bool standardize = false;
    bool visualize = false;
    bool verbose = false;
    std::string algorithm_name = "base";
    std::string experiment_name = "basic";
    std::string prefilter_type_name = "none";

    app.add_option("file", dataset_path, "Path to the dataset");
    app.add_option("experiment_name", experiment_name,
                   "Name of the experiment");
    app.add_option("--eps", eps, "Epsilon parameter");
    app.add_option("--min_pts", min_pts, "Minimum number of points");
    app.add_option("--algorithm_name", algorithm_name,
                   "Name of the algorithm to use. One of: [\"base\", "
                   "\"tanimoto\"]");
    app.add_option("--prefiltering_name", prefilter_type_name,
                   "Name of the prefiltering type to use. One of: [\"none\", "
                   "\"realveclen\", \"zpnveclen\"]");

    app.add_flag("--has_labels", has_labels, "Whether dataset has labels");
    app.add_flag("--standardize", standardize,
                 "Whether the dataset should be standardized");
    app.add_flag("--visualize", visualize,
                 "Whether the results should be visualized (doesn't work when "
                 "running in a container)");
    app.add_flag("--verbose", verbose, "Whether to print all messages");

    CLI11_PARSE(app, argc, argv);

    dbscan::TimeStats::get_instance().tick("data_reading");
    dbscan::ArffReader reader(dataset_path, has_labels);
    dbscan::Dataset dataset;
    reader.read_to_dataset(dataset);
    if (standardize) {
        dataset.standardize(true);
    }
    dbscan::TimeStats::get_instance().tock();

    dbscan::NeighbourPrefiltering prefiltering_type =
        get_prefilter_type(prefilter_type_name);

    dbscan::AbstractDBSCAN* algorithm;
    create_algorithm(algorithm, algorithm_name, eps, min_pts,
                     prefiltering_type);

    std::vector<int> labels;
    std::vector<int> point_types;
    std::vector<std::unordered_set<int>> cluster_assignments;

    dbscan::TimeStats::get_instance().tick("model_fitting");
    algorithm->fit_predict(dataset, labels, point_types, cluster_assignments);
    dbscan::TimeStats::get_instance().tock();

    std::set<int> labels_set(labels.begin(), labels.end());

    dbscan::Purity purity_calculator;
    dbscan::Rand rand_calculator;
    dbscan::DaviesBouldin davies_bouldin_calculator;
    dbscan::Silhouette silhouette_calculator;

    if (verbose) {
        dbscan::TimeStats::get_instance().tick("metric_calculation");
        std::cout << "Purity: "
                  << purity_calculator.get_metric(dataset.labels(), labels,
                                                  cluster_assignments,
                                                  dataset.data())
                  << std::endl;

        std::cout << "RAND: "
                  << rand_calculator.get_metric(dataset.labels(), labels,
                                                cluster_assignments,
                                                dataset.data())
                  << std::endl;

        std::cout << "Davies-Bouldin: "
                  << davies_bouldin_calculator.get_metric(
                         dataset.labels(), labels, cluster_assignments,
                         dataset.data())
                  << std::endl;
        std::cout << "Silhouette: "
                  << silhouette_calculator.get_metric(dataset.labels(), labels,
                                                      cluster_assignments,
                                                      dataset.data())
                  << std::endl;
        dbscan::TimeStats::get_instance().tock();
    }
    dbscan::MetricStats metric_stats;
    metric_stats.purity = purity_calculator.get_metric(
        dataset.labels(), labels, cluster_assignments, dataset.data());
    metric_stats.rand = rand_calculator.get_metric(
        dataset.labels(), labels, cluster_assignments, dataset.data());
    metric_stats.davisbouldin = davies_bouldin_calculator.get_metric(
        dataset.labels(), labels, cluster_assignments, dataset.data());
    metric_stats.silhouette = silhouette_calculator.get_metric(
        dataset.labels(), labels, cluster_assignments, dataset.data());

    dbscan::dump_experiment(experiment_name, dataset_path,
                            algorithm->data_points(), metric_stats, dataset,
                            eps, min_pts);

    if (visualize) {
        dbscan::visualize(dataset.data(), labels, point_types,
                          cluster_assignments);
    }

    delete algorithm;
    return 0;
}
