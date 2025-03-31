//
// Created by Artur Twardzik on 23/03/2025.
//

#ifndef HEX_MOD_H
#define HEX_MOD_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <exception>

//TODO: remove inlines, make the file cpp module

inline std::string bytesToStrRepr(std::vector<uint8_t> &hex_bytes) {
        std::stringstream out;

        size_t counter = 0;
        for (const auto &e: hex_bytes) {
                if (!((counter + 1) % 16)) {
                        out << "\n";
                }

                if (std::isprint(e)) {
                        out << e;
                }
                else {
                        out << ".";
                }

                counter += 1;
        }

        return out.str();
}

inline std::string bytesToPrintable(std::vector<uint8_t> &hex_bytes) {
        std::stringstream out;

        size_t counter = 0;
        for (const auto &e: hex_bytes) {
                if (counter && !(counter % 16)) {
                        out << std::endl;
                }
                else if (counter && !(counter % 8)) {
                        out << " ";
                }

                out << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(e);
                out << " ";

                counter += 1;
        }

        return out.str();
}


inline std::string readPlainText(const std::string &filename, std::ios_base::openmode mode = std::ios::in) {
        std::ifstream file(filename, mode);

        if (!file.is_open()) {
                throw std::runtime_error(std::format("File: {} could not be opened.", filename));
        }

        std::string plainText;
        if (mode == std::ios::in) {
                std::string line;
                while (std::getline(file, line)) {
                        plainText += line + "\n";
                }
        }
        else if (mode == (std::ios::in | std::ios::binary)) {
                std::vector<uint8_t> bytes(
                        (std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));

                plainText = bytesToPrintable(bytes);
        }

        file.close();

        return plainText;
}

#endif //HEX_MOD_H
