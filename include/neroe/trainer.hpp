#pragma once

#include <cstddef>
#include <span>
#include <vector>

#include "neroe/neuron.hpp"

namespace neroe {

//// Um exemplo de treinamento.
struct Sample {
    std::vector<double> input;
    double target;
};

struct TrainResult {
    std::size_t epochs;  ///< Épocas executadas (incluindo a última sem erros).
    bool converged;      ///< Verdadeiro se uma época terminou sem nenhum erro.
};

TrainResult train(Neuron& neuron, std::span<const Sample> samples,
                  double learning_rate, std::size_t max_epochs);

}  // namespace neroe
