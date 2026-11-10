// neroe: a terminal portal into a neuron's mind.
//
//   neroe                       interactive shell
//   neroe learn and             watch a neuron learn, live
//   neroe card and.neuron       what does this mind do?
//   neroe ask and.neuron 1 1    what does it answer?
//   neroe why and.neuron 1 1    why?

#include <chrono>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#include "neroe/card.hpp"
#include "neroe/inspect.hpp"
#include "neroe/neuron.hpp"
#include "neroe/serialization.hpp"
#include "neroe/trainer.hpp"

namespace {

using namespace neroe;
using Tokens = std::vector<std::string>;

struct Session {
    std::optional<Neuron> neuron;
};

void enable_ansi() {
#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(handle, &mode))
        SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

std::vector<Sample> gate_data(const std::string& name) {
    if (name == "and") return {{{0, 0}, 0}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 1}};
    if (name == "or") return {{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 1}};
    if (name == "nand") return {{{0, 0}, 1}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 0}};
    if (name == "nor") return {{{0, 0}, 1}, {{0, 1}, 0}, {{1, 0}, 0}, {{1, 1}, 0}};
    if (name == "xor") return {{{0, 0}, 0}, {{0, 1}, 1}, {{1, 0}, 1}, {{1, 1}, 0}};
    throw std::invalid_argument("unknown gate '" + name +
                                "' (try: and, or, nand, nor, xor)");
}

const Neuron& require(const Session& session) {
    if (!session.neuron)
        throw std::runtime_error("no mind loaded. Use 'learn <gate>' or 'load <file>'.");
    return *session.neuron;
}

std::vector<double> numbers(const Tokens& t, std::size_t from) {
    std::vector<double> values;
    for (std::size_t i = from; i < t.size(); ++i) {
        try {
            values.push_back(std::stod(t[i]));
        } catch (const std::exception&) {
            throw std::invalid_argument("not a number: " + t[i]);
        }
    }
    return values;
}

void draw_frame(const Neuron& neuron, const std::vector<Sample>& data,
                const std::string& gate, std::size_t epoch, bool animate) {
    if (animate) std::cout << "\x1b[H\x1b[J";  // cursor home + clear screen
    std::cout << "learning " << gate << "   epoch " << epoch << "\n\n"
              << plot_boundary(neuron, data) << '\n'
              << "# fires   . silent   0/1 target met   ! target missed\n\n"
              << describe(neuron) << std::flush;
}

void learn(Session& session, const Tokens& t) {
    if (t.size() < 2) throw std::invalid_argument("usage: learn <gate> [file] [--plain]");

    const std::string gate = t[1];
    bool plain = false;
    std::string file;
    for (std::size_t i = 2; i < t.size(); ++i) {
        if (t[i] == "--plain")
            plain = true;
        else
            file = t[i];
    }

    const std::vector<Sample> data = gate_data(gate);
    Neuron neuron(2);

    constexpr std::size_t kMaxEpochs = 80;
    std::size_t epoch = 0;
    bool converged = false;
    while (epoch < kMaxEpochs && !converged) {
        ++epoch;
        converged = train(neuron, data, 0.1, 1).converged;  // exactly one epoch
        if (!plain) {
            draw_frame(neuron, data, gate, epoch, true);
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }
    if (plain) draw_frame(neuron, data, gate, epoch, false);

    if (converged) {
        std::cout << "\n\nLearned " << gate << " in " << epoch << " epoch(s).\n";
    } else {
        std::cout << "\n\nDid NOT converge after " << epoch
                  << " epochs. A single step neuron draws one straight line,"
                     " and this gate needs more than that.\n";
    }
    session.neuron = neuron;

    if (converged && !file.empty()) {
        std::ofstream out(file);
        if (!out) throw std::runtime_error("cannot write " + file);
        save(neuron, out);
        std::cout << "Saved to " << file << ".\n";
    }
}

void print_help() {
    std::cout << "Commands:\n"
                 "  learn <gate> [file] [--plain]  watch a neuron learn (and, or, nand, "
                 "nor, xor)\n"
                 "  load <file>                    bring a saved mind back\n"
                 "  save <file>                    write the current mind to disk\n"
                 "  card                           what this mind does, measured from "
                 "the mind itself\n"
                 "  map                            draw where this mind fires\n"
                 "  ask <x1> <x2> ...              what does the mind answer?\n"
                 "  why <x1> <x2> ...              why that answer?\n"
                 "  quit\n"
                 "One-shot: neroe card and.neuron | neroe ask and.neuron 1 1 | neroe why "
                 "and.neuron 1 1\n";
}

// Returns false when the user asked to quit.
bool run(Session& session, const Tokens& t) {
    if (t.empty()) return true;
    const std::string& cmd = t[0];
    try {
        if (cmd == "quit" || cmd == "exit") return false;
        if (cmd == "help") {
            print_help();
        } else if (cmd == "learn") {
            learn(session, t);
        } else if (cmd == "load") {
            if (t.size() < 2) throw std::invalid_argument("usage: load <file>");
            std::ifstream in(t[1]);
            if (!in) throw std::runtime_error("cannot open " + t[1]);
            session.neuron = load(in);
            std::cout << "Loaded " << t[1] << ".\n";
        } else if (cmd == "save") {
            if (t.size() < 2) throw std::invalid_argument("usage: save <file>");
            std::ofstream out(t[1]);
            if (!out) throw std::runtime_error("cannot write " + t[1]);
            save(require(session), out);
            std::cout << "Saved to " << t[1] << ".\n";
        } else if (cmd == "card") {
            std::cout << make_card(require(session));
        } else if (cmd == "map") {
            std::cout << plot_boundary(require(session));
        } else if (cmd == "ask") {
            std::cout << require(session).forward(numbers(t, 1)) << '\n';
        } else if (cmd == "why") {
            std::cout << to_text(explain(require(session), numbers(t, 1)));
        } else {
            std::cout << "unknown command '" << cmd << "'. Type 'help'.\n";
        }
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << '\n';
    }
    return true;
}

Tokens split(const std::string& line) {
    std::istringstream in(line);
    Tokens tokens;
    for (std::string word; in >> word;) tokens.push_back(word);
    return tokens;
}

int shell(Session& session) {
    std::cout << "Neroe mind portal. Type 'help'.\n";
    std::string line;
    while (std::cout << "neroe> " << std::flush, std::getline(std::cin, line)) {
        if (!run(session, split(line))) break;
    }
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    enable_ansi();
    Session session;
    const Tokens args(argv + 1, argv + argc);

    if (args.empty()) return shell(session);

    const std::string& cmd = args[0];
    if (cmd == "shell") {
        if (args.size() > 1) run(session, {"load", args[1]});
        return shell(session);
    }
    if (cmd == "learn" || cmd == "help") {
        run(session, args);
        return 0;
    }

    // One-shot form: neroe <command> <file> [numbers...]
    if (args.size() < 2) {
        std::cout << "usage: neroe " << cmd << " <file> [numbers...]\n";
        return 1;
    }
    run(session, {"load", args[1]});
    if (!session.neuron) return 1;
    Tokens rest{cmd};
    rest.insert(rest.end(), args.begin() + 2, args.end());
    run(session, rest);
    return 0;
}