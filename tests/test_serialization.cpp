#include <sstream>

#include "minitest.hpp"
#include "neroe/serialization.hpp"

using namespace neroe;

TEST(save_and_load_round_trip_exactly) {
    const Neuron original(3, 123);
    std::stringstream buffer;
    save(original, buffer);

    const Neuron loaded = load(buffer);
    CHECK(loaded.n_inputs() == 3);
    CHECK(loaded.bias() == original.bias());
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(loaded.weights()[i] == original.weights()[i]);
}

TEST(load_rejects_garbage) {
    std::stringstream bad("Hello, World!");
    CHECK_THROWS(load(bad));
}
TEST(load_rejects_truncat_weights) {
    std::stringstream bad(
        "neroe-neuron 1\nactivation step\ninputs 3\nbias 0\nweights 1 2");
    CHECK_THROWS(load(bad));
}