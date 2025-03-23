//
// Created by Artur Twardzik on 23/03/2025.
//

module;

#include <fstream>
#include <vector>
#include <cstdint>

export module binary_file_handler;

export std::vector<uint8_t> open_binary(const std::string &filename) {
        std::ifstream file;

        file.open(filename, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
                throw std::ios_base::failure("Problem opening the file.");
        }

        std::vector<uint8_t> bytes(
                (std::istreambuf_iterator<char>(file)),
                (std::istreambuf_iterator<char>()));

        file.close();

        return bytes;
}
