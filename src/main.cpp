#include <iostream>
#include <vector>
#include <iomanip>
#include <memory>
#include <cmath>
#include "RandomGenerator.hpp"
#include "PathGenerator.hpp"
#include "PayoffCall.hpp"
#include "PayoffAsian.hpp"
#include "StatisticsCollector.hpp"

int main() {
    // Parameters
    double spot = 100.0;
    double strike = 100.0;
    double rate = 0.05;
    double vol = 0.20;
    double expiry = 1.0;
    int steps = 252;
    long long num_sims = 100000;

    RandomGenerator rg(42); 
    PathGenerator pg(spot, rate, vol, expiry, steps);
    
    // We'll price BOTH a European and an Asian call to compare them
    std::unique_ptr<Payoff> european_call = std::make_unique<PayoffCall>(strike);
    std::unique_ptr<Payoff> asian_call = std::make_unique<PayoffAsian>(strike);

    StatisticsCollector stats_euro;
    StatisticsCollector stats_asian;

    std::cout << "Starting Monte Carlo Engine (100,000 paths)..." << std::endl;

    for (long long i = 0; i < num_sims; ++i) {
        std::vector<double> path = pg.generatePath(rg);
        
        stats_euro.addPayoff((*european_call)(path));
        stats_asian.addPayoff((*asian_call)(path));
    }

    double discount = std::exp(-rate * expiry);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n--- MONTE CARLO PRICING COMPARISON ---" << std::endl;
    std::cout << "European Call Price: $" << stats_euro.getMean() * discount << std::endl;
    std::cout << "Asian Call Price:    $" << stats_asian.getMean() * discount << " (Should be lower!)" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    return 0;
}
