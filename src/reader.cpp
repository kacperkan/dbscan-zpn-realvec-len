#include "reader.hpp"

#include <exception>
#include <fstream>
#include <unordered_map>

namespace dbscan {
void ArffReader::read_to_dataset(Dataset& dataset) {
    std::string line;

    std::ifstream file(this->file_path);

    std::vector<VecXd> data;
    std::vector<int> labels;
    std::unordered_map<std::string, int> possible_labels;
    size_t pos_start;
    size_t pos_end;

    std::string token;
    int current_label_index = -1;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.length() == 0 || line.find("%") == 0 ||
                line.find("@") == 0) {
                continue;
            }

            std::vector<std::string> all_tokens;
            pos_start = 0;
            while ((pos_end = line.find(",", pos_start)) !=
                   std::string::npos) {
                token = line.substr(pos_start, pos_end - pos_start);
                pos_start = pos_end + 1;
                all_tokens.push_back(token);
            }
            all_tokens.push_back(line.substr(pos_start));

            std::vector<double> row;
            for (int i = 0; i < all_tokens.size(); ++i) {
                if (has_labels && i == all_tokens.size() - 1) {
                    if (possible_labels.find(all_tokens[i]) ==
                        possible_labels.end()) {
                        current_label_index++;
                        possible_labels[all_tokens[i]] = current_label_index;
                    }
                    labels.push_back(possible_labels[all_tokens[i]]);
                } else {
                    row.push_back(std::stod(all_tokens[i]));
                }
            }

            VecXd vec(row);
            data.push_back(vec);
        }
        file.close();
    } else {
        throw std::invalid_argument("File couldn't not be opened");
    }
    dataset.set_data(data);
    dataset.set_labels(labels);
    dataset.set_label_mapping(possible_labels);
}
}  // namespace dbscan
