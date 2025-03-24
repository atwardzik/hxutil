//
// Created by Artur Twardzik on 24/03/2025.
//

#include "code_editor.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
        lineNumberArea = new LineNumberArea(this);

        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

        changePalette();

        updateLineNumberAreaWidth(0);
        highlightCurrentLine();
}

void CodeEditor::changePalette() {
        QColor backgroundColor;

        if (QStyleHints const *style = QGuiApplication::styleHints();
                style->colorScheme() == Qt::ColorScheme::Dark) {
                lineColor = QColor("#31353E");
                backgroundColor = QColor("#282C34");
        }
        else {
                lineColor = QColor("#ABB2BF").lighter(130);
                backgroundColor = QColor(Qt::white);
        }

        QPalette palette = this->palette();
        palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
        palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
        this->setPalette(palette);
        this->setBackgroundVisible(false);

        highlightCurrentLine();
}


unsigned int CodeEditor::lineNumberAreaWidth() {
        unsigned int digits = 1;
        unsigned int max = qMax(1, blockCount());
        while (max >= 10) {
                max /= 10;
                ++digits;
        }

        unsigned int space = fontMetrics().averageCharWidth() * 2 + //spaces around
                             fontMetrics().averageCharWidth() * digits; //numbers

        return space;
}


void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
        if (dy) {
                lineNumberArea->scroll(0, dy);
        }
        else {
                lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
        }

        if (rect.contains(viewport()->rect())) {
                updateLineNumberAreaWidth(0);
        }
}


void CodeEditor::resizeEvent(QResizeEvent *e) {
        QPlainTextEdit::resizeEvent(e);

        QRect cr = contentsRect();
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::changeEvent(QEvent *event) {
        if (event->type() == QEvent::ThemeChange ||
            event->type() == QEvent::StyleChange ||
            event->type() == QEvent::PaletteChange) {
                changePalette();
        }

        QPlainTextEdit::changeEvent(event);
}


void CodeEditor::highlightCurrentLine() {
        QList<QTextEdit::ExtraSelection> extraSelections;

        if (!isReadOnly()) {
                QTextEdit::ExtraSelection selection;

                selection.format.setBackground(lineColor);
                selection.format.setProperty(QTextFormat::FullWidthSelection, true);
                selection.cursor = textCursor();
                selection.cursor.clearSelection();
                extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);
}


void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
        QPainter painter(lineNumberArea);
        QFont font = painter.font();
        font.setWeight(QFont::Light);
        painter.setFont(font);
        painter.fillRect(event->rect(), QWidget().palette().window());

        QTextBlock block = this->firstVisibleBlock();
        int blockNumber = block.blockNumber();
        auto top = static_cast<int>(this->blockBoundingGeometry(block).translated(this->contentOffset()).top());
        int bottom = top + static_cast<int>(blockBoundingRect(block).height());

        while (block.isValid() && top <= event->rect().bottom()) {
                if (block.isVisible() && bottom >= event->rect().top()) {
                        QString number = " " + QString::number(blockNumber + 1) + " ";
                        painter.setPen(QColor("#ABB2BF"));
                        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                         Qt::AlignRight, number);
                }

                block = block.next();
                top = bottom;
                bottom = top + static_cast<int>(blockBoundingRect(block).height());
                ++blockNumber;
        }
}
