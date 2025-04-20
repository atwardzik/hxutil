//
// Created by Artur Twardzik on 23/03/2025.
//

#include <QMutex>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QSettings>

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

class Highlighter : public QSyntaxHighlighter {
        Q_OBJECT

protected:
        struct HighlightingRule {
                QRegularExpression pattern;
                QTextCharFormat format;
        };

        explicit Highlighter(QTextDocument *parent = nullptr, QString fileName = "")
                : QSyntaxHighlighter(parent), fileName(fileName) {}

        void highlightBlock(const QString &text) override;

        void matchRule(const HighlightingRule &rule, const QString &text);

        void matchRules(const QVector<HighlightingRule> &rules, const QString &text);

        virtual void setupCodeHighlights() = 0;

        void addHighlightingRule(const QTextCharFormat &format, const QString &pattern);

        void highlightSyntax(const QString &text);

        void highlightComments(const QString &text);

        QRegularExpression commentStartExpression;
        QRegularExpression commentEndExpression;
        QTextCharFormat multiLineCommentFormat;

        QSettings settings;

        QString fileName;

private:
        QVector<HighlightingRule> highlightingRules;
};

class ARMv6_ASM_Highlighter final : public Highlighter {
        QTextCharFormat keywordFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat identifierFormat;
        QTextCharFormat directiveFormat;
        QTextCharFormat escapeSequenceFormat;

        void setupCodeHighlights() override;

public:
        explicit ARMv6_ASM_Highlighter(QTextDocument *parent = nullptr);
};

class C_Highlighter final : public Highlighter {
        QTextCharFormat keywordFormat;
        QTextCharFormat preprocessorFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat escapeSequenceFormat;

        HighlightingRule functionRule;
        QVector<HighlightingRule> globalIdentifiersRules;
        QVector<HighlightingRule> userDefinedTypesRules;
        QMutex tokenWriteMutex;
        QStringList tokens;

        void setupCodeHighlights() override;

        void readTokens();

        void highlightUserDefinedTypes(const QString &text);

        void highlightGlobalIdentifiers(const QString &text);

        void highlightOther(const QString &text);

        void highlightBlock(const QString &text) override;

public:
        C_Highlighter(QTextDocument *parent = nullptr, QString filename = "");
};

#endif //HIGHLIGHTER_H
