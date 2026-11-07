#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "neroe/activation.hpp"

namespace neroe {

/// A single perceptron: y = activation(w . x + b).
class Neuron {
public:
    /// Creates a neuron with small random weights in [-0.5, 0.5) and zero bias.
    /// The same seed always produces the same neuron.
    /// @throws std::invalid_argument if n_inputs is 0 or act is null.
    explicit Neuron(std::size_t n_inputs, std::uint32_t seed = 42,
                    Activation act = step);

    /// Creates a neuron from explicit parameters.
    /// @throws std::invalid_argument if weights is empty or act is null.
    Neuron(std::vector<double> weights, double bias, Activation act = step);

    /// Computes the neuron output for one input vector.
    /// @throws std::invalid_argument if input.size() != n_inputs().
    [[nodiscard]] double forward(std::span<const double> input) const;

    /// One step of the perceptron learning rule:
    ///   w <- w + lr * (target - y) * x,   b <- b + lr * (target - y)
    /// This rule is only valid for the binary step activation. Other activations
    /// are fine for forward() (e.g. evolutionary search) but cannot be trained here.
    /// @return true if the neuron was wrong (parameters changed), false otherwise.
    /// @throws std::logic_error if the neuron does not use the step activation.
    bool update(std::span<const double> input, double target, double learning_rate);

    [[nodiscard]] std::size_t n_inputs() const noexcept { return weights_.size(); }
    [[nodiscard]] std::span<const double> weights() const noexcept { return weights_; }
    [[nodiscard]] double bias() const noexcept { return bias_; }
    [[nodiscard]] Activation activation() const noexcept { return activation_; }

private:
    std::vector<double> weights_;
    double bias_{0.0};
    Activation activation_;
};

}  // namespace neroe
