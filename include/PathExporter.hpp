#pragma once

#include <vector>
#include <string>
#include <fstream>

/**
 * @brief Utility to export paths to CSV for Python visualization.
 */
class PathExporter {
public:
    static void exportToCSV(const std::vector<std::vector<double>>& all_paths, const std::string& filename) {
        std::ofstream file(filename);
        
        // Write header: Path0, Path1, Path2...
        for (size_t i = 0; i < all_paths.size(); ++i) {
            file << "Path" << i << (i == all_paths.size() - 1 ? "" : ",");
        }
        file << "\n";

        // Paths are stored as vectors of steps. We need to write row by row (step by step)
        size_t num_steps = all_paths[0].size();
        for (size_t step = 0; step < num_steps; ++step) {
            for (size_t p = 0; p < all_paths.size(); ++p) {
                file << all_paths[p][step] << (p == all_paths.size() - 1 ? "" : ",");
            }
            file << "\n";
        }
    }
};
