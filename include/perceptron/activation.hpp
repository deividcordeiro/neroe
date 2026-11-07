#pragma once

namespace perceptron {

/// Função de ativação: mapeia o valor pré-ativação z = w.x + b para a saída do neuronio.
//Ponteiro para função: qualquer função double -> double serve. Permite trocar a função sem mecher no neuronio.
using Activation = double (*)(double);

/// Função degrau de Heaviside: 1,0 se z >= 0, caso contrário 0,0.
//Permite definir a função num header sem erro de duplicação.
inline double step(double z) noexcept { return z >= 0.0 ? 1.0 : 0.0; }

}  // namespace perceptron
