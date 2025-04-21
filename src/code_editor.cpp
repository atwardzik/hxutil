//
// Created by Artur Twardzik on 24/03/2025.
//

#include "code_editor.h"
#include "ASMstatic.h"
#include "settings.h"


CodeEditor::CodeEditor(QWidget *parent, const QString &fileName, Language language) : QPlainTextEdit(parent) {
        lineNumberArea = new LineNumberArea(this);
        this->fileName = fileName;
        this->language = language;

        this->setLineWrapMode(QPlainTextEdit::NoWrap);


        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

        this->setFont(QFont("JetBrains Mono", 13, QFont::Normal));
        changePalette();

        updateLineNumberAreaWidth(0);
        highlightCurrentLine();
        setHighlighter();

        connect(this->document(), &QTextDocument::contentsChanged, [this] {
                if (!this->fileName.isEmpty()) {
                        saveFile();
                }
        });
}

void CodeEditor::setLanguage(const Language language) {
        this->language = language;
}

void CodeEditor::setHighlighter() {
        switch (language) {
                        using enum Language;
                case ARMv6_ASM:
                        highlighter = new ARMv6_ASM_Highlighter(this->document());
                        break;
                case x86_ASM:
                        break;
                case C:
                        highlighter = new C_Highlighter(this->document(), fileName);
                        break;
                case HEX:
                case None:
                default:
                        break;
        }
}

void CodeEditor::formatCode() {
        QString plainText = this->document()->toPlainText();
        if (language == Language::ARMv6_ASM) {
                std::unique_ptr<char> formatted_text(format_arm_asm_code(
                        plainText.toStdString().c_str(), plainText.length()));
                std::string formatted_text_str = formatted_text.get();

                this->document()->setPlainText(QString::fromStdString(formatted_text_str));
        }
        else if (language == Language::C) {
                const QString command = getSetting(settings, "clang-formatPath");

                QStringList params;
                params << this->fileName << "--cursor" << QString::number(this->textCursor().position());

                QProcess formatter;
                formatter.start(command, params);
                formatter.waitForFinished();
                QString output(formatter.readAllStandardOutput());
                QString errors(formatter.readAllStandardError());
                output += errors;

                int newlinePos = output.indexOf('\n');
                QString json = output.sliced(0, newlinePos);
                output.remove(0, newlinePos + 1);

                this->document()->setPlainText(output);


                QJsonDocument cursorData = QJsonDocument::fromJson(json.toUtf8());
                if (cursorData.isObject()) {
                        QJsonObject obj = cursorData.object();
                        QTextCursor cursor = this->textCursor();
                        cursor.setPosition(obj.value("Cursor").toInt());
                        this->setTextCursor(cursor);
                }
        }
}

void CodeEditor::changePalette() {
        QColor backgroundColor;

        if (QStyleHints const *style = QGuiApplication::styleHints();
                style->colorScheme() == Qt::ColorScheme::Dark) {
                backgroundColor = DarkTheme::backgroundColor;
                lineColor = DarkTheme::lineHighlight;
        }
        else {
                backgroundColor = LightTheme::backgroundColor;
                lineColor = LightTheme::lineHighlight;
        }

        QPalette palette = this->palette();
        palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
        palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
        this->setPalette(palette);
        this->setBackgroundVisible(false);

        highlightCurrentLine();
}

QString CodeEditor::getFileExtension() const {
        switch (language) {
                        using enum Language;
                case ARMv6_ASM:
                        return ".s";
                case x86_ASM:
                        return ".asm";
                case C:
                        return ".c";
                case HEX:
                        return ".hex";
                case None:
                default:
                        return "";
        }
}

QString CodeEditor::getObjectFileName() {
        QString currentExtension = getFileExtension();
        std::string objectFileName = fileName.toStdString();

        int fileExtensionPos = objectFileName.rfind(currentExtension.toStdString());
        objectFileName.erase(objectFileName.begin() + fileExtensionPos, objectFileName.end());

        objectFileName += ".o";

        return QString::fromStdString(objectFileName);
}

void CodeEditor::determineFileName() {
        if (!fileName.isEmpty()) {
                return;
        }

        QDialog *dialog = new QDialog();
        dialog->setWindowTitle("Save file");

        QFormLayout *form = new QFormLayout;
        QLineEdit *input = new QLineEdit;
        form->addRow(tr("File name:"), input);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        form->addRow(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        dialog->setLayout(form);

        int dialogCode = dialog->exec();

        if (dialogCode == QDialog::Accepted) {
                fileName = input->text();

                if (!fileName.contains('.')) {
                        fileName += getFileExtension();
                }
        }
}

//!
//! @return file name of a saved file
QString CodeEditor::saveFile() {
        determineFileName();

        if (!fileName.isEmpty()) {
                savePlaintextFile(fileName.toStdString(), this->toPlainText().toStdString());
        }

        return fileName;
}

int CodeEditor::lineNumberAreaWidth() {
        int digits = 1;
        int max = qMax(1, blockCount());
        while (max >= 10) {
                max /= 10;
                ++digits;
        }

        int space = fontMetrics().averageCharWidth() * 2 +     //spaces around
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
                        painter.setPen(OneDarkTheme::lightGray);
                        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                         Qt::AlignRight, number);
                }

                block = block.next();
                top = bottom;
                bottom = top + static_cast<int>(blockBoundingRect(block).height());
                ++blockNumber;
        }
}
