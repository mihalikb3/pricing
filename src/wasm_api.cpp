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

// Emscripten Binding Block
EMSCRIPTEN_BINDINGS(monte_carlo_engine) {
    // Register vectors for data transfer
    register_vector<double>("VectorDouble");
    
    // Register the Greeks struct as a Value Object (translates to a JS object)
    value_object<Greeks>("Greeks")
        .field("delta", &Greeks::delta)
        .field("gamma", &Greeks::gamma)
        .field("vega", &Greeks::vega)
        .field("theta", &Greeks::theta)
        .field("rho", &Greeks::rho);

    // Export functions to JS
    function("get_sample_path", &get_sample_path);
    function("get_greeks", &get_greeks);
}
