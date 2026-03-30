#pragma once

#include "Greeks.hpp"
#include "PathGenerator.hpp"
#include "Payoff.hpp"
#include "RandomGenerator.hpp"
#include <memory>

/**
 * @brief Calculates option sensitivities (Greeks) using Finite Difference (Bumping).
 * Uses Common Random Numbers (CRN) to reduce variance in the difference.
 */
class GreeksCalculator {
public:
    GreeksCalculator(double spot, double rate, double vol, double expiry, int steps);

    /**
     * @brief Computes Delta, Gamma, and Vega.
     * @param payoff The payoff structure to evaluate.
     * @param num_sims Number of paths per case.
     * @param seed The base seed for CRN.
     * @return A Greeks struct containing the sensitivities.
     */
    Greeks calculate(const Payoff& payoff, long long num_sims, unsigned int seed);

private:
    double spot_;
    double rate_;
    double vol_;
    double expiry_;
    int steps_;
};
