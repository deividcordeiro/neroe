#include <iostream>
#include <vector>

#include "neroe/serialization.hpp"

int main() {
    neroe::Neuron n(std::vector<double>{1.0, 1.0}, -1.5);
    neroe::save(n, std::cout);
}