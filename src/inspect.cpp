#include "neroe/inspect.hpp"

#include <cstddef>
#include <sstream>
#include <stdexcept>

namespace neroe {
Explanation explain(const Neuron& neuron, std::span<const double> input) {
    if (input.size() != neuron.n_inputs())
        throw std::invalid_argument("explain: input size mismatch");

    Explanation e;
    e.bias = neuron.bias();
    e.z = e.bias;
    e.contributions.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) {
        const double c = neuron.weights()[i] * input[i];
        e.contributions.push_back(c);
        e.z += c;
    }
    e.output = neuron.activation()(e.z);
    return e;
}

std::string to_text(const Explanation& e) {
    std::ostringstream out;
    out << std::showpos;
    for (std::size_t i = 0; i < e.contributions.size(); ++i)
        out << "input " << i + 1 << ": " << e.contributions[i] << '\n';
    out << "bias:    " << e.bias << '\n';
    out << "z:    " << e.z << '\n';
    out << std::noshowpos << "output:  " << e.output << '\n';
    return out.str();
}

std::string describe(const Neuron& neuron) {
    std::ostringstream out;
    out << std::showpos;
    out << (neuron.activation() == step ? "fires 1 when:" : "output = f(...) with:");
    for (std::size_t i = 0; i < neuron.n_inputs(); ++i)
        out << ' ' << neuron.weights()[i] << "*x" << i + 1;
    out << ' ' << neuron.bias();
    if (neuron.activation() == step) out << " >= 0";
    out << '\n';
    return out.str();
}

}  // namespace neroe