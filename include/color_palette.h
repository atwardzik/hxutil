//
// Created by Artur Twardzik on 24/03/2025.
//

#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QBitmap>

namespace LightTheme {
        const QColor backgroundColor = QColor(Qt::white);
        const QColor lineHighlight = QColor("#ABB2BF").lighter(130);
}

namespace DarkTheme {
        const QColor backgroundColor = QColor("#282C34");
        const QColor lineHighlight = QColor("#31353E");
}

namespace OneDarkTheme {
        const QColor darkGray = QColor("#282C34");
        const QColor lightGray = QColor("#ABB2BF");
        const QColor red = QColor("#E06C75");
        const QColor green = QColor("#98C379");
        const QColor yellow = QColor("#E5C07B");
        const QColor blue = QColor("#61AFEF");
        const QColor magenta = QColor("#C678DD");
        const QColor cyan = QColor("#56B6C2");
}

namespace UtilColors {
        const QColor iconColor = QColor("#6f9f55");
}

namespace Syntax {}

inline QIcon createColoredIcon(QString fileName, const QColor &color) {
        QPixmap pixmap(fileName);
        auto mask = pixmap.createMaskFromColor(Qt::black, Qt::MaskOutColor);
        pixmap.fill(color);
        pixmap.setMask(mask);

        return QIcon(pixmap);
}

#endif //PALETTE_H
