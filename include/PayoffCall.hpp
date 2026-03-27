#pragma once

#include "Payoff.hpp"

/**
 * @brief Standard European Call Option Payoff: max(S_T - K, 0)
 */
class PayoffCall : public Payoff {
public:
    explicit PayoffCall(double strike) : strike_(strike) {}

    virtual double operator()(const std::vector<double>& path) const override {
        // European options only care about the final price in the path
        double final_price = path.back();
        return (final_price > strike_) ? (final_price - strike_) : 0.0;
    }

private:
    double strike_;
};
