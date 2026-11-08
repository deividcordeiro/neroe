#include "neroe/serialization.hpp"

#include <cstddef>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace neroe {

void save(const Neuron& neuron, std::ostream& out) {
    if (neuron.activation() != step)
        throw std::runtime_error("save: only the step activation is supported");

    out.precision(std::numeric_limits<double>::max_digits10);
    out << "neroe-neuron 1\n";
    out << "activation step\n";
    out << "inputs " << neuron.n_inputs() << '\n';
    out << "bias " << neuron.bias() << '\n';
    out << "weights";
    for (const double w : neuron.weights()) out << ' ' << w;
    out << '\n';
}

Neuron load(std::istream& in) {
    std::string tag, key, activation;
    int version = 0;
    if (!(in >> tag >> version) || tag != "neroe-neuron" || version != 1)
        throw std::runtime_error("load: not a neroe-neuron v1 file");

    if (!(in >> key >> activation) || key != "activation" || activation != "step")
        throw std::runtime_error("load: unsupported activation");

    std::size_t n = 0;
    if (!(in >> key >> n) || key != "inputs" || n == 0)
        throw std::runtime_error("load: bad inputs");

    double bias = 0.0;
    if (!(in >> key >> bias) || key != "bias") throw std::runtime_error("load: bad bias");

    if (!(in >> key) || key != "weights") throw std::runtime_error("load: bad weights");

    std::vector<double> weights(n);
    for (double& w : weights)
        if (!(in >> w)) throw std::runtime_error("load: truncated weights");
    return Neuron(std::move(weights), bias);
}
}  // namespace neroe