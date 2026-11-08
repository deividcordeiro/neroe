#pragma once

#include <cstdint>

namespace perceptron {

/// Gerador pseudoaleatório Marsaglia xorshift32.
/// Pequeno, rápido e totalmente determinístico entre compiladores e plataformas,
/// o que torna experimentos (e depois, execuções evolucionárias) reproduzíveis.
class Xorshift32 {
   public:
    static constexpr std::uint32_t kDefaultSeed = 2463534242u;

    /// Uma semente zero iria travar o gerador em zero, então ela é substituída pelo
    /// padrão.
    explicit Xorshift32(std::uint32_t seed = kDefaultSeed) noexcept
        : state_(seed != 0 ? seed : kDefaultSeed) {}

    std::uint32_t next_u32() noexcept {
        state_ ^= state_ << 13;
        state_ ^= state_ >> 17;
        state_ ^= state_ << 5;
        return state_;
    }

    double next_double() noexcept {
        return static_cast<double>(next_u32()) / 4294967296.0;
    }

    double uniform(double lo, double hi) noexcept {
        return lo + (hi - lo) * next_double();
    }

   private:
    std::uint32_t state_;
};

}  // namespace perceptron
