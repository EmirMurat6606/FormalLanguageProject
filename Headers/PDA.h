/**
 * @brief: This file contains the declaration of the PDA class
 * @author: Emir Murat
 * @date: 06/10/2024
 */

#ifndef CFG_PDA_H
#define CFG_PDA_H

#include <tuple>

#include "json.hpp"
#include "CFG.h"

class PDA {

private:

    /**
     * Finite set of states
     */
    StringSet states;

    /**
     * Finite input alphabet
     */
    StringSet input_alphabet;

    /**
     * Finite stack alphabet
     */
    StringSet stack_alphabet;

    /**
     * Start state
     */
    std::string start_state;

    /**
     * Start symbol of the stack
     */
    std::string start_symbol;

    /**
     * Finite set of accepting states;
     */
    StringSet accepting_states;

    /**
     * Transition function of the PDA
     */
    std::map<std::tuple<std::string, std::string, std::string>, std::set<std::pair<std::string, std::vector<std::string>>>> transitions;


public:

    /**
     * Empty base constructor of the PDA class
     */
    PDA();

    /**
     * Constructor of the PDA class
     * @param filename the json file containing the initialization of the automaton
     */
    PDA(const std::string &filename);

    /**
     * Converts a PDA to a CFG
     * @return a CFG that is equivalent to the PDA
     */
    CFG toCFG();


};


#endif //CFG_PDA_H
