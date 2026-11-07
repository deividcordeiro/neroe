#include <vector>

#include "minitest.hpp"
#include "neroe/neuron.hpp"
#include "neroe/trainer.hpp"

using namespace neroe;

namespace {

const std::vector<Sample> kAnd{{{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}};
const std::vector<Sample> kOr{{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 1}};
const std::vector<Sample> kXor{{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 0}};

bool classifies_all(const Neuron& n, const std::vector<Sample>& data) {
    for (const Sample& s : data)
        if (n.forward(s.input) != s.target) return false;
    return true;
}

}  // namespace

TEST(neuron_forward_with_known_parameters) {
    const Neuron n({1.0, 1.0}, -1.5);  // behaves as AND
    CHECK(n.forward(std::vector<double>{0, 0}) == 0.0);
    CHECK(n.forward(std::vector<double>{1, 0}) == 0.0);
    CHECK(n.forward(std::vector<double>{1, 1}) == 1.0);
}

TEST(neuron_rejects_wrong_input_size) {
    const Neuron n(2);
    CHECK_THROWS(n.forward(std::vector<double>{1.0}));
}

TEST(neuron_rejects_invalid_construction) {
    CHECK_THROWS(Neuron(0));
    CHECK_THROWS(Neuron(std::vector<double>{}, 0.0));
    CHECK_THROWS(Neuron(2, 42, nullptr));
}

TEST(neuron_same_seed_gives_same_weights) {
    const Neuron a(3, 99), b(3, 99);
    for (std::size_t i = 0; i < 3; ++i) CHECK(a.weights()[i] == b.weights()[i]);
}

TEST(update_changes_nothing_when_correct) {
    Neuron n({1.0, 1.0}, -1.5);
    const bool changed = n.update(std::vector<double>{1, 1}, 1.0, 0.1);
    CHECK(!changed);
    CHECK_NEAR(n.weights()[0], 1.0, 1e-12);
    CHECK_NEAR(n.bias(), -1.5, 1e-12);
}

TEST(update_applies_perceptron_rule) {
    Neuron n({0.0, 0.0}, 0.0);  // z = 0 -> step = 1, but target is 0 -> error = -1
    const bool changed = n.update(std::vector<double>{1, 1}, 0.0, 0.1);
    CHECK(changed);
    CHECK_NEAR(n.weights()[0], -0.1, 1e-12);
    CHECK_NEAR(n.weights()[1], -0.1, 1e-12);
    CHECK_NEAR(n.bias(), -0.1, 1e-12);
}

TEST(train_learns_and) {
    Neuron n(2);
    const TrainResult r = train(n, kAnd, 0.1, 1000);
    CHECK(r.converged);
    CHECK(classifies_all(n, kAnd));
}

TEST(train_learns_or) {
    Neuron n(2);
    const TrainResult r = train(n, kOr, 0.1, 1000);
    CHECK(r.converged);
    CHECK(classifies_all(n, kOr));
}

TEST(train_cannot_learn_xor) {
    Neuron n(2);
    const TrainResult r = train(n, kXor, 0.1, 1000);
    CHECK(!r.converged);
    CHECK(!classifies_all(n, kXor));
}

TEST(train_rejects_empty_dataset) {
    Neuron n(2);
    CHECK_THROWS(train(n, std::vector<Sample>{}, 0.1, 10));
}

static double identity(double z) { return z; }

TEST(update_rejects_non_step_activation) {
    Neuron n(std::vector<double>{1.0}, 0.0, identity);
    CHECK(n.forward(std::vector<double>{2.0}) == 2.0);  // forward works with any activation
    CHECK_THROWS(n.update(std::vector<double>{1.0}, 1.0, 0.1));
}
