//
// Created by Artur Twardzik on 23/03/2025.
//

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

class Highlighter : public QSyntaxHighlighter {
        Q_OBJECT

public:
        Highlighter(QTextDocument *parent = 0);

protected:
        void highlightBlock(const QString &text) override;

private:
        struct HighlightingRule {
                QRegularExpression pattern;
                QTextCharFormat format;
        };

        QVector<HighlightingRule> highlightingRules;

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;

        QTextCharFormat keywordFormat;
        QTextCharFormat classFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat multiLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat functionFormat;
};

#endif //HIGHLIGHTER_H
