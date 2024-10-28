/**
 * @file Logger.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Record global log
 * 
 * @ingroup huanhuan
 * 
 * @include
 *     @namespace huanhuan
 *         @class    Logger
 *         @variable slog
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QTime>

#include <list>

namespace huanhuan
{

class Logger : public QObject
{
    Q_OBJECT
    
public:
    
    struct Item
    {
        QTime     time;
        quint32   index;
        
        QString   text;
        
        Item(const QString& __text, quint32 __index)
            : time(QTime::currentTime()), index(__index), text(__text)
        { }
    };
    
    explicit Logger(QObject* parent = nullptr)
        : QObject(parent), _M_data(), _M_index(0), _M_flags(linebreak)
    { }
    
    Logger(quint32 maxCount, QObject* parent = nullptr)
        : Logger(parent)
    {
        _M_maxCount = maxCount;
    }
    
    inline const Item& latest() const noexcept
    {
        return _M_data.back();
    }
    
    inline const Item& oldest() const noexcept
    {
        return _M_data.front();
    }
    
    inline Item& latest() noexcept
    {
        return _M_data.back();
    }
    
    inline Item& oldest() noexcept
    {
        return _M_data.front();
    }
    
    inline void remove_latest() noexcept
    {
        _M_data.pop_back();
    }
    
    inline void remove_oldest() noexcept
    {
        _M_data.pop_front();
    }
    
    inline void clear() noexcept
    {
        _M_data.clear();
    }
    
    inline int size() const noexcept
    {
        return static_cast<int>(_M_data.size());
    }
    
    /**
     * @throw std::invalid_argument if __maxSize is 0
     */
    inline void setMaxSize(quint32 __maxSize)
    {
        if (__maxSize == 0)
        {
            throw std::invalid_argument("The parameter __maxSize cannot be 0.");
        }
        
        _M_maxCount = __maxSize;
        
        while (_M_check() == false)
        {
            remove_oldest();
        }
    }
    
    inline constexpr quint32 maxSize() const noexcept
    {
        return _M_maxCount;
    }
    
    /**
     * @brief Not affected by maxSize()
     * @return The index of the last item
     */
    inline constexpr quint32 index() const noexcept
    {
        return _M_index;
    }
    
    Logger& operator<<(const QString& text)
    {
        if (_M_hasFlags(linebreak))
        {
            _M_data.emplace_back(text, ++_M_index);
            _M_removeFlags(linebreak);
            emit itemAdded(_M_data.back());
        }
        else
        {
            auto& ref = _M_data.back();
            
            ref.text.append(text);
            ref.time = QTime::currentTime();
            
            emit itemChanged(_M_data.back());
        }
        
        return *this;
    }
    
    Logger& operator<<(void(*func)(Logger&))
    {
        func(*this);
        return *this;
    }
    
    Logger& operator>>(QString& text)
    {
        text = std::move(_M_data.front().text);
        
        remove_oldest();
        return *this;
    }
    
    Logger& operator>>(Item& item)
    {
        item = oldest();
        remove_oldest();
        return *this;
    }
    
    template<typename _CallBack>
    void forEach(_CallBack __func) const noexcept
    {
        for (auto it = _M_data.cbegin(); it != _M_data.cend(); ++it)
        {
            __func(*it);
        }
    }
    
    friend void endl(Logger&);
    friend void backline(Logger&);
    
signals:
    
    void itemChanged(const Item&);
    void itemAdded(const Item&);
    
protected:
    
    const std::list<Item>* _M_constData() const noexcept
    {
        return &_M_data;
    }
    
    enum Flags : quint32
    {
        linebreak = 0x01
    };
    
    bool _M_hasFlags(Flags flags)
    {
        return (_M_flags & flags) == flags;
    }
    
    void _M_addFlags(Flags flags)
    {
        _M_flags |= flags;
    }
    
    void _M_removeFlags(Flags flags)
    {
        _M_flags &= ~flags;
    }
    
private:
    
    inline bool _M_check() const noexcept
    {
        return static_cast<decltype(_M_maxCount)>(_M_data.size()) <= _M_maxCount;
    }
    
private:
    
    std::list<Item> _M_data;
    quint32 _M_index;
    quint32 _M_maxCount = std::numeric_limits<quint32>::max();
    
protected:
    
    quint32 _M_flags;
};

/**
 * @brief It works similarly to std::endl for std::cout
 */
void endl(Logger&);

/**
 * @brief Go back to the beginning of the line
 */
void backline(Logger&);

extern Logger slog;

} // namespace huanhuan

#endif // LOGGER_H
