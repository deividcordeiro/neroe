// Trains a single perceptron on AND, OR and XOR.
// AND and OR are linearly separable -> the neuron learns them.
// XOR is not -> a single neuron can never learn it.

#include <iostream>
#include <string>
#include <vector>

#include "neroe/neuron.hpp"
#include "neroe/trainer.hpp"

namespace {

using neroe::Neuron;
using neroe::Sample;
using neroe::TrainResult;

void run(const std::string& name, const std::vector<Sample>& data) {
    Neuron neuron(2);
    const TrainResult result = neroe::train(neuron, data, 0.1, 1000);

    std::cout << "== " << name << " ==\n"
              << (result.converged ? "converged" : "did NOT converge")
              << " after " << result.epochs << " epoch(s)\n"
              << "w = [" << neuron.weights()[0] << ", " << neuron.weights()[1]
              << "], b = " << neuron.bias() << '\n';

    for (const Sample& s : data) {
        std::cout << "  " << s.input[0] << ", " << s.input[1] << " -> "
                  << neuron.forward(s.input) << " (expected " << s.target << ")\n";
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    run("AND", {{{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}});
    run("OR",  {{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 1}});
    run("XOR", {{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 0}});
    return 0;
}
