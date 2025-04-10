//
// Created by 2005e on 26/09/2024.
//


#include "../Headers/CFG.h"


StringSet findExistingRule(const std::string &terminal, const std::map<std::string, StringSet> &productions) {
    StringSet variables;
    for (const auto &rule: productions) {
        if (rule.second.find(terminal) != rule.second.end()) {
            variables.insert(rule.first);
        }
    }
    return variables;
}

StringSet cartesianProduct(const StringSet &set1, const StringSet &set2) {
    StringSet variable_combinations;
    for (const auto &variable1: set1) {
        for (const auto &variable2: set2) {
            variable_combinations.insert(variable1 + " " + variable2);
        }
    }
    return variable_combinations;
}


CFG::CFG() {}

CFG::CFG(std::string filename) {

    // Open input stream
    std::ifstream input_file(filename);

    // Read json file
    json j;
    input_file >> j;

    variables = static_cast<StringSet>(j["Variables"]);
    terminals = static_cast<StringSet>(j["Terminals"]);
    start_symbol = j["Start"];

    for (json::iterator it = j["Productions"].begin(); it != j["Productions"].end(); it++) {
        std::string head = (*it)["head"];
        std::string body;
        for (const std::string symbol: (*it)["body"]) {
            body.empty() ? body += symbol : body += " " + symbol;
        }
        production_rules[head].insert(body);
    }
}

void CFG::print() const {
    std::cout << "V = " << Logger::setToString(variables) << std::endl;
    std::cout << "T = " << Logger::setToString(terminals) << std::endl;
    Logger::printProductions(production_rules);
    std::cout << "S = " << start_symbol << std::endl;
}

void CFG::addVariable(const std::string &variable) {
    variables.insert(variable);
}

void CFG::setTerminals(const StringSet &terminal_list) {
    CFG::terminals = terminal_list;
}

void CFG::setStartSymbol(const std::string &startSymbol) {
    start_symbol = startSymbol;
}

void CFG::addProductionRule(const std::string head, const std::string body) {
    production_rules[head].insert(body);
}

bool CFG::isTerminal(const std::string &symbol) {
    if (terminals.find(symbol) != terminals.end()) {
        return true;
    }
    return false;
}

StringSet CFG::calculateFirst(const std::string &variable, StringSet firstSet, ParseTable &table) {

    for (const auto &body: production_rules[variable]) {

        std::istringstream iss(body);
        std::string symbol;

        bool epsilon_included = true;

        while (iss >> symbol) {
            if (isTerminal(symbol)) {
                firstSet.insert(symbol);
                table.setParseRule(variable, symbol, "`" + body + "`");
                epsilon_included = false;
                break;
            } else {
                StringSet subFirstSet = calculateFirst(symbol, {}, table);

                for (const auto &s: subFirstSet) {
                    if (s != " ") {
                        firstSet.insert(s);
                        table.setParseRule(variable, s, "`" + body + "`");
                    }
                }
                if (subFirstSet.find(" ") == subFirstSet.end()) {
                    epsilon_included = false;
                    break;
                }
            }
        }
        if (epsilon_included) {
            firstSet.insert(" ");
            if (body.empty())
                table.setFollowRule(variable, " ");
            else
                table.setFollowRule(variable, "`" + body + "`");
        }
    }
    return firstSet;
}

StringSet CFG::calculateFollow(const std::string &variable, std::map<std::string, StringSet> &firstSet,
                               std::map<std::string, StringSet> &followSet) {
    // If followSet for the variable is already calculated, return it
    if (!followSet[variable].empty()) {
        return followSet[variable];
    }

    if (variable == start_symbol) {
        followSet[variable].insert("<EOS>");
    }

    for (const auto &production: production_rules) {
        for (const auto &body: production.second) {
            std::istringstream iss(body);
            std::string symbol;
            std::string next_symbol;

            // Process the body of each production
            while (iss >> symbol) {
                if (symbol == variable) {
                    // Save current position
                    std::streampos current_pos = iss.tellg();

                    if (iss >> next_symbol) {
                        // Add FIRST(next_symbol) to FOLLOW(variable)
                        if (isTerminal(next_symbol)) {
                            followSet[variable].insert(next_symbol);
                        } else {
                            StringSet firstNext = firstSet[next_symbol];
                            for (const auto &next_first: firstNext) {
                                if (next_first != " ") {
                                    followSet[variable].insert(next_first);
                                } else {
                                    StringSet follow_next = calculateFollow(next_symbol, firstSet, followSet);
                                    followSet[variable].insert(follow_next.begin(), follow_next.end());
                                }
                            }
                        }
                    } else {
                        StringSet follow_head = calculateFollow(production.first, firstSet, followSet);
                        followSet[variable].insert(follow_head.begin(), follow_head.end());
                    }
                    iss.clear();
                    iss.seekg(current_pos); // Reset the stream to continue
                }
            }
        }
    }
    return followSet[variable];
}


