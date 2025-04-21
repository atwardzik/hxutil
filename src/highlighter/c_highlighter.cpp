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

#include "ASMstatic.h"

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
        const QString compilerCommand = getSetting(settings, "cCompilerPath");
        const QString includePath = getSetting(settings, "IncludePath");
        const QString ctagsPath = getSetting(settings, "ctagsPath");

        QString shellCommand = QString(
                                "%1 -M -I %2 %3 | sed -e 's/[\\\\ ]/\\n/g' | sed -e '/^$/d' -e '/\\.o:[ \\t]*$/d' | %4 -L - --c-kinds=+p --fields=+iaS --extras=+q -x")
                        .arg(compilerCommand)
                        .arg(includePath)
                        .arg(fileName)
                        .arg(ctagsPath);

        QProcess getDependencies;
        getDependencies.start("bash", QStringList() << "-c" << shellCommand);
        getDependencies.waitForFinished();

        QString output = getDependencies.readAllStandardOutput();
        output += getDependencies.readAllStandardError();


        tagsMutex.lock();
        tags.clear();
        tags = output.split("\n");
        tagsMutex.unlock();
}

QList<QString> C_Highlighter::detectUserDefinedTypes() {
        //yellow
        return {};
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

QList<QString> C_Highlighter::detectGlobalIdentifiers() {
        QList<QString> globalIdentifiers;

        tagsMutex.lock();
        auto currentTags = tags;
        tagsMutex.unlock();

        auto tag = currentTags.begin();
        while (tag++ != currentTags.end()) {
                QStringList line = tag->split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

                if (line.size() < 2) {
                        continue;
                }

                if (line.at(1) == "variable") {
                        globalIdentifiers.push_back(line.at(0));
                }
        }


        return globalIdentifiers;
}

void C_Highlighter::highlightOther(const QString &text) {
        readTokens();

        // QFuture<QList<QString>> typesFuture = QtConcurrent::run([this, text] {
        //         return detectUserDefinedTypes(text);
        // });
        // QFutureWatcher<QList<QString>> *typesWatcher = new QFutureWatcher<QList<QString>>();
        //
        // connect(typesWatcher, &QFutureWatcher<QList<QString>>::finished, [=] {
        //         qDebug() << "finished";
        // });

        QFuture<QList<QString> > globalsFuture = QtConcurrent::run([this, text] {
                return detectGlobalIdentifiers();
        });

        globalsFuture.then([&](const QFuture<QList<QString> > &f) {
                try {
                        auto res = f.result<>();
                        if (!res.isEmpty()) {
                                rulesMutex.lock();

                                globalIdentifiersRules.clear();

                                QTextCharFormat globalsFormat;
                                globalsFormat.setForeground(OneDarkTheme::red);

                                for (const auto &identifier: res) {
                                        globalIdentifiersRules.push_back({
                                                QRegularExpression("(?<![a-zA-Z0-9_])" + identifier + "(?![a-zA-Z0-9_])"),
                                                globalsFormat
                                        });
                                }

                                rulesMutex.unlock();
                        }
                } catch (QException &e) {
                        qDebug() << "[!]";
                }
        });
}


void C_Highlighter::highlightBlock(const QString &text) {
        if (text.isEmpty()) {
                return;
        }

        QRegularExpressionMatchIterator matchIterator = functionRule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength() - 1, functionRule.format);
        }


        Highlighter::highlightBlock(text);


        QFuture<void> highlightOtherFuture = QtConcurrent::run([this, text] {
                highlightOther(text);
        });

        rulesMutex.lock();
        matchRules(globalIdentifiersRules, text);
        matchRules(userDefinedTypesRules, text);
        rulesMutex.unlock();
}
