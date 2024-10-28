/**
 * @file VariableParser.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief @implements @class VariableParser
 *     
 * @ingroup huanhuan
 *     
 * @list VariableParser.h
 *       Logger.h
 */

#include "VariableParser.h"
#include "Logger.h"

using namespace huanhuan;

VariableParser::VariableParser()
    : Trie {
        // Get External Text
        { "env", &_S_env },
        { "clipboard", &_S_clipboard },
        { "file", &_S_file },
        
        // Get Current Time
        { "hour", &_S_hour },
        { "minute", &_S_minute },
        { "second", &_S_second },
        { "msec", &_S_msec },
        { "year", &_S_year },
        { "month", &_S_month },
        { "day", &_S_day },
        { "week", &_S_week },
        { "date", &_S_date },
        { "time", &_S_time },
        { "datetime", &_S_datetime },
        
        // Encode to Text
        { "escape", &_S_escape },
        { "linebreak", &_S_linebreak },
        { "unicode", &_S_unicode },
        
        // Text to Encoding
        { "tounicode", &_S_toUnicode },
        { "tohtml", &_S_toHtml },
        { "hex", &_S_hex },
        { "oct", &_S_oct },
        { "bin", &_S_bin },
        { "dec", &_S_dec },
        
        // Dynamic Variables
        { "random", &_S_random },
        { "rsoc", &_S_randomlySelectOneChar },
        { "rsow", &_S_randomlySelectOneWord },
        { "rsol", &_S_randomlySelectOneLine },
        { "iota", &_S_iota },
        { "index", &_S_index },
        
        // Text Transformation
        { "shuffle", &_S_shuffle },
        { "sort", &_S_sort },
        { "reverse", &_S_reverse },
        { "hash", &_S_hash },
        { "toupper", &_S_toUpper },
        { "tolower", &_S_toLower },
        { "trimmed", &_S_trimmed },
        { "simplified", &_S_simplified }
    }, _M_onFailure([](QStringView, const char*) -> void { })
{ }

template<typename _StringType>
typename std::enable_if<std::is_same_v<_StringType, QString> || std::is_same_v<_StringType, QStringView>, QString>::type
VariableParser::parse(const _StringType& text) const
{
    constexpr const QChar tag('$');
    constexpr const QChar range[2] = {'{', '}'};
    constexpr const QChar sep(':');
    
    // Like a stack, buffer[0] is the final result
    QStringList buffer{ QString() };
    
    for (int i = 0; i < text.size(); ++i)
    {
        if (text[i] == range[0] && i > 0 && text[i - 1] == tag)
        {
            buffer.back().remove(buffer.back().size() - 1, 1);
            buffer.append(QString());
        }
        else if (text[i] == range[1])
        {
            QStringView command(buffer.back());
            command = command.trimmed();
            
            int index_of_sep = command.indexOf(sep);
            if (index_of_sep == -1)
            {
                index_of_sep = command.size();
            }
            
            QStringView instruct = command.left(index_of_sep);
            QStringView parameter = command.right(command.size() - instruct.size() - (index_of_sep != command.size()));
            
            instruct = instruct.trimmed();
            
            auto func_ptr = find(instruct.toString());
            
            if (func_ptr != nullptr)
            {
                try
                {
                    buffer.back() = (*func_ptr)(parameter, _M_callNumber);
                    
                    if constexpr (SETTINGS__LOGGER_OUTPUT__RESULT_OF_VARIABLES)
                    {
                        slog << QObject::tr("${ %0%1 } => [%2] %3")
                                .arg(instruct)
                                .arg(parameter.isEmpty() ? QString("") : QString(" : %0").arg(parameter))
                                .arg(buffer.back().size())
                                .arg(buffer.back())
                             << endl;
                    }
                }
                catch (const std::exception& e)
                {
                    _M_onFailure(command, e.what());
                }
            }
            else if constexpr (SETTINGS__LOGGER_OUTPUT__RESULT_OF_VARIABLES)
            {
                slog << QObject::tr("Warning: ${ %0%1 } is not found!")
                        .arg(instruct)
                        .arg(parameter.isEmpty() ? QString("") : QString(" : %0").arg(parameter))
                     << endl;
            }
            
            
            // Merge with previous
            if (buffer.size() >= 2)
            {
                buffer[buffer.size() - 2].append(buffer.back());
                buffer.removeLast();
            }
            else
            {
                throw ExtraEndCharacterException("Extra end-character '}'!");
            }
        }
        else
        {
            buffer.back().append(text[i]);
        }
    }
    
    ++_M_callNumber;
    
    return buffer.front();
}

template QString VariableParser::parse(const QString&) const;
template QString VariableParser::parse(const QStringView&) const;
