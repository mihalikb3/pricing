#pragma once

#include <vector>
#include <cmath>

/**
 * @brief Collects payoffs and calculates mean and standard error.
 */
class StatisticsCollector {
public:
    void addPayoff(double payoff) {
        sum_ += payoff;
        sum_sq_ += (payoff * payoff);
        count_++;
        if (payoff > 0.0) {
            count_positive_++;
        }
    }

    double getMean() const {
        return (count_ > 0) ? (sum_ / static_cast<double>(count_)) : 0.0;
    }

    double getStandardError() const {
        if (count_ <= 1) return 0.0;
        double variance = (sum_sq_ / count_) - (getMean() * getMean());
        return std::sqrt(std::max(0.0, variance / static_cast<double>(count_)));
    }

    double getProbProfit() const {
        return (count_ > 0) ? (static_cast<double>(count_positive_) / count_) : 0.0;
    }

    long long getCount() const { return count_; }

private:
    double sum_ = 0.0;
    double sum_sq_ = 0.0;
    long long count_ = 0;
    long long count_positive_ = 0;
};
