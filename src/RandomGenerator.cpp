#include "RandomGenerator.hpp"
#include <algorithm>

RandomGenerator::RandomGenerator(unsigned int seed) 
    : engine_(seed), dist_(0.0, 1.0) {}

double RandomGenerator::getGaussian() {
    return dist_(engine_);
}

void RandomGenerator::fillGaussian(std::vector<double>& vec) {
    std::generate(vec.begin(), vec.end(), [this]() { return getGaussian(); });
}
