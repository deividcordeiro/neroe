# Neroe

O Neroe é um projeto em C++20 que implementa um único neurônio artificial, também conhecido como perceptron, desenvolvido do zero.

O projeto não utiliza bibliotecas externas. Até mesmo o gerador de números aleatórios e a estrutura de testes foram implementados sem dependências de terceiros.

A ideia é construir uma base simples e funcional para estudar como um neurônio aprende, ajusta seus parâmetros e pode evoluir com o uso de algoritmos evolutivos.

## Como funciona

O neurônio recebe valores de entrada, aplica pesos a cada um deles, soma os resultados com um viés (*bias*) e produz uma saída.

Seu funcionamento é baseado nas seguintes equações:

```text
y = f(w · x + b)

f = função degrau:
    1 se z >= 0
    0 caso contrário

Regra de aprendizado de Rosenblatt:

w <- w + lr * (target - y) * x
b <- b + lr * (target - y)
```

O treinamento permite que o neurônio aprenda operações lógicas como AND e OR, cujos dados podem ser separados por uma linha.

Já a operação XOR não pode ser aprendida por um único neurônio. Esse exemplo demonstra uma das limitações fundamentais do perceptron.

## Compilação e execução

**Requisitos:** compilador compatível com C++20 (GCC 10+, Clang 11+ ou MSVC 2019+). O CMake 3.20 ou superior é opcional.

**Sem CMake (GCC / MinGW / MSYS2):**

```bash
g++ -std=c++20 -Iinclude src/neuron.cpp src/trainer.cpp tests/test_main.cpp tests/test_neuron.cpp tests/test_random.cpp -o tests
./tests

g++ -std=c++20 -Iinclude src/neuron.cpp src/trainer.cpp examples/logic_gates.cpp -o logic_gates
./logic_gates
```

**Com CMake:**

```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

## Exemplo de uso

O exemplo abaixo cria um neurônio com duas entradas e o treina para aprender a operação lógica AND.

```cpp
#include "neroe/neuron.hpp"
#include "neroe/trainer.hpp"

using namespace neroe;

std::vector<Sample> and_gate{
    {{0,0},0},
    {{0,1},0},
    {{1,0},0},
    {{1,1},1}
};

Neuron neuron(2);

TrainResult result = train(neuron, and_gate, 0.1, 1000);

double y = neuron.forward(std::vector<double>{1, 1});
// Resultado esperado: 1.0
```

## Estrutura do projeto

```text
include/neroe/       Cabeçalhos públicos (Neuron, Trainer, activation, RNG)
src/                 Implementação
examples/            Exemplos com AND, OR e XOR
tests/               Testes independentes (minitest.hpp)
.github/workflows/   Testes automatizados no Windows e Linux
```

## Características

- **Determinístico:** utiliza um gerador de números aleatórios xorshift32 desenvolvido para o projeto. Com a mesma semente, o neurônio produz os mesmos valores iniciais em diferentes plataformas.

- **Funções de ativação:** o método `forward()` permite utilizar diferentes funções de ativação. Entretanto, a regra de aprendizado do perceptron é válida apenas para a função degrau. Por isso, `update()` e `train()` lançam `std::logic_error` quando outra função é utilizada.

- **Parâmetros organizados:** os pesos e o viés são armazenados como dados contíguos, facilitando seu uso futuro em algoritmos evolutivos. Para essa finalidade, basta utilizar o método `forward()`.

## Licença

MIT — consulte o arquivo [LICENSE](LICENSE).