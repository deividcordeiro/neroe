#include <chrono>
#include <iostream>
#include <vector>

#include "neroe/neuron.hpp"

int main() {
    const neroe::Neuron n(std::vector<double>{1.0, 1.0}, -1.5);
    const ::std::vector<std::vector<double>> X{{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    const long kRounds = 5'000'000;  // 20 milhões de forwards

    double sink = 0.0;  // impede o compilador de apagar o loop
    const auto t0 = std::chrono::steady_clock::now();
    for (long r = 0; r < kRounds; ++r)
        for (const auto& x : X) sink += n.forward(x);
    const auto t1 = std::chrono::steady_clock::now();

    const double sec = std::chrono::duration<double>(t1 - t0).count();
    std::cout << "forwards/s: " << (kRounds * 4) / sec << " (sink=" << sink << ")\n";
}