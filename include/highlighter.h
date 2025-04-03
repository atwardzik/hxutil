//
// Created by Artur Twardzik on 23/03/2025.
//

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

class Highlighter : public QSyntaxHighlighter {
        Q_OBJECT

        struct HighlightingRule {
                QRegularExpression pattern;
                QTextCharFormat format;
        };

        QVector<HighlightingRule> highlightingRules;

protected:
        explicit Highlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent) {}

        void highlightBlock(const QString &text) override;

        virtual void setupCodeHighlights() = 0;

        void addHighlightingRule(const QTextCharFormat &format, const QString &pattern);

        void highlightSyntax(const QString &text);

        void highlightComments(const QString &text);

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;
        QTextCharFormat multiLineCommentFormat;
};

class ARMv6_ASM_Highlighter final : public Highlighter {
        QTextCharFormat keywordFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat identifierFormat;
        QTextCharFormat directiveFormat;

        void setupCodeHighlights() override;

public:
        explicit ARMv6_ASM_Highlighter(QTextDocument *parent = nullptr);
};

#endif //HIGHLIGHTER_H
