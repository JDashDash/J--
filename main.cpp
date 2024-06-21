#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <utility>
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

bool endsWith(std::string_view str, std::string_view suffix) {
    if (str.length() >= suffix.length()) {
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }
    return false;
}

std::string toLower(std::string str) {
    std::string result = std::move(str);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}

int main(int argc, char** argv) {
    std::string file_name;
    if (argc == 2) {
        file_name = argv[1];
    } else {
        std::cout << "File: ";
        std::cin >> file_name;
        std::cout << std::endl;
    }

    if (!endsWith(toLower(file_name), toLower(".jdd"))) {
        std::cerr << "Error: The file must have a .jdd extension." << std::endl;
        return -1;
    }

    std::ifstream file{file_name};
    if (!file) {
        std::cerr << "Error: Unable to open the specified file." << std::endl;
        return -1;
    }

    std::string code((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

    auto lexerResult = JDD::Lexer::Lexer(code);
    //for (auto const& t : lexerResult) {std::cout << t << std::endl;}
    JDD::Parser::run(lexerResult);
    return 0;
}