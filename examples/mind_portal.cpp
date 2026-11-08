#include <fstream>
#include <iostream>
#include <vector>

#include "neroe/inspect.hpp"
#include "neroe/neuron.hpp"
#include "neroe/serialization.hpp"
#include "neroe/trainer.hpp"

int main() {
    using namespace neroe;

    const std::vector<Sample> and_gate{
        {{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}};
    Neuron neuron(2);
    const TrainResult result = train(neuron, and_gate, 0.1, 1000);
    std::cout << "learned AND in " << result.epochs << " epoch(s)\n\n";

    std::cout << describe(neuron) << '\n';
    for (const Sample& s : and_gate) {
        std::cout << "--- input (" << s.input[0] << ", " << s.input[1] << ") ---\n"
                  << to_text(explain(neuron, s.input)) << '\n';
    }

    {
        std::ofstream out("and.neuron");
        if (!out) {
            std::cerr << "cannot write and.neuron\n";
            return 1;
        }
        save(neuron, out);
    }
    std::ifstream in("and.neuron");
    if (!in) {
        std::cerr << "cannot read and.neuron\n";
        return 1;
    }
    const Neuron restored = load(in);
    std::cout << "Restore from and.neuron:\n" << describe(restored);
    return 0;
}