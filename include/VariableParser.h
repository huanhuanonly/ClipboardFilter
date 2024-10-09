/**
 * @file VariableParser.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Parse and replace variables in text,
 *        Defines and implements some default variables
 * 
 * @include
 *     @namespace huanhuan
 *         @class VariableParser
 *     
 * @list Trie.h
 */

#ifndef VARIABLEPARSER_H
#define VARIABLEPARSER_H

#include <QObject>

#include <QString>
#include <QStringList>

#include <QProcessEnvironment>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QRandomGenerator64>
#include <QClipboard>
#include <QGuiApplication>
#include <QFile>

#include <QHash>

#include <exception>
#include <functional>

#include "Trie.h"

#if !defined STR
#define STR(str) #str
#endif

#if !defined STRVALUE
#define STRVALUE(str) STR(str)
#endif

namespace huanhuan
{

class VariableParser
{
public:
    
    VariableParser();
    
    using extra_end_character = std::invalid_argument;
    
    /**
     * @brief parse
     * 
     * @param func Called when an exception is thrown
     *     @param command : QStringView
     *     @param what : const char*
     *     
     * @throw extra_end_character
     */
    QString parse(const QString& __text, const std::function<void(QStringView, const char*)>& __func = &defaultExceptionCallbackFunction) const;
    
    static void defaultExceptionCallbackFunction(QStringView, const char*)
    { }
    
public:
    
    void insert(const QString& __instruct, const std::function<QString(QStringView, int)>& __func)
    {
        _M_trie.insert(__instruct, __func);
    }
    
    void remove(const QString& __instruct)
    {
        _M_trie.remove(__instruct);
    }
    
    QString operator()(const QString& __text) const
    {
        return parse(__text);
    }
    
public:
    
#undef  VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION
#define VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(funcName) \
    static QString _S_##funcName(QStringView __param, int __callNumber)
    
