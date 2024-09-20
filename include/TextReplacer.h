#ifndef TEXTREPLACER_H
#define TEXTREPLACER_H

#include <QObject>

#include <QString>
#include <QMap>
#include <QSet>
#include <QRegularExpression>

#include "VariableParser.h"

namespace huanhuan
{

class TextReplacer
{
public:
    
    enum class MatchOptions: int
    {
        None = 0,
        Wildcard,
        Regexp
    };
    
    struct Rule
    {
        QString      pattern;
        QString      replacement;
        bool         patternEnableVariables;
        bool         replacementEnableVariables;
        MatchOptions matchOptions;
        
        explicit Rule() = default;
        
        Rule(
            const QString& __text,
            const QString& __pattern,
            bool __escapeText,
            bool __escapePattern,
            MatchOptions __matchingOptions)
                : pattern(__text), replacement(__pattern)
                , patternEnableVariables(__escapeText), replacementEnableVariables(__escapePattern)
                , matchOptions(__matchingOptions) { }
        
        
    };
    
    struct CharacterSet : public QSet<QChar>
    { };
    
public:
    
    inline TextReplacer& setVariableParser(const VariableParser& __parser)
    {
        _M_parser = __parser;
        return *this;
    }
    
    inline constexpr VariableParser& variableParser() noexcept
    {
        return _M_parser;
    }
    
    inline TextReplacer& setRule(const Rule& __rule)
    {
        _M_rule = __rule;
        return *this;
    }
    
    inline constexpr Rule& rule() noexcept
    {
        return _M_rule;
    }
    
    QString replace(const QString& __text, const std::function<void(QStringView, const char*)>& __callback = &VariableParser::defaultExceptionCallbackFunction) const
    {
        QString res = __text;
        
        QString text;
        QString pattern;
        
        if (_M_rule.patternEnableVariables)
        {
            text = _M_parser.parse(_M_rule.pattern, __callback);
        }
        else
        {
            text = _M_rule.pattern;
        }
        
        if (_M_rule.replacementEnableVariables)
        {
            pattern = _M_parser.parse(_M_rule.replacement, __callback);
        }
        else
        {
            pattern = _M_rule.replacement;
        }
        
        switch (_M_rule.matchOptions)
        {
            case MatchOptions::None:
            {
                res.replace(_M_rule.pattern, _M_rule.replacement);
                break;
            }
            
            case MatchOptions::Wildcard:
            {
                QRegExp regex(_M_rule.pattern, Qt::CaseSensitive, QRegExp::Wildcard);
                res.replace(regex, _M_rule.replacement);
                break;
            }
            
            case MatchOptions::Regexp:
            {
                QRegExp regex(_M_rule.pattern, Qt::CaseSensitive, QRegExp::RegExp2);
                res.replace(regex, _M_rule.replacement);
                break;
            }
        }
        
        return res;
    }
    
    static QString filter(const QString& __text, const CharacterSet& __cs)
    {
        QString res;
        
        for (const QChar& c : __text)
        {
            if (__cs.contains(c))
                continue;
            
            res.append(c);
        }
        
        return res;
    }
    
private:
    
    Rule _M_rule;
    VariableParser _M_parser;
};

} // namespace huanhuan

#endif // TEXTREPLACER_H
