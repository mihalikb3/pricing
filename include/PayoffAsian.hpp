#pragma once

#include "Payoff.hpp"
#include <numeric>
#include <vector>

/**
 * @brief Asian Call Option Payoff: max(Average(S) - K, 0)
 * This is 'Path-Dependent' because it looks at every price in the path.
 */
class PayoffAsian : public Payoff {
public:
    explicit PayoffAsian(double strike) : strike_(strike) {}

    virtual double operator()(const std::vector<double>& path) const override {
        if (path.empty()) return 0.0;

        // Use std::accumulate to sum up the entire price path
        double sum = std::accumulate(path.begin(), path.end(), 0.0);
        double average = sum / static_cast<double>(path.size());

        return (average > strike_) ? (average - strike_) : 0.0;
    }

private:
    double strike_;
};
