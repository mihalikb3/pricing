#pragma once

#include <vector>
#include "RandomGenerator.hpp"

/**
 * @brief Generates asset price paths using Geometric Brownian Motion.
 */
class PathGenerator {
public:
    PathGenerator(double spot, double rate, double vol, double expiry, int steps);

    /**
     * @brief Generates a single path of prices.
     * @param rg Reference to a RandomGenerator to ensure we don't re-seed every time.
     * @return A vector of prices from t=0 to t=Expiry.
     */
    std::vector<double> generatePath(RandomGenerator& rg) const;

private:
    double spot_;
    double rate_;
    double vol_;
    double expiry_;
    int steps_;
    double dt_;
};
