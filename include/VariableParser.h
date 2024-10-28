/**
 * @file VariableParser.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Parse and replace variables in text,
 *        Defines and implements some default variables
 * 
 * @ingroup huanhuan
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
#include <QCryptographicHash>

#include <QHash>
#include <QSet>

#include <exception>
#include <functional>
#include <type_traits>

#include "Exception.h"

// Before including Trie.h

namespace huanhuan
{

template<typename = QString>
void popBack(QString& __c)
{
    __c.chop(1);
}

};

#include "Trie.h"

namespace huanhuan
{

class VariableParser : public Trie<QString, std::function<QString(QStringView, int)>>
{
public:
    
    using VariableFuncPtr = QString (*)(QStringView, int);
                                                                                      
    VariableParser();
    
    using ExtraEndCharacterException = std::invalid_argument;
    
    using InvalidArgumentException = Exception;
    
    static inline VariableParser*
    global() noexcept
    {
        static VariableParser parser;
        return &parser;
    }
    
    /**
     * @brief parse
     * 
     * @tparam _StringType can only be QString or QStringView
     * 
     * @throw ExtraEndCharacterException
     */
    template<typename _StringType>
    typename std::enable_if<std::is_same_v<_StringType, QString> || std::is_same_v<_StringType, QStringView>, QString>::type
    parse(const _StringType& __text) const;
    
    inline void
    setOnFailure(const std::function<void(QStringView, const char*)>& __func)
    {
        _M_onFailure = __func;
    }
    
    inline const std::function<void(QStringView, const char*)>&
    onFailure() const noexcept
    {
        return _M_onFailure;
    }
    
public:
    
    inline QString
    operator()(const QString& __text) const
    {
        return parse(__text);
    }
    
    inline QString
    operator()(QStringView __text) const
    {
        return parse(__text);
    }
    
public:
    
    enum ExceptionEnum : int
    {
        EmptyArgument,
        IncompleteArgument,
        NotNumber,
        NotNumberOrOutOfRange,
        NotHexNumber,
        NotHexNumberOrOutOfRange,
        NotOctNumber,
        NotOctNumberOrOutOfRange,
        MustBeOrEmpty,
        FileDoesNotExist,
        FileOpenFailed,
    };
    
    static inline constexpr std::pair<const char*, std::size_t>
    get(ExceptionEnum __code)
    {
        switch (__code)
        {
        case EmptyArgument:
            return getTextSizePair("The argument cannot be empty!");
        case IncompleteArgument:
            return getTextSizePair("Incomplete argument!");
        case NotNumber:
            return getTextSizePair("This is not a number (decimal, hexadecimal, binary or octal)!");
        case NotNumberOrOutOfRange:
            return getTextSizePair("This is not a number (decimal, hexadecimal, binary or octal) or is out of range [\0, \0]!");
        case NotHexNumber:
            return getTextSizePair("This is not a hex number!");
        case NotHexNumberOrOutOfRange:
            return getTextSizePair("This is not a hex number or is out of range [\0, \0]!");
        case NotOctNumber:
            return getTextSizePair("This is not a oct number!");
        case NotOctNumberOrOutOfRange:
            return getTextSizePair("This is not a oct number or is out of range [\0, \0]!");
        case MustBeOrEmpty:
            return getTextSizePair("The argument must be \0, or empty!");
        case FileDoesNotExist:
            return getTextSizePair("The file does not exist!");
        case FileOpenFailed:
            return getTextSizePair("File open failed!");
        default:
            return getTextSizePair("");
        }
    }
    
