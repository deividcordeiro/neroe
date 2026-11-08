#pragma once

#include <iosfwd>

#include "neroe/neuron.hpp"

namespace neroe {
void save(const Neuron& neuron, std::ostream& out);
Neuron load(std::istream& in);  // lança std::runtime_error se inválido

}  // namespace neroe