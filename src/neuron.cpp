#include "neroe/neuron.hpp"

#include <stdexcept>
#include <utility>

#include "neroe/random.hpp"

namespace neroe {

Neuron::Neuron(std::size_t n_inputs, std::uint32_t seed, Activation act)
    : weights_(n_inputs), activation_(act) {
    if (n_inputs == 0) throw std::invalid_argument("Neuron: n_inputs must be > 0");
    if (act == nullptr) throw std::invalid_argument("Neuron: activation is null");

    Xorshift32 rng(seed);
    for (double& w : weights_) w = rng.uniform(-0.5, 0.5);
}

Neuron::Neuron(std::vector<double> weights, double bias, Activation act)
    : weights_(std::move(weights)), bias_(bias), activation_(act) {
    if (weights_.empty()) throw std::invalid_argument("Neuron: weights are empty");
    if (act == nullptr) throw std::invalid_argument("Neuron: activation is null");
}

double Neuron::forward(std::span<const double> input) const {
    if (input.size() != weights_.size())
        throw std::invalid_argument("Neuron::forward: input size mismatch");

    double z = bias_;
    for (std::size_t i = 0; i < input.size(); ++i) z += weights_[i] * input[i];
    return activation_(z);
}

bool Neuron::update(std::span<const double> input, double target,
                    double learning_rate) {
    if (activation_ != step)
        throw std::logic_error(
            "Neuron::update: the perceptron rule requires the step activation");

    const double error = target - forward(input);  // also validates input size
    if (error == 0.0) return false;

    for (std::size_t i = 0; i < input.size(); ++i)
        weights_[i] += learning_rate * error * input[i];
    bias_ += learning_rate * error;
    return true;
}

}  // namespace neroe
