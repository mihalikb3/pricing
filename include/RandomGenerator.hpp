#pragma once

#include <random>
#include <vector>

/**
 * @brief Encapsulates a Mersenne Twister engine to generate Gaussian samples.
 * Follows RAII principles for engine state management.
 */
class RandomGenerator {
public:
    // Explicit constructor to prevent accidental conversions
    explicit RandomGenerator(unsigned int seed = std::random_device{}());

    // Generate a single standard normal sample N(0,1)
    double getGaussian();

    // Fill a vector with Gaussian samples
    void fillGaussian(std::vector<double>& vec);

private:
    std::mt19937 engine_;
    std::normal_distribution<double> dist_;
};
