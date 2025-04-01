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
#include <QFileDialog>

#include "lanugage_specifics.h"

struct OpenFile {
        QString fileName;
        QString plaintext;
};

std::string bytesToStrRepr(std::vector<uint8_t> &hex_bytes);

std::string bytesToPrintable(std::vector<uint8_t> &hex_bytes);

std::string readPlainText(const std::string &filename, std::ios_base::openmode mode = std::ios::in);

OpenFile openFileGetPlaintext(Language language);

#endif //HEX_MOD_H
