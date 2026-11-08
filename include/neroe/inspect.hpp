#pragma once

#include <span>
#include <string>
#include <vector>

#include "neroe/neuron.hpp"

namespace neroe {

struct Explanation {
    std::vector<double> contributions;
    double bias;
    double z;
    double output;
};

[[nodiscard]] Explanation explain(const Neuron& neuron, std::span<const double> input);

[[nodiscard]] std::string to_text(const Explanation& explanation);

[[nodiscard]] std::string describe(const Neuron& neuron);

}  // namespace neroe