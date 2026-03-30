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

    // We'll calculate prices for: Base, S+h, S-h, and Vol+h
    // Using 4 PathGenerators
    PathGenerator pg_base(spot_, rate_, vol_, expiry_, steps_);
    PathGenerator pg_up(spot_ + spot_h, rate_, vol_, expiry_, steps_);
    PathGenerator pg_down(spot_ - spot_h, rate_, vol_, expiry_, steps_);
    PathGenerator pg_vol_up(spot_, rate_, vol_ + vol_h, expiry_, steps_);

    StatisticsCollector stats_base, stats_up, stats_down, stats_vol_up;
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
    // Note: We can combine these, but separating clarifies the CRN logic
    for (long long i = 0; i < num_sims; ++i) {
        rg.resetSeed(seed + i);
        stats_vol_up.addPayoff(payoff(pg_vol_up.generatePath(rg)));
    }

    double discount = std::exp(-rate_ * expiry_);
    double v_base = stats_base.getMean() * discount;
    double v_up   = stats_up.getMean() * discount;
    double v_down = stats_down.getMean() * discount;
    double v_vol_up = stats_vol_up.getMean() * discount;

    // Delta = [V(S+h) - V(S-h)] / 2h
    results.delta = (v_up - v_down) / (2.0 * spot_h);

    // Gamma = [V(S+h) - 2V(S) + V(S-h)] / h^2
    results.gamma = (v_up - 2.0 * v_base + v_down) / (spot_h * spot_h);

    // Vega = [V(vol+h) - V(vol)] / h
    results.vega = (v_vol_up - v_base) / vol_h;

    return results;
}
