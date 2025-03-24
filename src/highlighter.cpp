//
// Created by Artur Twardzik on 23/03/2025.
//

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
        HighlightingRule rule;

        keywordFormat.setForeground(QBrush(QColor("#C678DD")));
        keywordFormat.setFontItalic(true);
        const QString keywordPatterns[] = {
                QStringLiteral("\\bmovs\\b"), QStringLiteral("\\bldr\\b"), QStringLiteral("\\bsvc\\b"),
                QStringLiteral("\\bmov\\b"), QStringLiteral("\\benum\\b"), QStringLiteral("\\bexplicit\\b"),
        };
        for (const QString &pattern: keywordPatterns) {
                rule.pattern = QRegularExpression(pattern);
                rule.format = keywordFormat;
                highlightingRules.append(rule);
        }

        classFormat.setFontWeight(QFont::Bold);
        classFormat.setForeground(Qt::darkMagenta);
        rule.pattern = QRegularExpression(QStringLiteral("\\bQ[A-Za-z]+\\b"));
        rule.format = classFormat;
        highlightingRules.append(rule);

        quotationFormat.setForeground(QBrush(QColor("#98C379")));
        rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        numberFormat.setForeground(QBrush(QColor("#56B6C2")));
        rule.pattern = QRegularExpression(QStringLiteral("#[0-9A-Fa-fx]*"));
        rule.format = numberFormat;
        highlightingRules.append(rule);

        identifierFormat.setForeground(QBrush(QColor("#61AFEF")));
        rule.pattern = QRegularExpression(QStringLiteral("[^\n]*:"));
        rule.format = identifierFormat;
        highlightingRules.append(rule);

        directiveFormat.setForeground(QBrush(QColor("#E5C07B")));
        rule.pattern = QRegularExpression(QStringLiteral("\\.([^\n\\s]+)(\\s|\n)"));
        rule.format = directiveFormat;
        highlightingRules.append(rule);

        functionFormat.setFontItalic(true);
        functionFormat.setForeground(Qt::blue);
        rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        rule.format = functionFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(QBrush(QColor("#ABB2BF")));
        rule.pattern = QRegularExpression(QStringLiteral("(//[^\n]*)|(;[^\n]*)|(@[^\n]*)"));
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(QBrush(QColor("#ABB2BF")));

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::highlightBlock(const QString &text) {
        for (const HighlightingRule &rule: qAsConst(highlightingRules)) {
                QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
                while (matchIterator.hasNext()) {
                        QRegularExpressionMatch match = matchIterator.next();
                        setFormat(match.capturedStart(), match.capturedLength(), rule.format);
                }
        }

        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1) {
                startIndex = text.indexOf(commentStartExpression);
        }

        while (startIndex >= 0) {
                QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
                int endIndex = match.capturedStart();
                int commentLength = 0;
                if (endIndex == -1) {
                        setCurrentBlockState(1);
                        commentLength = text.length() - startIndex;
                }
                else {
                        commentLength = endIndex - startIndex
                                        + match.capturedLength();
                }
                setFormat(startIndex, commentLength, multiLineCommentFormat);
                startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
}
