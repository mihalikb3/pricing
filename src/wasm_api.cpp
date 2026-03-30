#include <emscripten/bind.h>
#include <vector>
#include "PathGenerator.hpp"
#include "RandomGenerator.hpp"

using namespace emscripten;

/**
 * @brief This function will be called directly by JavaScript.
 * It returns a flat array of prices to the browser for plotting.
 */
std::vector<double> get_sample_path(double spot, double rate, double vol, double expiry, int steps, int seed) {
    RandomGenerator rg(seed);
    PathGenerator pg(spot, rate, vol, expiry, steps);
    
    return pg.generatePath(rg);
}

// Emscripten Binding Block
EMSCRIPTEN_BINDINGS(monte_carlo_engine) {
    register_vector<double>("VectorDouble");
    
    function("get_sample_path", &get_sample_path);
}
