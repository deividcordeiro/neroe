#pragma once

#include <span>
#include <string>
#include <vector>

#include "neroe/neuron.hpp"
#include "neroe/trainer.hpp"

namespace neroe {

struct TruthRow {
    std::vector<double> input;
    double output;
};

/// Output for every binary input combination (the first input is the most significant).
/// @throws std::invalid_argument if the neuron has more than 6 inputs.
[[nodiscard]] std::vector<TruthRow> truth_table(const Neuron& neuron);

/// Name of the boolean function a 2-input neuron implements (AND, OR, NAND, ...),
/// or "n/a ..." for other sizes. Measured from the neuron, not declared.
[[nodiscard]] std::string recognize(const Neuron& neuron);

/// Capability card: what this mind does, measured from the mind itself.
[[nodiscard]] std::string make_card(const Neuron& neuron);

/// ASCII map of where a 2-input neuron fires (#) or stays silent (.).
/// Samples are drawn on top: their target digit if the neuron gets them right,
/// '!' if it gets them wrong.
/// @throws std::invalid_argument if the neuron does not have exactly 2 inputs.
[[nodiscard]] std::string plot_boundary(const Neuron& neuron,
                                        std::span<const Sample> samples = {});

}  // namespace neroe