//
// Created by Artur Twardzik on 01/04/2025.
//

#include "file_modifier.h"

std::string bytesToStrRepr(std::vector<uint8_t> &hex_bytes) {
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

std::string bytesToPrintable(std::vector<uint8_t> &hex_bytes) {
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


std::string readPlainText(const std::string &filename, std::ios_base::openmode mode) {
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

static QString getFileFilter(Language language) {
        QString filter;

        switch (language) {
                        using enum Language;
                case Generic_ASM:
                        filter = "Assembly files (*.s *.S *.asm)";
                        break;
                case HEX:
                        filter = "Object files (*.o *.obj *.elf *.so *.out)";
                        break;
                default:
                        throw std::runtime_error("Language not supported");
        }

        return filter;
}

OpenFile openFileGetPlaintext(Language language) {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Open the file",
                                                        QDir::homePath(), getFileFilter(language));
        if (fileName.isEmpty()) {
                return {};
        }

        QString text = "";

        switch (language) {
                        using enum Language;
                case Generic_ASM:
                case ARMv6_ASM:
                case x86_ASM:
                case C:
                        text = QString::fromStdString(readPlainText(fileName.toStdString()));
                        break;
                case HEX:
                        text = QString::fromStdString(
                                readPlainText(fileName.toStdString(), std::ios::in | std::ios::binary));
                        break;
                case None:
                default:
                        break;
        }

        return {
                fileName,
                text
        };
}

void savePlaintextFile(const std::string &filename, const std::string &data) {
        std::ofstream file(filename);
        file << data;
        file.close();
}
