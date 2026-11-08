#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "perceptron/activation.hpp"

namespace perceptron {

/// Um único perceptron: y = ativação(w . x + b).
class Neuron {
   public:
    /// Cria um neurônio com pequenos pesos aleatórios em [-0,5, 0,5) e viés zero.
    /// A mesma semente sempre produz o mesmo neurônio.
    /// @throws std::invalid_argument se n_inputs for 0 ou act for nulo.
    explicit Neuron(std::size_t n_inputs, std::uint32_t seed = 42, Activation act = step);

    /// Cria um neurônio a partir de parâmetros explícitos. @throws std::invalid_argument
    /// se weights estiver vazio ou act for nulo.
    Neuron(std::vector<double> weights, double bias, Activation act = step);

    /// /// Calcula a saída do neurônio para um vetor de entrada. @throws
    /// std::invalid_argument se input.size() != n_inputs().
    [[nodiscard]] double forward(std::span<const double> input) const;

    /// Um passo da regra de aprendizagem do perceptron: w <- w + lr * (alvo - y) * x, b
    /// <- b + lr * (alvo - y)
    /// @return true se o neurônio estava errado (parâmetros mudaram), caso contrário
    /// false.
    bool update(std::span<const double> input, double target, double learning_rate);

    [[nodiscard]] std::size_t n_inputs() const noexcept { return weights_.size(); }
    [[nodiscard]] std::span<const double> weights() const noexcept { return weights_; }
    [[nodiscard]] double bias() const noexcept { return bias_; }

   private:
    std::vector<double> weights_;
    double bias_{0.0};
    Activation activation_;
};

}  // namespace perceptron
