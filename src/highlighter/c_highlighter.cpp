//
// Created by Artur Twardzik on 19/04/2025.
//

#include "color_palette.h"
#include "highlighter.h"
#include "settings.h"

#include <QProcess>
#include <QtConcurrent>
#include <QThread>
#include <thread>

C_Highlighter::C_Highlighter(QTextDocument *parent, QString filename) : Highlighter(parent, filename) {
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
                "\\benum\\b", "\\bsigned\\b", "\\bwhile\\b", "\\b__attribute__\\b"
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

void C_Highlighter::readTokens() {
        const QString command = getSetting(settings, "clangPath");
        const QString includePath = getSetting(settings, "IncludePath");

        QStringList params;
        params << "-I" << includePath
                        << "-fsyntax-only"
                        << "-Xclang"
                        << "-dump-tokens"
                        << fileName;

        QProcess clangIdentifiers;
        clangIdentifiers.start(command, params);
        clangIdentifiers.waitForFinished();
        QString output(clangIdentifiers.readAllStandardOutput());
        QString errors(clangIdentifiers.readAllStandardError());
        output += errors;

        tokenWriteMutex.lock();
        tokens.clear();
        tokens = output.split("\n");
        tokenWriteMutex.unlock();
}

void C_Highlighter::highlightUserDefinedTypes(const QString &text) {
        //yellow
}

// static bool is_std_type(const QString &token) {
//         const QString types[] = {
//                 "int", "char", "bool", "float", "double", "long", "short", "auto", "signed", "unsigned"
//         };
//
//         for (int i = 0; i < 10; ++i) {
//                 if (token.startsWith(types[i])) {
//                         return true;
//                 }
//         }
//
//         return false;
// }

void C_Highlighter::highlightGlobalIdentifiers(const QString &text) {
        /*
        int 'int'	 [StartOfLine]	Loc=<lib.h:8:1>
        identifier 'global'	 [LeadingSpace]	Loc=<lib.h:8:5>
        equal '='	 [LeadingSpace]	Loc=<lib.h:8:12>
        numeric_constant '10'	 [LeadingSpace]	Loc=<lib.h:8:14>
        semi ';'		Loc=<lib.h:8:16>
         */

        // if identifier and previous in_types or previous is ptr
        // else if identifier and previous struct and previous not typedef
        //      -> while not (bracket and next *ident* and next __attribute or semi)

        QTextCharFormat format;
        format.setForeground(OneDarkTheme::red);

        int scope = 0;
        auto it = tokens.begin();

        while (it++ != tokens.end()) {
                // QStringList line = it->split(QRegularExpression("\\s"));
                // if (it->startsWith("identifier")) {
                //         if ((it + 1)->startsWith("l_paren")) {
                //                 continue;
                //         }
                //
                //         if (is_std_type(*(it - 1))) {
                //                 globalIdentifiersRules.push_back({});
                //         }
                //         else if ((it - 1)->startsWith("star") && is_std_type(*(it - 2))) {
                //
                //         }
                //         else if ((it - 1)->startsWith("star") && (it - 2)->startsWith("identifier")) {
                //
                //         }
                //         else if ((it - 1)->startsWith("struct") && !(it - 2)->startsWith("typedef")) {
                //
                //         }
                // }
        }

        scope = false;

        if (scope) {}
}

void C_Highlighter::highlightOther(const QString &text) {
        readTokens();

        // auto detectTypes = std::thread(&C_Highlighter::highlightUserDefinedTypes, this, text);
        // auto detectGlobals = std::thread(&C_Highlighter::highlightGlobalIdentifiers, this, text);

        // detectTypes.join();
        // detectGlobals.join();
}


void C_Highlighter::highlightBlock(const QString &text) {
        QRegularExpressionMatchIterator matchIterator = functionRule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength() - 1, functionRule.format);
        }


        Highlighter::highlightBlock(text);


        QFuture<void> highlightOtherFuture = QtConcurrent::run([this, text]() {
                highlightOther(text);
        });
        // detectIdentifiers();
        // matchRules(globalIdentifiersRules, text);
        // matchRules(userDefinedTypesRules, text);
}
