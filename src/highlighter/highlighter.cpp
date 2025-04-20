//
// Created by Artur Twardzik on 23/03/2025.
//

#include "highlighter.h"

void Highlighter::addHighlightingRule(const QTextCharFormat &format, const QString &pattern) {
        const HighlightingRule rule{
                .pattern = QRegularExpression(QString(pattern)),
                .format = format,
        };

        highlightingRules.append(rule);
}


void Highlighter::matchRule(const HighlightingRule &rule, const QString &text) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
}

void Highlighter::matchRules(const QVector<HighlightingRule> &rules, const QString &text) {
        for (const HighlightingRule &rule: std::as_const(rules)) {
                matchRule(rule, text);
        }
}


void Highlighter::highlightSyntax(const QString &text) {
        matchRules(highlightingRules, text);
}

void Highlighter::highlightComments(const QString &text) {
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
                        commentLength = endIndex - startIndex + match.capturedLength();
                }
                setFormat(startIndex, commentLength, multiLineCommentFormat);
                startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
}

void Highlighter::highlightBlock(const QString &text) {
        highlightSyntax(text);

        setCurrentBlockState(0);

        highlightComments(text);
}
