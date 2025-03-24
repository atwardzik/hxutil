//
// Created by Artur Twardzik on 24/03/2025.
//

#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>

namespace LightTheme {
        const QColor backgroundColor = QColor(Qt::white);
        const QColor lineHighlight = QColor("#ABB2BF").lighter(130);
}

namespace DarkTheme {
        const QColor backgroundColor = QColor("#282C34");
        const QColor lineHighlight = QColor("#31353E");
}

namespace Syntax {}

#endif //PALETTE_H
