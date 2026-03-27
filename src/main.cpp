#include <iostream>
#include <vector>
#include <iomanip>
#include <memory>
#include "RandomGenerator.hpp"
#include "PathGenerator.hpp"
#include "PayoffCall.hpp"

int main() {
    RandomGenerator rg(42); 

    // Simulation Parameters
    double spot = 100.0;
    double rate = 0.05;
    double vol = 0.20;
    double expiry = 1.0;
    int steps = 252; // 252 trading days in a year

    PathGenerator pg(spot, rate, vol, expiry, steps);
    
    // Create our Payoff (European Call with Strike 100)
    // Using std::unique_ptr for memory safety (RAII)
    std::unique_ptr<Payoff> call_option = std::make_unique<PayoffCall>(100.0);

    std::cout << "Simulating One Year (252 steps) for a Call Option (Strike 100):" << std::endl;
    std::vector<double> path = pg.generatePath(rg);

    double final_price = path.back();
    double payoff_val = (*call_option)(path);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "------------------------------------" << std::endl;
    std::cout << "Starting Price: $" << spot << std::endl;
    std::cout << "Final Price:    $" << final_price << std::endl;
    std::cout << "Payoff Value:   $" << payoff_val << std::endl;
    std::cout << "------------------------------------" << std::endl;

    return 0;
}
