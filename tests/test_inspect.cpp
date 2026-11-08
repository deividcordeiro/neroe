#include <string>
#include <vector>

#include "minitest.hpp"
#include "neroe/inspect.hpp"

using namespace neroe;

TEST(explain_sums_contributions_and_matches_forward) {
    const Neuron n({1.0, 2.0}, -1.5);
    const std::vector<double> x{1.0, 1.0};
    const Explanation e = explain(n, x);

    CHECK_NEAR(e.contributions[0], 1.0, 1e-12);
    CHECK_NEAR(e.contributions[1], 2.0, 1e-12);
    CHECK_NEAR(e.z, 1.5, 1e-12);
    CHECK(e.output == n.forward(x));
}

TEST(explain_rejects_wrong_input_size) {
    const Neuron n(2);
    CHECK_THROWS(explain(n, std::vector<double>{1.0}));
}

TEST(describe_show_weights_and_bias) {
    const Neuron n({1.0, 1.0}, -1.5);
    const std::string text = describe(n);
    CHECK(text.find("+1*x1") != std::string::npos);
    CHECK(text.find("-1.5") != std::string::npos);
    CHECK(text.find(">= 0") != std::string::npos);
}