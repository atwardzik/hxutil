//
// Created by Artur Twardzik on 01/04/2025.
//

#ifndef LANUGAGE_SPECIFICS_H
#define LANUGAGE_SPECIFICS_H

#include <QString>
#include <QFileInfo>

enum class Language {
        Generic_ASM,
        ARMv6_ASM,
        x86_ASM,
        C,
        HEX,
        None,
};

inline Language detectAssemblyDialect(const QString &fileName) {
        Language assembly_dialect = Language::Generic_ASM;

        if (const QString fileExtension = QFileInfo(fileName).suffix();
                fileExtension == "s" || fileExtension == "S") {
                assembly_dialect = Language::ARMv6_ASM;
        }
        else if (fileExtension == "asm") {
                assembly_dialect = Language::x86_ASM;
        }

        return assembly_dialect;
}

// TODO: add keywords, directives, etc

#endif //LANUGAGE_SPECIFICS_H
