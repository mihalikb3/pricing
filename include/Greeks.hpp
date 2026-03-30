#pragma once

/**
 * @brief Container for Option Sensitivities (The Greeks).
 */
struct Greeks {
    double delta = 0.0;
    double gamma = 0.0;
    double vega = 0.0;
    double theta = 0.0; // Time decay (Bonus!)
    double rho = 0.0;   // Interest rate sensitivity
};