protected:
    
    // For param
    
    static inline void
    _S_paramTrimmed(QStringView& __param) noexcept
    {
        __param = __param.trimmed();
    }
    
    template<typename _Func>
    static inline QVector<QStringView>
    _S_paramSplit(QStringView __param, _Func __checker, bool __keepEmpty = false) noexcept
    {
        QVector<QStringView> list;
        
        int len = 0;
        
        for (int i = 0; i < static_cast<int>(__param.size()); ++i)
        {
            if (__checker(__param.at(i)))
            {
                if (len == 0 && __keepEmpty == false)
                {
                    continue;
                }
                
                list.append(__param.mid(i - len, len));
                len = 0;
            }
            else
            {
                ++len;
            }
        }
        
        if (len != 0 || __keepEmpty)
        {
            list.append(__param.mid(__param.size() - len, len));
        }
        
        return list;
    }
    
    static inline QVector<QStringView>
    _S_paramSplit(QStringView __param, const QString& __charSet, bool __keepEmpty = false) noexcept
    {
        QSet<QChar> st(__charSet.cbegin(), __charSet.cend());
        return _S_paramSplit(__param, [&st](QChar c) -> bool { return st.contains(c); }, __keepEmpty);
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
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
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
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(file)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        QFile file(__param.toString());
        
        if (file.exists() == false)
        {
            throw InvalidArgumentException(get(FileDoesNotExist));
        }
        
        if (file.open(QIODevice::ReadOnly) == false)
        {
            throw InvalidArgumentException(get(FileOpenFailed));
        }
        
        QString res = file.readAll();
        
        file.close();
        
        return res;
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
        Q_UNUSED(__callNumber)
    
        if (__param.isEmpty())        
            return QDate::currentDate().toString();
        else
            return QDate::currentDate().toString(__param);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(time)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            return QTime::currentTime().toString();
        else
            return QTime::currentTime().toString(__param);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(datetime)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            return QDateTime::currentDateTime().toString();
        else
            return QDateTime::currentDateTime().toString(__param);
    }
    
    // return special character
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(escape)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
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
            throw InvalidArgumentException(get(IncompleteArgument));
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
                throw InvalidArgumentException(get(NotHexNumberOrOutOfRange)).fmt('0').fmt("0xff");
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
                throw InvalidArgumentException(get(NotOctNumberOrOutOfRange)).fmt('0').fmt("0o777");
            }
            
            return QString(QChar(number));
        }
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(linebreak)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
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
        
        throw InvalidArgumentException(get(MustBeOrEmpty)).fmt("'CRLF' or 'LF'");
    }
    
    /**
     * @param __param is a number, dec/hex/oct or binary
     * @return The unicode character
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(unicode)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        bool ok = true;
        int number = __param.toInt(&ok, 0);
        
        if (ok == false)
        {
            throw InvalidArgumentException(get(NotNumber));
        }
        
        return QString(QChar(number));
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(toUnicode)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        QString result;
        
        for (int i = 0; i < static_cast<int>(__param.size()) - 1; ++i)
        {
            result.append(QString::number(__param.at(i).unicode()));
            result.append(' ');
        }
        
        if (__param.isEmpty() == false)
        {
            result.append(QString::number(__param.back().unicode()));
        }
        
        return result;
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(toHtml)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        return __param.toString().toHtmlEscaped();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(hex)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        bool ok = true;
        quint64 number = __param.toULongLong(&ok, 0);
        
        if (ok == false)
        {
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(number)>::min())
                    .fmt(std::numeric_limits<decltype(number)>::max());
        }
        
        return QString::number(number, 16);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(oct)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        bool ok = true;
        quint64 number = __param.toULongLong(&ok, 0);
        
        if (ok == false)
        {
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(number)>::min())
                    .fmt(std::numeric_limits<decltype(number)>::max());
        }
        
        return QString::number(number, 8);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(bin)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        bool ok = true;
        quint64 number = __param.toULongLong(&ok, 0);
        
        if (ok == false)
        {
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(number)>::min())
                    .fmt(std::numeric_limits<decltype(number)>::max());
        }
        
        return QString::number(number, 2);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(dec)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        bool ok = true;
        quint64 number = __param.toULongLong(&ok, 0);
        
        if (ok == false)
        {
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(number)>::min())
                    .fmt(std::numeric_limits<decltype(number)>::max());
        }
        
        return QString::number(number, 10);
    }
    
    /**
     * @return A random number in the range [LLONG_MIN, LLONG_MAX],
     *         or the custom range
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(random)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        auto list = __param.split(',');
        
        if (list.front().isEmpty())
            list.pop_back();
        
        bool ok = true;
        
        qint64 lo = (list.size() >= 2 ? list.front().toLongLong(&ok, 0) : 0);
        
        if (ok == false)
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(lo)>::min())
                    .fmt(std::numeric_limits<decltype(lo)>::max());
        
        qint64 hi = (list.size() >= 2
                        ? list.back().toLongLong(&ok, 0)
                        : (list.size() >= 1
                            ? list.front().toLongLong(&ok, 0)
                            : std::numeric_limits<decltype(hi)>::max()
                          )
                    );
        
        if (ok == false)
            throw InvalidArgumentException(get(NotNumberOrOutOfRange))
                    .fmt(std::numeric_limits<decltype(hi)>::min())
                    .fmt(std::numeric_limits<decltype(hi)>::max());
        
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
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(randomlySelectOneChar)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        return QString(__param[QRandomGenerator::global()->bounded(static_cast<int>(__param.size()))]);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(randomlySelectOneWord)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        auto res = _S_paramSplit(__param, [](QChar c) -> bool { return c.isPunct() || c.isSpace() || c.isSymbol(); }, false);
        return res.at(QRandomGenerator::global()->bounded(static_cast<int>(res.size()))).toString();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(randomlySelectOneLine)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
        {
            throw InvalidArgumentException(get(EmptyArgument));
        }
        
        auto res = _S_paramSplit(__param, [](QChar c) -> bool { return c == '\r' || c == '\n'; }, false);
        return res.at(QRandomGenerator::global()->bounded(static_cast<int>(res.size()))).toString();
    }
    
    /**
     * @return A continuous subsequence in the custom range
     */
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(iota)
    {
        Q_UNUSED(__callNumber)
        
        _S_paramTrimmed(__param);
        
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
        _S_paramTrimmed(__param);
        
        static int lastCallNumber = -1;
        static QHash<decltype(__param), int> map;
        
        if (__callNumber != lastCallNumber)
        {
            lastCallNumber = __callNumber;
            map.clear();
        }
        
        return QString::number(++map[__param]);
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(shuffle)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        QString result = __param.toString();
        
        std::shuffle(result.begin(), result.end(), *QRandomGenerator::global());
        
        return result;
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(sort)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        QString result = __param.toString();
        
        std::sort(result.begin(), result.end());
        
        return result;
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(reverse)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        QString result = __param.toString();
        
        std::reverse(result.begin(), result.end());
        
        return result;
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(hash)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        return QString(
                QCryptographicHash::hash(
                    __param.toLocal8Bit(),
                    QCryptographicHash::Algorithm::Md5).toHex());
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(toUpper)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        return __param.toString().toUpper();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(toLower)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        return __param.toString().toLower();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(trimmed)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        return __param.trimmed().toString();
    }
    
    VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION(simplified)
    {
        Q_UNUSED(__callNumber)
        
        if (__param.isEmpty())
            throw InvalidArgumentException(get(EmptyArgument));
        
        return __param.toString().simplified();
    }
    
#undef VARIABLE_PARSER__PREDEFINED_VARIABLE_FUNCTION
    
private:
    
    std::function<void(QStringView, const char*)> _M_onFailure;
    
    mutable int _M_callNumber = 0;
};

} // namespace huanhuan

#endif // VARIABLEPARSER_H
