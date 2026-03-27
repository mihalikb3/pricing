#include "PathGenerator.hpp"
#include <cmath>

PathGenerator::PathGenerator(double spot, double rate, double vol, double expiry, int steps)
    : spot_(spot), rate_(rate), vol_(vol), expiry_(expiry), steps_(steps) {
    dt_ = expiry_ / static_cast<double>(steps_);
}

std::vector<double> PathGenerator::generatePath(RandomGenerator& rg) const {
    std::vector<double> path(steps_ + 1);
    path[0] = spot_;

    // Pre-calculate the deterministic part of the GBM step (Optimization)
    // Drift = (r - 0.5 * sigma^2) * dt
    double drift = (rate_ - 0.5 * vol_ * vol_) * dt_;
    // Diffusion = sigma * sqrt(dt)
    double diffusion = vol_ * std::sqrt(dt_);

    for (int i = 1; i <= steps_; ++i) {
        double gauss = rg.getGaussian();
        // S_next = S_prev * exp(drift + diffusion * Z)
        path[i] = path[i-1] * std::exp(drift + diffusion * gauss);
    }

    return path;
}
