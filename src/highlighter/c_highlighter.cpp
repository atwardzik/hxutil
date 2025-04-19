//
// Created by Artur Twardzik on 19/04/2025.
//

#include "color_palette.h"
#include "highlighter.h"

C_Highlighter::C_Highlighter(QTextDocument *parent) : Highlighter(parent) {
        setupCodeHighlights();
}

void C_Highlighter::setupCodeHighlights() {
        const QString keywordPatterns[] = {
                "\\balignas\\b", "\\bextern\\b", "\\bsizeof\\b", "\\b_Alignas\\b",
                "\\balignof\\b", "\\bfalse\\b", "\\bstatic\\b", "\\b_Alignof\\b",
                "\\bauto\\b", "\\bfloat\\b", "\\bstatic_assert\\b", "\\b_Atomic\\b",
                "\\bbool\\b", "\\bfor\\b", "\\bstruct\\b", "\\b_BitInt\\b",
                "\\bbreak\\b", "\\bgoto\\b", "\\bswitch\\b", "\\b_Bool\\b",
                "\\bcase\\b", "\\bif\\b", "\\bthread_local\\b", "\\b_Complex\\b",
                "\\bchar\\b", "\\binline\\b", "\\btrue\\b", "\\b_Decimal128\\b",
                "\\bconst\\b", "\\bint\\b", "\\btypedef\\b", "\\b_Decimal32\\b",
                "\\bconstexpr\\b", "\\blong\\b", "\\btypeof\\b", "\\b_Decimal64\\b",
                "\\bcontinue\\b", "\\bnullptr\\b", "\\btypeof_unqual\\b", "\\b_Generic\\b",
                "\\bdefault\\b", "\\bregister\\b", "\\bunion\\b", "\\b_Imaginary\\b",
                "\\bdo\\b", "\\brestrict\\b", "\\bunsigned\\b", "\\b_Noreturn\\b",
                "\\bdouble\\b", "\\breturn\\b", "\\bvoid\\b", "\\b_Static_assert\\b",
                "\\belse\\b", "\\bshort\\b", "\\bvolatile\\b", "\\b_Thread_local\\b",
                "\\benum\\b", "\\bsigned\\b", "\\bwhile\\b"
        };
        keywordFormat.setForeground(OneDarkTheme::magenta);
        keywordFormat.setFontItalic(true);
        for (const QString &pattern: keywordPatterns) {
                addHighlightingRule(keywordFormat, pattern);
        }

        const QString preprocessorPatterns[] = {
                "(?<!\\w)#if(?!\\w)", "(?<!\\w)#ifdef(?!\\w)", "(?<!\\w)#include(?!\\w)",
                "(?<!\\w)#defined(?!\\w)", "(?<!\\w)#elif(?!\\w)", "(?<!\\w)#ifndef(?!\\w)",
                "(?<!\\w)#embed(?!\\w)", "(?<!\\w)__has_include(?!\\w)", "(?<!\\w)#else(?!\\w)",
                "(?<!\\w)#elifdef(?!\\w)", "(?<!\\w)#line(?!\\w)", "(?<!\\w)__has_embed(?!\\w)",
                "(?<!\\w)#endif(?!\\w)", "(?<!\\w)#elifndef(?!\\w)", "(?<!\\w)#error(?!\\w)",
                "(?<!\\w)__has_c_attribute(?!\\w)", "(?<!\\w)#define(?!\\w)", "(?<!\\w)#warning(?!\\w)",
                "(?<!\\w)#undef(?!\\w)", "(?<!\\w)#pragma(?!\\w)",
        };
        preprocessorFormat.setForeground(OneDarkTheme::magenta);
        preprocessorFormat.setFontItalic(true);
        for (const QString &pattern: preprocessorPatterns) {
                addHighlightingRule(preprocessorFormat, pattern);
        }

        quotationFormat.setForeground(OneDarkTheme::green);
        addHighlightingRule(quotationFormat, "\".*\"");

        numberFormat.setForeground(OneDarkTheme::yellow);
        addHighlightingRule(numberFormat, "((0x)|(0b)|(0o)|(0d))?([0-9']*)");

        escapeSequenceFormat.setForeground(OneDarkTheme::blue);
        addHighlightingRule(escapeSequenceFormat, "(\\\\[abefnrtv\\\\\'\"\?])|(%[diuoxXfFeEgGaAcspn%])");


        functionRule.format.setForeground(OneDarkTheme::blue);
        functionRule.pattern = QRegularExpression("[_\\w][A-Za-z0-9_]*\\(");

        singleLineCommentFormat.setForeground(OneDarkTheme::lightGray);
        singleLineCommentFormat.setFontItalic(true);
        addHighlightingRule(singleLineCommentFormat, "(//[^\n]*)");

        multiLineCommentFormat.setForeground(OneDarkTheme::lightGray);
        multiLineCommentFormat.setFontItalic(true);
        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void C_Highlighter::highlightBlock(const QString &text) {
        QRegularExpressionMatchIterator matchIterator = functionRule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength() - 1, functionRule.format);
        }


        Highlighter::highlightBlock(text);
}
