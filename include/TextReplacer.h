/**
 * @file TextReplacer.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Parse and replace variables in
 *        text, and perform text replacement
 * 
 * @ingroup huanhuan
 * 
 * @include
 *     @namespace huanhuan
 *         @class TextReplacer
 *     
 * @list VariableParser.h
 */

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
    
    enum MatchOptions: int
    {
        CaseSensitive = 0b000,
        CaseInsensitive = 0b001,
        
        Wildcard = 0b010,
        Regex = 0b100
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
            const QString& __pattern,
            const QString& __replacement,
            bool __patternEnableVariables,
            bool __replacementEnableVariables,
            MatchOptions __matchingOptions)
                : pattern(__pattern), replacement(__replacement)
                , patternEnableVariables(__patternEnableVariables)
                , replacementEnableVariables(__replacementEnableVariables)
                , matchOptions(__matchingOptions) { }
        
        
    };
    
    struct CharacterSet : public QSet<QChar>
    { };
    
public:
    
    explicit TextReplacer(VariableParser* const __parser = VariableParser::global())
        : _M_parser(__parser)
    {
        _M_parser->insert("parse", [__parser](QStringView __param, int) -> QString
        {
            if (__param.isEmpty())
                throw VariableParser::InvalidArgumentException(VariableParser::get(VariableParser::EmptyArgument));
            
            return __parser->parse(__param);
        });
        
        _M_parser->insert("buffer", [this](QStringView, int) -> QString
        {
            return _M_buffer;
        });
    }
    
    virtual ~TextReplacer()
    {
        if (_M_parser != VariableParser::global())
        {
            delete _M_parser;
        }
    }
    
    inline void setVariableParser(VariableParser* const __parser)
    {
        _M_parser = __parser;
    }
    
    inline constexpr VariableParser* variableParser() noexcept
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
    
    QString replace(const QString& __text) const
    {
        QString res = __text;
        
        QString text;
        QString pattern;
        
        if (_M_rule.patternEnableVariables)
        {
            text = _M_parser->parse(_M_rule.pattern);
        }
        else
        {
            text = _M_rule.pattern;
        }
        
        if (_M_rule.replacementEnableVariables)
        {
            pattern = _M_parser->parse(_M_rule.replacement);
        }
        else
        {
            pattern = _M_rule.replacement;
        }
        
        switch (_M_rule.matchOptions)
        {
            case MatchOptions::CaseSensitive:
            {
                res.replace(text, pattern, Qt::CaseSensitive);
                break;
            }
            
            case MatchOptions::CaseInsensitive:
            {
                res.replace(text, pattern, Qt::CaseInsensitive);
                break;
            }
            
            case MatchOptions::Wildcard:
            {
                QRegExp regex(text, Qt::CaseSensitive, QRegExp::Wildcard);
                regex.setMinimal(true);
                res.replace(regex, pattern);
                break;
            }
            
            case MatchOptions::Wildcard | MatchOptions::CaseInsensitive:
            {
                QRegExp regex(text, Qt::CaseInsensitive, QRegExp::Wildcard);
                regex.setMinimal(true);
                res.replace(regex, pattern);
                break;
            }
            
            case MatchOptions::Regex:
            {
                QRegExp regex(text, Qt::CaseSensitive, QRegExp::RegExp2);
                regex.setMinimal(true);
                res.replace(regex, pattern);
                break;
            }
            
            case MatchOptions::Regex | MatchOptions::CaseInsensitive:
            {
                QRegExp regex(text, Qt::CaseInsensitive, QRegExp::RegExp2);
                regex.setMinimal(true);
                res.replace(regex, pattern);
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
    
    constexpr inline QString& buffer() noexcept
    {
        return _M_buffer;
    }
    
private:
    
    Rule _M_rule;
    VariableParser* _M_parser;
    QString _M_buffer;
};

} // namespace huanhuan

#endif // TEXTREPLACER_H
