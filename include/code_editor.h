//
// Created by Artur Twardzik on 24/03/2025.
//

#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QApplication>
#include <QFont>

class CodeEditor : public QPlainTextEdit {
        Q_OBJECT

public:
        CodeEditor(QWidget *parent = nullptr);

        void lineNumberAreaPaintEvent(QPaintEvent *event);

        unsigned int lineNumberAreaWidth();

protected:
        void resizeEvent(QResizeEvent *event);

private slots:
        void updateLineNumberAreaWidth(int newBlockCount);

        void highlightCurrentLine();

        void updateLineNumberArea(const QRect &, int);

private:
        QWidget *lineNumberArea;
};


class LineNumberArea : public QWidget {
public:
        explicit LineNumberArea(CodeEditor *editor) : QWidget(editor) {
                codeEditor = editor;
        }

        QSize sizeHint() const {
                return QSize(codeEditor->lineNumberAreaWidth(), 0);
        }

protected:
        void paintEvent(QPaintEvent *event) {
                codeEditor->lineNumberAreaPaintEvent(event);
        }

private:
        CodeEditor *codeEditor;
};

#endif //CODE_EDITOR_H
