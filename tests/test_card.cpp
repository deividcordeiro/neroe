#include <string>
#include <vector>

#include "minitest.hpp"
#include "neroe/card.hpp"

using namespace neroe;

namespace {

const std::vector<Sample> kAnd{{{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}};

std::size_t count_char(const std::string& text, char c) {
    std::size_t n = 0;
    for (const char x : text)
        if (x == c) ++n;
    return n;
}

}  // namespace

TEST(truth_table_lists_every_binary_input) {
    const Neuron n({1.0, 1.0}, -1.5);
    const std::vector<TruthRow> rows = truth_table(n);
    CHECK(rows.size() == 4);
    CHECK(rows[0].output == 0.0);
    CHECK(rows[1].output == 0.0);
    CHECK(rows[2].output == 0.0);
    CHECK(rows[3].output == 1.0);
    CHECK(rows[1].input[0] == 0.0 && rows[1].input[1] == 1.0);
}

TEST(truth_table_rejects_too_many_inputs) {
    const Neuron n(7);
    CHECK_THROWS(truth_table(n));
}

TEST(recognize_names_known_gates) {
    CHECK(recognize(Neuron({1.0, 1.0}, -1.5)) == "AND");
    CHECK(recognize(Neuron({1.0, 1.0}, -0.5)) == "OR");
    CHECK(recognize(Neuron({-1.0, -1.0}, 1.5)) == "NAND");
    CHECK(recognize(Neuron({-1.0, -1.0}, 0.5)) == "NOR");
}

TEST(recognize_only_names_two_input_neurons) {
    CHECK(recognize(Neuron(3)).rfind("n/a", 0) == 0);
}

TEST(card_mentions_rule_and_behavior) {
    const std::string card = make_card(Neuron({1.0, 1.0}, -1.5));
    CHECK(card.find("behaves as:  AND") != std::string::npos);
    CHECK(card.find(">= 0") != std::string::npos);
    CHECK(card.find("truth table:") != std::string::npos);
}

TEST(plot_has_expected_shape_and_marks_correct_samples) {
    const std::string plot = plot_boundary(Neuron({1.0, 1.0}, -1.5), kAnd);
    CHECK(count_char(plot, '\n') == 13);
    CHECK(plot.find('#') != std::string::npos);
    CHECK(plot.find('.') != std::string::npos);
    CHECK(count_char(plot, '1') == 1);
    CHECK(count_char(plot, '0') == 3);
    CHECK(plot.find('!') == std::string::npos);
}

TEST(plot_marks_wrong_samples_with_bang) {
    // Weights and bias zero: z = 0 -> fires everywhere, so the three 0-targets are wrong.
    const std::string plot = plot_boundary(Neuron({0.0, 0.0}, 0.0), kAnd);
    CHECK(count_char(plot, '!') == 3);
}

TEST(plot_rejects_non_two_input_neuron) { CHECK_THROWS(plot_boundary(Neuron(3))); }