    /**
     * @return Environment variable
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(env)
    {
        Q_UNUSED(__callNumber);
        
        return QProcessEnvironment::systemEnvironment().value(__param.toString());
    }
    
    /**
     * @return Clipboard text
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(clipboard)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QGuiApplication::clipboard()->text();
    }
    
    // Returns time or date
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(hour)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QTime::currentTime().hour());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(minute)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QTime::currentTime().minute());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(second)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QTime::currentTime().second());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(msec)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QTime::currentTime().msec());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(year)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QDate::currentDate().year());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(month)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QDate::currentDate().month());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(day)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QDate::currentDate().day());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(week)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QString::number(QDate::currentDate().weekNumber());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(date)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QDate::currentDate().toString();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(time)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QTime::currentTime().toString();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(datetime)
    {
        Q_UNUSED(__param)
        Q_UNUSED(__callNumber)
        
        return QDateTime::currentDateTime().toString();
    }
    
    // return special character
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(escape)
    {
        Q_UNUSED(__callNumber)
        
        static const std::map<char, char> mp{
            {'a', '\a'},
            {'b', '\b'},
            {'f', '\f'},
            {'n', '\n'},
            {'r', '\r'},
            {'t', '\t'},
            {'v', '\v'},
            {'0', '\0'}
        };
        
        QStringView str(__param.begin() + (__param.size() >= 1 && __param.front() == '\\'), __param.end());
        
        if (str.size() == 0)
        {
            throw std::invalid_argument("Empty escape character!");
        }
        
        if (str.size() == 1)
        {
            auto it = mp.find(str.front().toLatin1());
            
            if (it != mp.end())
            {
                return QString(QChar::fromLatin1(it->second));
            }
        }
        
        // \xhh
        if (str.front().toLower() == 'x')
        {
            str = str.mid(1);
            
            bool ok = true;
            
            int number = str.toInt(&ok, 16);
            
            if (ok == false)
            {
                throw std::invalid_argument("A character that is out of range [0, F]!");
            }
            
            return QString(QChar(number));
        }
        // \ddd
        else
        {
            bool ok = true;
            
            int number = str.toInt(&ok, 8);
            
            if (ok == false)
            {
                throw std::invalid_argument("A character that is not a digit exists or is out of range [0, 7]!");
            }
            
            return QString(QChar(number));
        }
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(linebreak)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
        {
#if defined Q_OS_WINDOWS
            return QString("\r\n");
#elif defined Q_OS_LINUX
            return QString("\n");
#elif defined Q_OS_UNIX
            return QString("\n");
#else
#   error Undefined
#endif
        }
        else if (__param == QString("CRLF"))
        {
            return QString("\r\n");
        }
        else if (__param == QString("LF"))
        {
            return QString("\n");
        }
        
        throw std::invalid_argument("The argument must be 'CRLF' or 'LF', or empty!");
    }
    
    /**
     * @param __param is a number, dec/hex/oct or binary
     * @return The unicode character
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(unicode)
    {
        Q_UNUSED(__callNumber)
        
        bool ok = true;
        int number = __param.toInt(&ok, 0);
        
        if (ok == false)
        {
            throw std::invalid_argument("This is not a number (decimal, hexadecimal, binary or octal)!");
        }
        
        return QString(QChar(number));
    }
    
    /**
     * @return A random number in the range [LLONG_MIN, LLONG_MAX],
     *         or the custom range
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(random)
    {
        Q_UNUSED(__callNumber)
        
        auto list = __param.split(',');
        
        if (list.front().isEmpty())
            list.pop_back();
        
        bool ok = true;
        
        qint64 lo = (list.size() >= 2 ? list.front().toLongLong(&ok, 0) : 0);
        
        if (ok == false)
            throw std::invalid_argument("This is not a number (decimal, hexadecimal, binary or octal) or is out of range [" STRVALUE(LLONG_MIN) ", " STRVALUE(LLONG_MAX) "]!");
        
        qint64 hi = (list.size() >= 2
                        ? list.back().toLongLong(&ok, 0)
                        : (list.size() >= 1
                            ? list.front().toLongLong(&ok, 0)
                            : std::numeric_limits<decltype(hi)>::max()
                          )
                    );
        
        if (ok == false)
            throw std::invalid_argument("This is not a number (decimal, hexadecimal, binary or octal) or is out of range [" STRVALUE(LLONG_MIN) ", " STRVALUE(LLONG_MAX) "]!");
        
        if (lo > hi)
        {
            lo ^= hi;
            hi ^= lo;
            lo ^= hi;
        }
        
        quint64 res = QRandomGenerator64::global()->generate64();
        
        res %= (hi - lo);
        
        // Here is no need to use absolute values ​​here
        return QString::number(lo + static_cast<qint64>(res));
    }
    
    /**
     * @return A continuous subsequence in the custom range
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(iota)
    {
        Q_UNUSED(__callNumber)
        
        // Single character, or empty
        if (__param.size() <= 1)
        {
            return __param.toString();
        }
        
        QString res;
        
        QStringView sep;
        
        if (__param.front().isDigit() && __param.back().isDigit())
        {
            auto find_is_not_digit = [](const QChar& c) -> bool { return c.isDigit() == false; };
            
            QStringView l(
                    __param.begin(),
                    std::find_if(__param.begin(), __param.end(), find_is_not_digit));
            
            if (l.size() == __param.size())
            {
                return __param.toString();
            }
            
            QStringView r(
                    std::find_if(__param.rbegin(), __param.rend(), find_is_not_digit).base(),
                    __param.end());
            
            sep = QStringView(__param.begin() + l.size(), __param.end() - r.size());
            
            qint64 nl = l.toLongLong();
            qint64 nr = r.toLongLong();
            
            res.append(QString::number(nl));
            
            if (nl <= nr)
                for (++nl; nl <= nr; ++nl)
                    res.append(sep),
                    res.append(QString::number(nl));
            else
                for (--nl; nl >= nr; --nl)
                    res.append(sep),
                    res.append(QString::number(nl));
        }
        else
        {
            QChar l = __param.front();
            QChar r = __param.back();
            sep = QStringView(__param.begin() + 1, __param.end() - 1);
            
            if (l.isLetter() == false || r.isLetter() == false)
            {
                throw std::invalid_argument("The first and last characters are not letters or numbers!");
            }
            
            static const QString letters("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
            
            int il = letters.indexOf(l);
            int ir = letters.indexOf(r);
            
            res.append(letters[il]);
            
            if (il <= ir)
                for (++il; il <= ir; ++il)
                    res.append(sep),
                    res.append(letters[il]);
            else
                for (--il; il >= ir; --il)
                    res.append(sep),
                    res.append(letters[il]);
        }
        
        return res;
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(index)
    {
        static int lastCallNumber = -1;
        static QHash<decltype(__param), int> map;
        
        if (__callNumber != lastCallNumber)
        {
            lastCallNumber = __callNumber;
            map.clear();
        }
        
        return QString::number(++map[__param]);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(file)
    {
        Q_UNUSED(__callNumber)
        
        QFile file(__param.toString());
        
        if (file.exists() == false)
        {
            throw std::invalid_argument("The file does not exist!");
        }
        
        if (file.open(QIODevice::ReadOnly) == false)
        {
            throw std::invalid_argument("File open failed!");
        }
        
        QString res = file.readAll();
        
        file.close();
        
        return res;
    }
    
#undef VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION
    
public:
    
    inline static const std::map<const char*, QString(*)(QStringView, int)> predefinedVariables {
        { "env", &_S_env },
        { "clipboard", &_S_clipboard },
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
        { "escape", &_S_escape },
        { "linebreak", &_S_linebreak },
        { "unicode", &_S_unicode },
        { "random", &_S_random },
        { "iota", &_S_iota },
        { "index", &_S_index },
        { "file", &_S_file }
    };
    
private:
    
    Trie<QString, std::function<QString(QStringView, int)>> _M_trie;
    
    mutable int _M_callNumber = 0;
};

} // namespace huanhuan

#endif // VARIABLEPARSER_H
