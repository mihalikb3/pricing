#pragma once

/**
 * @brief Container for Option Results and Sensitivities.
 */
struct Greeks {
    double price = 0.0;
    double delta = 0.0;
    double gamma = 0.0;
    double vega = 0.0;
    double theta = 0.0;
    double rho = 0.0;
    double std_error = 0.0;
    double prob_profit = 0.0;
};
