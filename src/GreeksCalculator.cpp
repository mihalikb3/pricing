#include "GreeksCalculator.hpp"
#include "StatisticsCollector.hpp"
#include <cmath>

GreeksCalculator::GreeksCalculator(double spot, double rate, double vol, double expiry, int steps)
    : spot_(spot), rate_(rate), vol_(vol), expiry_(expiry), steps_(steps) {}

Greeks GreeksCalculator::calculate(const Payoff& payoff, long long num_sims, unsigned int seed) {
    Greeks results;
    
    // Bump sizes (Perturbations)
    double spot_h = 0.01 * spot_; // 1% of spot
    double vol_h = 0.01;         // 1% vol bump
    double rate_h = 0.0001;      // 1 basis point bump (0.01%)
    double time_h = 1.0/365.0;   // 1 day bump

    // We'll calculate prices for: Base, S+h, S-h, Vol+h, Rate+h, and Time-h
    PathGenerator pg_base(spot_, rate_, vol_, expiry_, steps_);
    PathGenerator pg_up(spot_ + spot_h, rate_, vol_, expiry_, steps_);
    PathGenerator pg_down(spot_ - spot_h, rate_, vol_, expiry_, steps_);
    PathGenerator pg_vol_up(spot_, rate_, vol_ + vol_h, expiry_, steps_);
    PathGenerator pg_rho_up(spot_, rate_ + rate_h, vol_, expiry_, steps_);
    
    // For Theta, we decrease time-to-maturity
    double new_expiry = (expiry_ > time_h) ? (expiry_ - time_h) : expiry_;
    PathGenerator pg_theta_down(spot_, rate_, vol_, new_expiry, steps_);

    StatisticsCollector stats_base, stats_up, stats_down, stats_vol_up, stats_rho_up, stats_theta_down;
    RandomGenerator rg(seed);

    // --- REPLAY 1: Base, Up, Down (CRN for Delta/Gamma) ---
    for (long long i = 0; i < num_sims; ++i) {
        unsigned int current_seed = seed + i;
        
        rg.resetSeed(current_seed);
        stats_base.addPayoff(payoff(pg_base.generatePath(rg)));

        rg.resetSeed(current_seed);
        stats_up.addPayoff(payoff(pg_up.generatePath(rg)));

        rg.resetSeed(current_seed);
        stats_down.addPayoff(payoff(pg_down.generatePath(rg)));
    }

    // --- REPLAY 2: Vol Up (CRN for Vega) ---
    for (long long i = 0; i < num_sims; ++i) {
        rg.resetSeed(seed + i);
        stats_vol_up.addPayoff(payoff(pg_vol_up.generatePath(rg)));
    }

    // --- REPLAY 3: Rate Up & Time Down (CRN for Rho/Theta) ---
    for (long long i = 0; i < num_sims; ++i) {
        unsigned int current_seed = seed + i;
        
        rg.resetSeed(current_seed);
        stats_rho_up.addPayoff(payoff(pg_rho_up.generatePath(rg)));

        rg.resetSeed(current_seed);
        stats_theta_down.addPayoff(payoff(pg_theta_down.generatePath(rg)));
    }

    double discount = std::exp(-rate_ * expiry_);
    double v_base = stats_base.getMean() * discount;
    double v_up   = stats_up.getMean() * discount;
    double v_down = stats_down.getMean() * discount;
    double v_vol_up = stats_vol_up.getMean() * discount;
    
    double v_rho_up = stats_rho_up.getMean() * std::exp(-(rate_ + rate_h) * expiry_);
    double v_theta_down = stats_theta_down.getMean() * std::exp(-rate_ * new_expiry);

    // Delta = [V(S+h) - V(S-h)] / 2h
    results.delta = (v_up - v_down) / (2.0 * spot_h);

    // Gamma = [V(S+h) - 2V(S) + V(S-h)] / h^2
    results.gamma = (v_up - 2.0 * v_base + v_down) / (spot_h * spot_h);

    // Vega = [V(vol+h) - V(vol)] / h
    results.vega = (v_vol_up - v_base) / vol_h;

    // Rho = [V(r+h) - V(r)] / h
    results.rho = (v_rho_up - v_base) / rate_h;

    // Theta = [V(t-h) - V(t)] / h (Approximated as decay per unit time)
    results.theta = (v_theta_down - v_base) / time_h;

    return results;
}
