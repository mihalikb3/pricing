#pragma once

#include <vector>

/**
 * @brief Abstract base class for option payoffs.
 * This allows the engine to be 'product-agnostic'.
 */
class Payoff {
public:
    virtual ~Payoff() = default;

    /**
     * @brief Calculates the payoff based on a price path.
     * @param path The simulated price trajectory.
     * @return The payoff value (e.g., max(S_T - K, 0)).
     */
    virtual double operator()(const std::vector<double>& path) const = 0;
};
