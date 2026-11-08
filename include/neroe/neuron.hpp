#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "neroe/activation.hpp"

namespace neroe {

/// Um único perceptron: y = ativação(w . x + b).
class Neuron {
public:

    explicit Neuron(std::size_t n_inputs, std::uint32_t seed = 42,
                    Activation act = step);

/// Cria um neurônio a partir de parâmetros explícitos.
/// @throws std::invalid_argument se weights estiver vazio ou act for nulo.
    Neuron(std::vector<double> weights, double bias, Activation act = step);

    /// Calcula a saída do neurônio para um vetor de entrada.
    /// @throws std::invalid_argument se input.size() != n_inputs().
    [[nodiscard]] double forward(std::span<const double> input) const;

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
