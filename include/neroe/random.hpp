#pragma once

#include <cstdint>

namespace neroe {

/// Marsaglia xorshift32 pseudo-random generator.
/// Tiny, fast and fully deterministic across compilers and platforms,
/// which makes experiments (and later, evolutionary runs) reproducible.
class Xorshift32 {
public:
    static constexpr std::uint32_t kDefaultSeed = 2463534242u;

    /// A zero seed would lock the generator at zero, so it is replaced by the default.
    explicit Xorshift32(std::uint32_t seed = kDefaultSeed) noexcept
        : state_(seed != 0 ? seed : kDefaultSeed) {}

    std::uint32_t next_u32() noexcept {
        state_ ^= state_ << 13;
        state_ ^= state_ >> 17;
        state_ ^= state_ << 5;
        return state_;
    }

    /// Uniform double in [0, 1).
    double next_double() noexcept {
        return static_cast<double>(next_u32()) / 4294967296.0;
    }

    /// Uniform double in [lo, hi).
    double uniform(double lo, double hi) noexcept {
        return lo + (hi - lo) * next_double();
    }

private:
    std::uint32_t state_;
};

}  // namespace neroe
