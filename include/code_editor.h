//
// Created by Artur Twardzik on 24/03/2025.
//

#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <QGuiApplication>
#include <QStyleHints>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QApplication>
#include <QFont>
#include <QLabel>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QString>
#include <QFormLayout>
#include <QPushButton>

#include "color_palette.h"
#include "lanugage_specifics.h"
#include "highlighter.h"
#include "file_modifier.h"

// todo: https://doc.qt.io/qt-6/qtwidgets-tools-customcompleter-example.html

class CodeEditor : public QPlainTextEdit {
        Q_OBJECT

public:
        explicit CodeEditor(QWidget *parent = nullptr, const QString &fileName = "", Language language = Language::None
        );

        QString saveFile();

        void setLanguage(Language language);

        QString getObjectFileName();

        void formatCode();

        void lineNumberAreaPaintEvent(QPaintEvent *event);

        int lineNumberAreaWidth();

protected:
        void resizeEvent(QResizeEvent *event) final;

        void changeEvent(QEvent *) final;

private slots:
        void updateLineNumberAreaWidth(int newBlockCount);

        void highlightCurrentLine();

        void updateLineNumberArea(const QRect &, int);

private:
        void changePalette();

        void setHighlighter() const;

        QString getFileExtension() const;

        void determineFileName();

private:
        QWidget *lineNumberArea;

        QColor lineColor;

        QString fileName;

        Language language;
};


class LineNumberArea : public QWidget {
public:
        explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

        QSize sizeHint() const final {
                return {codeEditor->lineNumberAreaWidth(), 0};
        }

protected:
        void paintEvent(QPaintEvent *event) final {
                codeEditor->lineNumberAreaPaintEvent(event);
        }

private:
        CodeEditor *codeEditor;
};

#endif //CODE_EDITOR_H
