#include "neroe/card.hpp"

#include <cmath>
#include <cstddef>
#include <sstream>
#include <stdexcept>

#include "neroe/inspect.hpp"

namespace neroe {

std::vector<TruthRow> truth_table(const Neuron& neuron) {
    const std::size_t n = neuron.n_inputs();
    if (n > 6) throw std::invalid_argument("truth_table: at most 6 inputs");

    const std::size_t count = std::size_t{1} << n;
    std::vector<TruthRow> rows;
    rows.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        std::vector<double> input(n);
        for (std::size_t j = 0; j < n; ++j)
            input[j] = static_cast<double>((i >> (n - 1 - j)) & 1u);
        const double output = neuron.forward(input);
        rows.push_back({std::move(input), output});
    }
    return rows;
}

std::string recognize(const Neuron& neuron) {
    if (neuron.n_inputs() != 2) return "n/a (only 2-input neurons are named)";

    std::string bits;
    for (const TruthRow& row : truth_table(neuron))
        bits += (row.output >= 0.5 ? '1' : '0');

    // Outputs for inputs (0,0) (0,1) (1,0) (1,1), in that order.
    struct Entry {
        const char* bits;
        const char* name;
    };
    static constexpr Entry kNames[] = {{"0000", "FALSE (never fires)"},
                                       {"0001", "AND"},
                                       {"0010", "x1 AND NOT x2"},
                                       {"0011", "x1"},
                                       {"0100", "NOT x1 AND x2"},
                                       {"0101", "x2"},
                                       {"0110", "XOR"},
                                       {"0111", "OR"},
                                       {"1000", "NOR"},
                                       {"1001", "XNOR"},
                                       {"1010", "NOT x2"},
                                       {"1011", "x1 OR NOT x2"},
                                       {"1100", "NOT x1"},
                                       {"1101", "NOT x1 OR x2"},
                                       {"1110", "NAND"},
                                       {"1111", "TRUE (always fires)"}};
    for (const Entry& e : kNames)
        if (bits == e.bits) return e.name;
    return "unknown";
}

std::string make_card(const Neuron& neuron) {
    std::string rule = describe(neuron);
    while (!rule.empty() && rule.back() == '\n') rule.pop_back();

    std::ostringstream out;
    out << "inputs:      " << neuron.n_inputs() << '\n';
    out << "rule:        " << rule << '\n';
    out << "behaves as:  " << recognize(neuron) << '\n';
    if (neuron.n_inputs() <= 6) {
        out << "truth table:\n";
        for (const TruthRow& row : truth_table(neuron)) {
            out << "  ";
            for (const double x : row.input) out << x << ' ';
            out << "-> " << row.output << '\n';
        }
    }
    return out.str();
}

std::string plot_boundary(const Neuron& neuron, std::span<const Sample> samples) {
    if (neuron.n_inputs() != 2)
        throw std::invalid_argument("plot_boundary: needs a 2-input neuron");

    constexpr int kCols = 25;
    constexpr int kRows = 13;
    constexpr double kMin = -0.5;  // both axes cover [-0.5, 1.5]
    constexpr double kSpan = 2.0;

    std::vector<std::string> grid(kRows, std::string(kCols, '.'));
    for (int r = 0; r < kRows; ++r) {
        for (int c = 0; c < kCols; ++c) {
            const double point[2] = {kMin + kSpan * c / (kCols - 1),
                                     kMin + kSpan * (kRows - 1 - r) / (kRows - 1)};
            if (neuron.forward(point) >= 0.5) grid[r][c] = '#';
        }
    }

    for (const Sample& s : samples) {
        if (s.input.size() != 2) continue;
        const int c =
            static_cast<int>(std::lround((s.input[0] - kMin) / kSpan * (kCols - 1)));
        const int r = static_cast<int>(
            std::lround((kRows - 1) - (s.input[1] - kMin) / kSpan * (kRows - 1)));
        if (c < 0 || c >= kCols || r < 0 || r >= kRows) continue;

        const bool wants = s.target >= 0.5;
        const bool fires = neuron.forward(s.input) >= 0.5;
        grid[r][c] = (wants == fires) ? (wants ? '1' : '0') : '!';
    }

    std::string out;
    for (const std::string& line : grid) out += line + '\n';
    return out;
}

}  // namespace neroe