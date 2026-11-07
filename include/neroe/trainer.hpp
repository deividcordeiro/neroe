#pragma once

#include <cstddef>
#include <span>
#include <vector>

#include "neroe/neuron.hpp"

namespace neroe {

/// One labeled training example.
struct Sample {
    std::vector<double> input;
    double target;
};

struct TrainResult {
    std::size_t epochs;  ///< Epochs executed (including the final error-free one).
    bool converged;      ///< True if an epoch finished with zero mistakes.
};

/// Trains the neuron with the perceptron rule, visiting samples in order.
/// Stops at the first epoch without mistakes, or after max_epochs.
/// Guaranteed to converge only if the data is linearly separable.
/// @throws std::invalid_argument if samples is empty.
/// @throws std::logic_error if the neuron does not use the step activation.
TrainResult train(Neuron& neuron, std::span<const Sample> samples,
                  double learning_rate, std::size_t max_epochs);

}  // namespace neroe
