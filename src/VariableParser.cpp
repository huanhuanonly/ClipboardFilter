/**
 * @file VariableParser.cpp
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Implement @class VariableParser
 *     
 * @list VariableParser.h
 *       Logger.h
 */

#include "VariableParser.h"
#include "Logger.h"

using namespace huanhuan;

VariableParser::VariableParser()
{
    for (const auto& [key, value] : predefinedVariables)
    {
        _M_trie.insert(key, value);
    }
    
//    _M_trie.insert("env", &_S_env);
//    _M_trie.insert("clipboard", &_S_clipboard);
//    _M_trie.insert("hour", &_S_hour);
//    _M_trie.insert("minute", &_S_minute);
//    _M_trie.insert("second", &_S_second);
//    _M_trie.insert("msec", &_S_msec);
//    _M_trie.insert("year", &_S_year);
//    _M_trie.insert("month", &_S_month);
//    _M_trie.insert("day", &_S_day);
//    _M_trie.insert("week", &_S_week);
//    _M_trie.insert("date", &_S_date);
//    _M_trie.insert("time", &_S_time);
//    _M_trie.insert("datetime", &_S_datetime);
//    _M_trie.insert("escape", &_S_escape);
//    _M_trie.insert("linebreak", &_S_linebreak);
//    _M_trie.insert("unicode", &_S_unicode);
//    _M_trie.insert("random", &_S_random);
//    _M_trie.insert("iota", &_S_iota);
//    _M_trie.insert("index", &_S_index);
}

QString VariableParser::parse(const QString& text, const std::function<void(QStringView, const char*)>& func) const
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
            QStringView parameter = command.right(command.size() - instruct.size() - (index_of_sep != command.size())).trimmed();
            
            instruct = instruct.trimmed();
            
            auto func_ptr = _M_trie.find(instruct.toString());
            
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
                    func(command, e.what());
                }
            }
            
            // Merge with previous
            if (buffer.size() >= 2)
            {
                buffer[buffer.size() - 2].append(buffer.back());
                buffer.removeLast();
            }
            else
            {
                throw extra_end_character("Extra end-character '}'!");
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
