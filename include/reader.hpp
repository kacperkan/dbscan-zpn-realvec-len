#pragma once

#include "dataset.hpp"
#include <string>
#include <vector>

namespace dbscan {
class ArffReader {
  public:
    ArffReader(std::string file_path, bool has_labels)
        : file_path(file_path), has_labels(has_labels){};
    ~ArffReader(){};

    void read_to_dataset(Dataset& dataset);

  private:
    std::string file_path;
    bool has_labels;
};
} // namespace dbscan
