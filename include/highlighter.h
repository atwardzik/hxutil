//
// Created by Artur Twardzik on 23/03/2025.
//


#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QMutex>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QSettings>

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


// TODO: for now it searches million times through all dependencies, it should be optimized to
//      search in those files only if they change (QFileSystemWatcher)
class C_Highlighter final : public Highlighter {
        QTextCharFormat keywordFormat;
        QTextCharFormat preprocessorFormat;
        QTextCharFormat singleLineCommentFormat;
        QTextCharFormat quotationFormat;
        QTextCharFormat numberFormat;
        QTextCharFormat escapeSequenceFormat;

        QMutex globalIdentifiersRulesMutex;
        QList<HighlightingRule> globalIdentifiersRules;

        QMutex typedefsMutex;
        QList<HighlightingRule> typedefsRules;

        QMutex functionsMutex;
        QList<HighlightingRule> functionsRules;

        QMutex structsMutex;
        QList<HighlightingRule> structsRules;

        QMutex tagsMutex;
        QStringList tags;

        QTimer *debounceTimer;

        void setupCodeHighlights() override;

        void readTokens();

        QList<QString> detect(const QString &type);

        void addRules(const QList<QString> &newRules, QList<HighlightingRule> &dst, const QTextCharFormat &format);

        void updateIdentifierHighlights();

        void highlightBlock(const QString &text) override;

public:
        C_Highlighter(QTextDocument *parent = nullptr, QString filename = "");
};

#endif //HIGHLIGHTER_H
