#pragma once

namespace neroe {

/// /// Função de ativação: mapeia o valor de pré-ativação z = w.x + b para a saída do neurônio.
using Activation = double (*)(double);

/// Função degrau de Heaviside: 1,0 se z >= 0, caso contrário 0,0.
inline double step(double z) noexcept { return z >= 0.0 ? 1.0 : 0.0; }

}  // namespace neroe
