#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDateTime>

#include <QMutex>

#include <list>

namespace huanhuan
{

class Logger : public QObject
{
    Q_OBJECT
    
public:
    
    struct Item
    {
        QDateTime time;
        quint32   index;
        
        QString   text;
        
        Item(const QString& __text, quint32 __index)
            : time(QDateTime::currentDateTime()), index(__index), text(__text)
        { }
    };
    
    explicit Logger(QObject* parent = nullptr)
        : QObject(parent)
    { }
    
    Logger(quint32 maxCount, QObject* parent = nullptr)
        : QObject(parent), _M_maxCount(maxCount)
    { }
    
    inline Item top() const noexcept
    {
        _M_mutex.lock();
        auto& res = _M_data.front();
        _M_mutex.unlock();
        return res;
    }
    
    inline void pop() noexcept
    {
        _M_mutex.lock();
        _M_data.pop_front();
        _M_mutex.unlock();
    }
    
    inline void clear() noexcept
    {
        _M_mutex.lock();
        _M_data.clear();
        _M_mutex.unlock();
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
        
        _M_mutex.lock();
        _M_maxCount = __maxSize;
        _M_mutex.unlock();
        
        while (_M_check() == false)
        {
            pop();
        }
    }
    
    inline constexpr quint32 maxSize() const noexcept
    {
        return _M_maxCount;
    }
    
    Logger& operator<<(const QString& text)
    {
        _M_mutex.lock();
        _M_data.emplace_back(text, ++_M_index);
        _M_mutex.unlock();
        
        while (_M_check() == false)
        {
            pop();
        }
        
        emit itemAdded(_M_data.back());
        
        return *this;
    }
    
    Logger& operator>>(QString& text)
    {
        _M_mutex.lock();
        text = std::move(_M_data.front().text);
        _M_mutex.unlock();
        
        pop();
        return *this;
    }
    
    Logger& operator>>(Item& item)
    {
        item = top();
        pop();
        return *this;
    }
    
    template<typename _CallBack>
    void forEach(_CallBack __func) const noexcept
    {
        _M_mutex.lock();
        for (auto it = _M_data.cbegin(); it != _M_data.cend(); ++it)
        {
            __func(*it);
        }
        _M_mutex.unlock();
    }
    
signals:
    
    void itemAdded(const Item&);
    
private:
    
    inline bool _M_check() const noexcept
    {
        return _M_data.size() <= _M_maxCount;
    }
    
private:
    
    std::list<Item> _M_data;
    quint32 _M_index = 0;
    quint32 _M_maxCount = std::numeric_limits<quint32>::max();
    
    mutable QMutex _M_mutex;
};

extern Logger slog;

} // namespace huanhuan

#endif // LOGGER_H
