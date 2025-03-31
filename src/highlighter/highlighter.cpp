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


void Highlighter::highlightBlock(const QString &text) {
        for (const HighlightingRule &rule: std::as_const(highlightingRules)) {
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
