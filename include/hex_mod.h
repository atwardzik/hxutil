//
// Created by Artur Twardzik on 23/03/2025.
//

#ifndef HEX_MOD_H
#define HEX_MOD_H

#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

inline std::string bytes_to_str_repr(std::vector<uint8_t> &hex_bytes) {
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

inline std::string bytes_to_printable(std::vector<uint8_t> &hex_bytes) {
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

#endif //HEX_MOD_H