void CFG::accepts(const std::string &input_string) {

    std::vector<std::vector<TableObject>> table;
    AssociationGroup group;

    for (int i = 0; i < input_string.size(); i++) {
        std::vector<TableObject> row;
        for (int j = 0; j < input_string.size() - i; j++) {
            TableObject table_object;
            table_object.setToken(input_string.substr(j, i + 1));
            row.push_back(table_object);
        }
        table.push_back(row);
    }
    // Do the first row
    for (auto &table_object: table[0]) {
        StringSet variables = findExistingRule(table_object.getToken(), production_rules);
        table_object.setVariables(variables);
        group.setAssociation(table_object.getToken(), variables);
    }

    for (int i = 1; i < table.size(); i++) {
        for (auto &table_object: table[i]) {
            StringSet body_group;

            for (const auto &token_split: table_object.getTokenCombination()) {
                StringSet bodies = (cartesianProduct(group.getAssociativeVariables(token_split.first),
                                                     group.getAssociativeVariables(token_split.second)));
                body_group.insert(bodies.begin(), bodies.end());

            }
            StringSet final_variables;
            for (const auto &string: body_group) {
                StringSet variables = findExistingRule(string, production_rules);
                final_variables.insert(variables.begin(), variables.end());
            }
            group.setAssociation(table_object.getToken(), final_variables);
            table_object.setVariables(final_variables);
        }
    }

    // Print the table
    Logger::printAcceptanceTable(table);

    // Print the acceptance boolean
    std::cout << std::boolalpha;
    std::cout << (table[input_string.size() - 1][0].getVariables().find(start_symbol) !=
                  table[input_string.size() - 1][0].getVariables().end()) << std::endl;

}


void CFG::ll() {

    std::cout << ">>> Building LL(1) Table" << std::endl;

//     FIRST en FOLLOW sets
    std::map<std::string, StringSet> first_set;
    std::map<std::string, StringSet> follow_set;

    // Parse Table definition
    StringSet new_symbols = terminals;
    new_symbols.insert("<EOS>");
    ParseTable table{variables, new_symbols};

    std::cout << " >> FIRST:" << std::endl;
    for (const auto &variable: variables) {
        first_set[variable] = calculateFirst(variable, {}, table);
        std::cout << "    " + variable + ": " + Logger::setToString(first_set[variable]) << std::endl;
    }
    table.setFirstSet(first_set);

    std::cout << " >> FOLLOW:" << std::endl;
    for (const auto &variable: variables) {
        follow_set[variable] = calculateFollow(variable, first_set, follow_set);
        std::cout << "    " + variable + ": " + Logger::setToString(follow_set[variable]) << std::endl;
    }
    table.setFollowSet(follow_set);

    std::cout << ">>> Table is built.\n" << std::endl;
    std::cout << "-------------------------------------\n" << std::endl;

    Logger::printTable(table);
//    Logger::printJSON(table);
}

ParseTable::ParseTable(const StringSet &variables, StringSet symbols) : variables{variables},
                                                                        symbols{std::move(symbols)} {
    firstSetAdded = false;
}


void ParseTable::setParseRule(const std::string &variable, const std::string &symbol, const std::string &production) {
    if (!Table[variable][symbol].empty() && (Table[variable][symbol] != production)) {
        ParseTable::Table[variable][symbol] = "<ERR>";
        return;
    }
    ParseTable::Table[variable][symbol] = production;
}

void ParseTable::setFirstSet(const std::map<std::string, StringSet> &first_set) {
    ParseTable::firstSet = first_set;
    ParseTable::firstSetAdded = true;
}

void ParseTable::setFollowSet(const std::map<std::string, StringSet> &follow_set) {
    if (firstSetAdded) {
        ParseTable::followSet = follow_set;
        for (const auto &rule: followRule) {
            for (const auto &body: rule.second) {
                for (const auto &value: followSet[rule.first]) {
                    this->setParseRule(rule.first, value, body);
                }
            }
        }
        for (const auto &variable: variables) {
            for (const auto &symbol: symbols) {
                if (Table[variable][symbol].empty()) {
                    Table[variable][symbol] = "<ERR>";
                }
            }
        }
    } else
        throw (std::runtime_error("ParseTable error::function setFirstSet has to be called before setFollowSet"));
}

void ParseTable::setFollowRule(const std::string &variable, const std::string &rule) {
    ParseTable::followRule[variable].insert(rule);
}

const StringSet &TableObject::getVariables() const {
    return variables;
}

void TableObject::setVariables(const StringSet &vars) {
    TableObject::variables = vars;
}

const std::string &TableObject::getToken() const {
    return token;
}

void TableObject::setToken(std::string Token) {
    TableObject::token = std::move(Token);
}

std::set<std::pair<std::string, std::string>> TableObject::getTokenCombination() const {
    std::set<std::pair<std::string, std::string>> token_combinations;
    if (token.size() == 1) {
        token_combinations.insert(std::make_pair(token, ""));
    } else {
        for (int i = 0; i < token.size() - 1; i++) {
            token_combinations.insert(
                    std::pair<std::string, std::string>{token.substr(0, i + 1), token.substr(i + 1, token.size())});
        }
    }
    return token_combinations;
}

void AssociationGroup::setAssociation(const std::string &string, const StringSet &variables) {
    associations[string] = variables;
}

StringSet &AssociationGroup::getAssociativeVariables(const std::string &string) {
    return associations[string];
}
