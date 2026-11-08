#pragma once

#include <cstddef>
#include <span>
#include <vector>

#include "perceptron/neuron.hpp"

namespace perceptron {

//Um exemplo de treinamento.
struct Sample {
    std::vector<double> input;
    double target;
};

struct TrainResult {
    std::size_t epochs;  ////< Épocas executadas (incluindo a última sem erros).
    bool converged;     ///< TRUE se uma época terminou sem erros.
};

/// Trains the neuron with the perceptron rule, visiting samples in order.
/// Stops at the first epoch without mistakes, or after max_epochs.
/// Guaranteed to converge only if the data is linearly separable.
/// @throws std::invalid_argument if samples is empty.
TrainResult train(Neuron& neuron, std::span<const Sample> samples,
                  double learning_rate, std::size_t max_epochs);

}  // namespace perceptron
