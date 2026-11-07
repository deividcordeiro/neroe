#include "neroe/trainer.hpp"

#include <stdexcept>

namespace neroe {

TrainResult train(Neuron& neuron, std::span<const Sample> samples,
                  double learning_rate, std::size_t max_epochs) {
    if (samples.empty()) throw std::invalid_argument("train: no samples");

    for (std::size_t epoch = 1; epoch <= max_epochs; ++epoch) {
        std::size_t mistakes = 0;
        for (const Sample& s : samples)
            if (neuron.update(s.input, s.target, learning_rate)) ++mistakes;

        if (mistakes == 0) return {epoch, true};
    }
    return {max_epochs, false};
}

}  // namespace neroe
