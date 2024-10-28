/**
 * @file Exception.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Exception class
 * 
 *        Supports placeholders and subsequent
 *        filling with values.
 * 
 * @ingroup huanhuan
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <type_traits>

namespace huanhuan
{

class Exception : public std::exception
{
public:
    
    template<std::size_t _Size>
    Exception(const char (&__what)[_Size]) noexcept
        : _M_what(__what, _Size)
    { }
    
    Exception(const char* __what, std::size_t __n) noexcept
        : _M_what(__what, __n)
    { }
    
    /**
     * This constructor can be called with the result
     * of the getTextSizePair() function as a parameter.
     * 
     * In order to avoid losing string size during
     * parameter passing and return value.
     */
    Exception(const std::pair<const char*, std::size_t>& __what) noexcept
        : _M_what(__what.first, __what.second)
    { }
    
    Exception(const std::string& __what) noexcept
        : _M_what(__what)
    { }
    
    Exception(std::string&& __what) noexcept
        : _M_what(std::forward<std::string>(__what))
    { }
    
    virtual const char*
    what() const noexcept override
    {
        return _M_what.c_str();
    }
    
    template<std::size_t _Size>
    Exception&
    fmt(const char (&__arg)[_Size])
    {
        if (_M_nextInsertionPoint()) [[likely]]
        {
            _M_what.replace(_M_pos, 1, __arg, _Size);
        }
        else [[unlikely]]
        {
            _M_what.append(__arg, _Size);
        }
        return *this;
    }
    
    Exception&
    fmt(char __arg)
    {
        if (_M_nextInsertionPoint()) [[likely]]
        {
            _M_what[_M_pos] = __arg;
        }
        else [[unlikely]]
        {
            _M_what.push_back(__arg);
        }
        return *this;
    }
    
    template<typename _Tp>
    typename std::enable_if<std::is_integral_v<_Tp>, Exception>::type&
    fmt(const _Tp& __arg)
    {
        if (_M_nextInsertionPoint()) [[likely]]
        {
            _M_what.replace(_M_pos, 1, std::to_string(__arg));
        }
        else [[unlikely]]
        {
            _M_what.append(std::to_string(__arg));
        }
        return *this;
    }
    
    template<typename _Tp>
    typename std::enable_if<std::is_floating_point_v<_Tp>, Exception>::type&
    fmt(const _Tp& __arg)
    {
        if (_M_nextInsertionPoint()) [[likely]]
        {
            _M_what.replace(_M_pos, 1, std::to_string(__arg));
        }
        else [[unlikely]]
        {
            _M_what.append(std::to_string(__arg));
        }
        return *this;
    }
    
    Exception&
    fmt(const std::string& __arg)
    {
        if (_M_nextInsertionPoint()) [[likely]]
        {
            _M_what.replace(_M_pos, 1, __arg);
        }
        else [[unlikely]]
        {
            _M_what.append(__arg);
        }
        return *this;
    }
    
    static constexpr inline char placeholder = '\0';
    
private:
    
    bool
    _M_nextInsertionPoint() noexcept
    {
        _M_pos = _M_what.find_first_of(placeholder, _M_pos);
        
        if (_M_pos != std::string::npos) [[likely]]
        {
            return true;
        }
        else [[unlikely]]
        {
            return false;
        }
    }
    
private:
    
    std::string _M_what;
    std::size_t _M_pos = 0;
};

template<std::size_t _Size>
static inline constexpr std::pair<const char*, std::size_t>
getTextSizePair(const char (&__text)[_Size])
{
    return { __text, _Size };
}

} // namespace huanhuan

#endif // EXCEPTION_H
