//
// Created by Artur Twardzik on 31/03/2025.
//

#include "color_palette.h"
#include "highlighter.h"

ARMv6_ASM_Highlighter::ARMv6_ASM_Highlighter(QTextDocument *parent) : Highlighter(parent) {
        setupCodeHighlights();
}


void ARMv6_ASM_Highlighter::setupCodeHighlights() {
        const QString keywordPatterns[] = {
                QStringLiteral("\\bmovs\\b"),
                QStringLiteral("\\bmov\\b"),
                QStringLiteral("\\badds\\b"),
                QStringLiteral("\\badd\\b"),
                QStringLiteral("\\badcs\\b"),
                QStringLiteral("\\badr\\b"),
                QStringLiteral("\\bsubs\\b"),
                QStringLiteral("\\bsbcs\\b"),
                QStringLiteral("\\bsub\\b"),
                QStringLiteral("\\brsbs\\b"),
                QStringLiteral("\\bmuls\\b"),
                QStringLiteral("\\bcmp\\b"),
                QStringLiteral("\\bcmn\\b"),
                QStringLiteral("\\bands\\b"),
                QStringLiteral("\\beors\\b"),
                QStringLiteral("\\borrs\\b"),
                QStringLiteral("\\bbics\\b"),
                QStringLiteral("\\bmvns\\b"),
                QStringLiteral("\\btst\\b"),
                QStringLiteral("\\blsls\\b"),
                QStringLiteral("\\blsrs\\b"),
                QStringLiteral("\\basrs\\b"),
                QStringLiteral("\\brors\\b"),
                QStringLiteral("\\bldr\\b"),
                QStringLiteral("\\bldrh\\b"),
                QStringLiteral("\\bldrb\\b"),
                QStringLiteral("\\bldrsh\\b"),
                QStringLiteral("\\blsrsb\\b"),
                QStringLiteral("\\bldm\\b"),
                QStringLiteral("\\bstr\\b"),
                QStringLiteral("\\bstrh\\b"),
                QStringLiteral("\\bstrb\\b"),
                QStringLiteral("\\bstr\\b"),
                QStringLiteral("\\bstrh\\b"),
                QStringLiteral("\\bstrb\\b"),
                QStringLiteral("\\bstr\\b"),
                QStringLiteral("\\bstm\\b"),
                QStringLiteral("\\bpush\\b"),
                QStringLiteral("\\bpop\\b"),
                QStringLiteral("\\bb\\b"),
                QStringLiteral("\\bbl\\b"),
                QStringLiteral("\\bbx\\b"),
                QStringLiteral("\\bblx\\b"),
                QStringLiteral("\\bbeq\\b"),
                QStringLiteral("\\bbne\\b"),
                QStringLiteral("\\bbgt\\b"),
                QStringLiteral("\\bblt\\b"),
                QStringLiteral("\\bbge\\b"),
                QStringLiteral("\\bble\\b"),
                QStringLiteral("\\bbcs\\b"),
                QStringLiteral("\\bbcc\\b"),
                QStringLiteral("\\bbmi\\b"),
                QStringLiteral("\\bbpl\\b"),
                QStringLiteral("\\bbvs\\b"),
                QStringLiteral("\\bbvc\\b"),
                QStringLiteral("\\bbhi\\b"),
                QStringLiteral("\\bbls\\b"),
                QStringLiteral("\\bsxth\\b"),
                QStringLiteral("\\bsxtb\\b"),
                QStringLiteral("\\buxth\\b"),
                QStringLiteral("\\buxtb\\b"),
                QStringLiteral("\\brev\\b"),
                QStringLiteral("\\brev16\\b"),
                QStringLiteral("\\brevsh\\b"),
                QStringLiteral("\\bsvc\\b"),
                QStringLiteral("\\bcpsid\\b"),
                QStringLiteral("\\bcpsie\\b"),
                QStringLiteral("\\bmrs\\b"),
                QStringLiteral("\\bmsr\\b"),
                QStringLiteral("\\bbkpt\\b")
        };
        keywordFormat.setForeground(OneDarkTheme::magenta);
        keywordFormat.setFontItalic(true);
        for (const QString &pattern: keywordPatterns) {
                addHighlightingRule(keywordFormat, QRegularExpression(pattern));
        }

        quotationFormat.setForeground(OneDarkTheme::green);
        addHighlightingRule(quotationFormat, QRegularExpression(QStringLiteral("\".\"")));


        // numberFormat.setForeground(QBrush(QColor("#56B6C2")));
        // rule.pattern = QRegularExpression(QStringLiteral("#[0-9A-Fa-fx]*"));
        // rule.format = numberFormat;
        // highlightingRules.append(rule);
        //
        // identifierFormat.setForeground(QBrush(QColor("#61AFEF")));
        // rule.pattern = QRegularExpression(QStringLiteral("[^\n]*:"));
        // rule.format = identifierFormat;
        // highlightingRules.append(rule);
        //
        // directiveFormat.setForeground(QBrush(QColor("#E5C07B")));
        // rule.pattern = QRegularExpression(QStringLiteral("\\.([^\n\\s]+)(\\s|\n)"));
        // rule.format = directiveFormat;
        // highlightingRules.append(rule);
        //
        // functionFormat.setFontItalic(true);
        // functionFormat.setForeground(Qt::blue);
        // rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
        // rule.format = functionFormat;
        // highlightingRules.append(rule);
        //
        // singleLineCommentFormat.setForeground(QBrush(QColor("#ABB2BF")));
        // rule.pattern = QRegularExpression(QStringLiteral("(//[^\n]*)|(;[^\n]*)|(@[^\n]*)"));
        // rule.format = singleLineCommentFormat;
        // highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(QBrush(QColor("#ABB2BF")));

        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}
