#include "minitest.hpp"
#include "neroe/random.hpp"

using neroe::Xorshift32;

TEST(random_is_deterministic_for_same_seed) {
    Xorshift32 a(123), b(123);
    for (int i = 0; i < 100; ++i) CHECK(a.next_u32() == b.next_u32());
}

TEST(random_differs_for_different_seeds) {
    Xorshift32 a(1), b(2);
    CHECK(a.next_u32() != b.next_u32());
}

TEST(random_zero_seed_does_not_lock_at_zero) {
    Xorshift32 r(0);
    CHECK(r.next_u32() != 0);
}

TEST(random_uniform_stays_in_range) {
    Xorshift32 r(7);
    for (int i = 0; i < 10000; ++i) {
        const double v = r.uniform(-0.5, 0.5);
        CHECK(v >= -0.5 && v < 0.5);
    }
}
