#pragma once

namespace neroe {

/// Activation function: maps the pre-activation value z = w.x + b to the neuron output.
using Activation = double (*)(double);

/// Heaviside step function: 1.0 if z >= 0, otherwise 0.0.
inline double step(double z) noexcept { return z >= 0.0 ? 1.0 : 0.0; }

}  // namespace neroe
