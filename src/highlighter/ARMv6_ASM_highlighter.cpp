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
                "\\bmovs\\b", "\\bmov\\b", "\\badds\\b", "\\badd\\b", "\\badcs\\b", "\\badr\\b", "\\bsubs\\b",
                "\\bsbcs\\b", "\\bsub\\b", "\\brsbs\\b", "\\bmuls\\b", "\\bcmp\\b", "\\bcmn\\b", "\\bands\\b",
                "\\beors\\b", "\\borrs\\b", "\\bbics\\b", "\\bmvns\\b", "\\btst\\b", "\\blsls\\b", "\\blsrs\\b",
                "\\basrs\\b", "\\brors\\b", "\\bldr\\b", "\\bldrh\\b", "\\bldrb\\b", "\\bldrsh\\b", "\\blsrsb\\b",
                "\\bldm\\b", "\\bstr\\b", "\\bstrh\\b", "\\bstrb\\b", "\\bstr\\b", "\\bstrh\\b", "\\bstrb\\b",
                "\\bstr\\b", "\\bstm\\b", "\\bpush\\b", "\\bpop\\b", "\\bb\\b", "\\bbl\\b", "\\bbx\\b",
                "\\bblx\\b", "\\bbeq\\b", "\\bbne\\b", "\\bbgt\\b", "\\bblt\\b", "\\bbge\\b", "\\bble\\b",
                "\\bbcs\\b", "\\bbcc\\b", "\\bbmi\\b", "\\bbpl\\b", "\\bbvs\\b", "\\bbvc\\b", "\\bbhi\\b",
                "\\bbls\\b", "\\bsxth\\b", "\\bsxtb\\b", "\\buxth\\b", "\\buxtb\\b", "\\brev\\b", "\\brev16\\b",
                "\\brevsh\\b", "\\bsvc\\b", "\\bcpsid\\b", "\\bcpsie\\b", "\\bmrs\\b", "\\bmsr\\b", "\\bbkpt\\b"
        };
        keywordFormat.setForeground(OneDarkTheme::magenta);
        keywordFormat.setFontItalic(true);
        for (const QString &pattern: keywordPatterns) {
                addHighlightingRule(keywordFormat, pattern);
        }

        quotationFormat.setForeground(OneDarkTheme::green);
        addHighlightingRule(quotationFormat, "\".*\"");

        numberFormat.setForeground(OneDarkTheme::cyan);
        addHighlightingRule(numberFormat, "#[0-9A-Fa-fx]*");

        identifierFormat.setForeground(OneDarkTheme::blue);
        addHighlightingRule(identifierFormat, "[^\n]*:");

        directiveFormat.setForeground(OneDarkTheme::yellow);
        addHighlightingRule(directiveFormat, R"(\.\S*(\s|$))");

        escapeSequenceFormat.setForeground(OneDarkTheme::blue);
        addHighlightingRule(escapeSequenceFormat, R"((\\[abefnrtv\\'"?])|(%[diuoxXfFeEgGaAcspn%]))");

        singleLineCommentFormat.setForeground(OneDarkTheme::lightGray);
        singleLineCommentFormat.setFontItalic(true);
        addHighlightingRule(singleLineCommentFormat, "(//[^\n]*)|(;[^\n]*)|(@[^\n]*)");

        multiLineCommentFormat.setForeground(OneDarkTheme::lightGray);
        multiLineCommentFormat.setFontItalic(true);
        commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
        commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}
