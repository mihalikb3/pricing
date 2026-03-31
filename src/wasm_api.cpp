#include <emscripten/bind.h>
#include <vector>
#include <string>
#include <memory>
#include "PathGenerator.hpp"
#include "RandomGenerator.hpp"
#include "GreeksCalculator.hpp"
#include "PayoffCall.hpp"
#include "PayoffAsian.hpp"

using namespace emscripten;

/**
 * @brief Returns a single sample price path for visualization.
 */
std::vector<double> get_sample_path(double spot, double rate, double vol, double expiry, int steps, int seed) {
    RandomGenerator rg(seed);
    PathGenerator pg(spot, rate, vol, expiry, steps);
    
    return pg.generatePath(rg);
}

/**
 * @brief Computes all Greeks for a given option type.
 * @param type "european" or "asian"
 */
Greeks get_greeks(std::string type, double spot, double strike, double rate, double vol, double expiry, int steps, int num_sims, int seed) {
    std::unique_ptr<Payoff> payoff;

    if (type == "asian") {
        payoff = std::make_unique<PayoffAsian>(strike);
    } else {
        // Default to European Call
        payoff = std::make_unique<PayoffCall>(strike);
    }

    GreeksCalculator gc(spot, rate, vol, expiry, steps);
    return gc.calculate(*payoff, static_cast<long long>(num_sims), static_cast<unsigned int>(seed));
}

/**
 * @brief Generates a 2D grid of data (Heatmap data).
 * @param metric The metric to return: "price", "delta", "gamma", "vega", "theta", "rho", "std_error", "prob_profit"
 */
std::vector<double> get_price_heatmap(std::string type, std::string metric, double strike, double rate, double expiry,
                                      double spot_min, double spot_max, int spot_steps,
                                      double vol_min, double vol_max, int vol_steps,
                                      int num_sims, int seed) {
    std::vector<double> heatmap;
    heatmap.reserve(spot_steps * vol_steps);

    std::unique_ptr<Payoff> payoff;
    if (type == "asian") {
        payoff = std::make_unique<PayoffAsian>(strike);
    } else {
        payoff = std::make_unique<PayoffCall>(strike);
    }

    double d_spot = (spot_steps > 1) ? (spot_max - spot_min) / (spot_steps - 1) : 0.0;
    double d_vol = (vol_steps > 1) ? (vol_max - vol_min) / (vol_steps - 1) : 0.0;

    bool needs_greeks = (metric != "price" && metric != "std_error" && metric != "prob_profit");

    for (int i = 0; i < vol_steps; ++i) {
        double current_vol = vol_min + i * d_vol;
        for (int j = 0; j < spot_steps; ++j) {
            double current_spot = spot_min + j * d_spot;
            
            GreeksCalculator gc(current_spot, rate, current_vol, expiry, 50);
            Greeks res;
            
            if (needs_greeks) {
                res = gc.calculate(*payoff, static_cast<long long>(num_sims), static_cast<unsigned int>(seed));
            } else {
                res = gc.calculateBasic(*payoff, static_cast<long long>(num_sims), static_cast<unsigned int>(seed));
            }
            
            if (metric == "delta") heatmap.push_back(res.delta);
            else if (metric == "gamma") heatmap.push_back(res.gamma);
            else if (metric == "vega") heatmap.push_back(res.vega);
            else if (metric == "theta") heatmap.push_back(res.theta);
            else if (metric == "rho") heatmap.push_back(res.rho);
            else if (metric == "std_error") heatmap.push_back(res.std_error);
            else if (metric == "prob_profit") heatmap.push_back(res.prob_profit);
            else heatmap.push_back(res.price); // Default to price
        }
    }

    return heatmap;
}

// Emscripten Binding Block
EMSCRIPTEN_BINDINGS(monte_carlo_engine) {
    // Register vectors for data transfer
    register_vector<double>("VectorDouble");
    
    // Register the Greeks struct as a Value Object (translates to a JS object)
    value_object<Greeks>("Greeks")
        .field("price", &Greeks::price)
        .field("delta", &Greeks::delta)
        .field("gamma", &Greeks::gamma)
        .field("vega", &Greeks::vega)
        .field("theta", &Greeks::theta)
        .field("rho", &Greeks::rho)
        .field("std_error", &Greeks::std_error)
        .field("prob_profit", &Greeks::prob_profit);

    // Export functions to JS
    function("get_sample_path", &get_sample_path);
    function("get_greeks", &get_greeks);
    function("get_price_heatmap", &get_price_heatmap);
}
