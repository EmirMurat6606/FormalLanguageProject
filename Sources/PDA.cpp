//
// Created by 2005e on 6/10/2024.
//

#include "../Headers/PDA.h"

PDA::PDA() {}

PDA::PDA(const std::string &filename) {

    // Open input stream
    std::ifstream input_file(filename);
    // Read json file
    json j;
    input_file >> j;

    start_state = j["StartState"];
    start_symbol = j["StartStack"];
    states = static_cast<StringSet>(j["States"]);
    input_alphabet = static_cast<StringSet>(j["Alphabet"]);
    stack_alphabet = static_cast<StringSet>(j["StackAlphabet"]);

    for (json::iterator it = j["Transitions"].begin(); it != j["Transitions"].end(); it++) {
        std::vector<std::string> replacement;
        for (const std::string stack_symbol: (*it)["replacement"]) {
            replacement.push_back(stack_symbol);
        }
        transitions[std::tuple<std::string, std::string, std::string>((*it)["from"], (*it)["input"],
                                                                      (*it)["stacktop"])].insert(
                std::make_pair((*it)["to"], replacement));;
    }
}

/**
 * Recursive helper function for toCFG
 * @return a production rule body as a string
 */
void generateBody(int iterations, const std::string &head, std::string body, CFG &cfg, const std::string &state,
                  const std::string &final_state, const std::vector<std::string> &replacement, int index,
                  const StringSet &states) {
    if (iterations == 0) {
        if (!body.empty()) {
            body += " ";
        }
        body += "[" + state + "," + replacement.back() + "," + final_state + "]";
        cfg.addProductionRule(head, body);
    } else {
        const std::string &current_symbol = replacement[index];
        for (const auto &new_state: states) {
            std::string new_body = body;
            if (!body.empty()) {
                new_body += " ";
            }
            new_body += "[" + state + "," + current_symbol + "," + new_state + "]"; // NOLINT
            generateBody(iterations - 1, head, new_body, cfg, new_state, final_state, replacement, index + 1, states);
        }
    }
}

CFG PDA::toCFG() {

    CFG cfg;
    // Set StartState
    cfg.setStartSymbol("S");
    // Set the Terminals
    cfg.setTerminals(input_alphabet);
    // Set the Variables
    cfg.addVariable("S");
    for (const auto &state1: states)
        for (const auto &state2: states)
            for (const auto &alphabet_symbol: stack_alphabet)
                cfg.addVariable("[" + state1 + "," + alphabet_symbol + "," + state2 + "]"); // NOLINT
    // Add the production rules
    for (const auto &state: states) {
        cfg.addProductionRule("S", "[" + start_state + "," + start_symbol + "," + state + "]");
    }

    for (const auto &transition: transitions) {
        for (const auto &next: transition.second) {
            std::string q = std::get<0>(transition.first);
            std::string a = std::get<1>(transition.first);
            std::string X = std::get<2>(transition.first);
            if (next.second.empty()) {
                // Case 1: Geen vervangingen
                cfg.addProductionRule("[" + q + "," + X + "," + next.first + "]", a); // NOLINT
            } else {
                // Case 2: Vervangingen zijn aanwezig
                for (const auto &state: states) {
                    std::string head = "[" + q + "," + X + "," + state + ']'; // NOLINT
                    generateBody(next.second.size() - 1, head, a, cfg, next.first, state, next.second, 0, states);
                }
            }
        }
    }
    return cfg;
}


