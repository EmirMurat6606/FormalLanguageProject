//
// Created by 2005e on 25/10/2024.
//

#include "../Headers/Logger.h"
#include "../Headers/CFG.h"

void Logger::printTable(ParseTable &table) {
    std::vector<int> table_sizes;
    int max_size = 0;
    for (const auto &variable: table.variables) {
        if (variable.size() > max_size) {
            max_size = static_cast<int>(variable.size());
        }
    }
    table_sizes.push_back(max_size);
    for (const auto &symbol: table.symbols) {
        max_size = symbol.size();
        for (auto &column: table.Table) {
            int size = column.second[symbol].size();
            if (column.second[symbol] == " ") {
                size = 0;
            }
            if (size > max_size) {
                max_size = column.second[symbol].size();
            }
        }
        table_sizes.push_back(max_size);
    }
    std::cout << generateSpaces(table_sizes[0] + 4, false);
    int counter = 1;
    for (const auto &symbol: table.symbols) {
        std::cout << "| " << symbol << generateSpaces(table_sizes[counter] - symbol.size(), false) << "  ";
        counter++;
    }
    std::cout << "|" << std::endl;
    for (int size: table_sizes) {
        std::cout << "|" << generateSpaces(size + 3, true);
    }
    std::cout << "|" << std::endl;
    for (const auto &row: table.Table) {
        std::cout << "| " << row.first << generateSpaces(table_sizes[0] - row.first.size(), false) << "  ";
        counter = 1;
        for (const auto &column: row.second) {
            std::string copy = column.second;
            if (column.second == " ")
                copy = "";
            std::cout << "| " << copy << generateSpaces(table_sizes[counter] - copy.size(), false) << "  ";
            counter++;
        }
        std::cout << "|" << std::endl;
    }
    for (int size: table_sizes) {
        std::cout << "|" << generateSpaces(size + 3, true);
    }
    std::cout << "|" << std::endl;
}

std::string Logger::setToString(const StringSet &string_set) {
    std::string output = "{";
    for (auto it = string_set.begin(); it != string_set.end(); it++) {
        if (it != string_set.begin())
            output.append(", ");
        if (*it == " ")
            output.append("");
        else
            output.append(*it);
    }
    output.append("}");
    return output;
}

std::vector<std::string> Logger::splitString(const std::string &str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    while (iss >> token) {
        if (token == " ") {
            result.emplace_back("");
        } else {
            token.erase(std::remove(token.begin(), token.end(), '`'), token.end());
            result.push_back(token);
        }
    }
    if (result.empty())
        result.emplace_back("");
    return result;
}

std::string Logger::generateSpaces(int count, bool underlining) {
    std::string spaces;
    if (underlining)
        for (int i = 0; i < count; ++i)
            spaces += "-";
    else
        for (int i = 0; i < count; ++i)
            spaces += " ";
    return std::move(spaces);
}

void Logger::printAcceptanceTable(const std::vector<std::vector<TableObject>> &table) {
    std::vector<int> table_sizes;
    table_sizes.reserve(table.size());

    for (int j = 0; j < table.size(); ++j) {
        int max_setsize = 0;
        for (int i = table.size() - 1 - j; i >= 0; --i) {
            int set_size = setToString(table[i][j].getVariables()).size();
            if (set_size > max_setsize) {
                max_setsize = set_size;
            }
        }
        table_sizes.push_back(max_setsize);
    }
    for (int i = table.size() - 1; i >= 0; --i) {
        std::cout << "|";
        for (int j = 0; j < table.size() - i; ++j) {
            std::string set = setToString(table[i][j].getVariables());
            int extra_spaces = table_sizes[j] - set.size();

            std::string spaces;
            for (int counter = 0; counter < extra_spaces; ++counter) {
                spaces += " ";
            }
            std::cout << " " + setToString(table[i][j].getVariables()) + spaces + "  |";
        }
        std::cout << "\n";
    }
}

void Logger::printProductions(const std::map<std::string, StringSet> &productions) {
    std::cout << "P = {" << std::endl;
    std::string align_space = "    ";
    for (const auto &body: productions) {
        for (auto it = body.second.begin(); it != body.second.end(); ++it) {
            std::cout << align_space << body.first << " -> `" << *it << "`" << std::endl;
        }
    }
    std::cout << "}" << std::endl;
}

void Logger::printJSON(const ParseTable &table) {
    json j;
    j["first"] = table.firstSet;
    j["follow"] = table.followSet;
    j["ll1_table"] = table.Table;
    std::cout << std::setw(4) << j << std::endl;
}
