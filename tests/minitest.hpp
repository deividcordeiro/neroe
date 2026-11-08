#pragma once

// Minimal self-contained test framework (no third-party code).
//
//   TEST(name) { CHECK(condition); CHECK_NEAR(a, b, eps); CHECK_THROWS(expr); }

#include <cmath>
#include <exception>
#include <iostream>
#include <vector>

namespace minitest {

struct Case {
    const char* name;
    void (*fn)();
};

inline std::vector<Case>& registry() {
    static std::vector<Case> cases;
    return cases;
}

inline int& failures() {
    static int count = 0;
    return count;
}

struct Registrar {
    Registrar(const char* name, void (*fn)()) { registry().push_back({name, fn}); }
};

inline int run_all() {
    std::size_t failed_tests = 0;
    for (const Case& c : registry()) {
        const int before = failures();
        try {
            c.fn();
        } catch (const std::exception& e) {
            std::cerr << "uncaught exception in " << c.name << ": " << e.what() << '\n';
            ++failures();
        } catch (...) {
            std::cerr << "uncaught unknown exception in " << c.name << '\n';
            ++failures();
        }
        const bool ok = failures() == before;
        std::cout << (ok ? "[ PASS ] " : "[ FAIL ] ") << c.name << '\n';
        if (!ok) ++failed_tests;
    }
    std::cout << '\n'
              << (registry().size() - failed_tests) << "/" << registry().size()
              << " tests passed\n";
    return failed_tests == 0 ? 0 : 1;
}

}  // namespace minitest

#define TEST(name)                                                  \
    static void name();                                             \
    static const minitest::Registrar registrar_##name(#name, name); \
    static void name()

#define CHECK(cond)                                                                    \
    do {                                                                               \
        if (!(cond)) {                                                                 \
            std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK failed: " #cond "\n"; \
            ++minitest::failures();                                                    \
        }                                                                              \
    } while (0)

#define CHECK_NEAR(a, b, eps) CHECK(std::fabs((a) - (b)) <= (eps))

#define CHECK_THROWS(expr)   \
    do {                     \
        bool threw_ = false; \
        try {                \
            (void)(expr);    \
        } catch (...) {      \
            threw_ = true;   \
        }                    \
        CHECK(threw_);       \
    } while (0)
