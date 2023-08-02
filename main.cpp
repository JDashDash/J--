#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    std::string file_name;
    if (argc == 2) {
        file_name = argv[1];
    } else {
        std::cout << "File: ";
        std::cin >> file_name;
        std::cout << std::endl;
    }

    std::ifstream file {file_name};
    if (!file) {
        std::cerr << "Specify the file you want to use" << std::endl;
        return -1;
    }

    return 0;
}
