/**
 * @file Trie.h
 * 
 * @author YangHuanhuan (3347484963@qq.com)
 * 
 * @brief Data structures, Trie implementation with iterator
 * 
 *        For the key-value mapping of a sequence,
 *        the time complexity of insertion and search
 *        is O(|S|logN), where |S| is the length of
 *        the sequence and N is the number of child
 *        nodes of each node.
 * 
 * @ingroup huanhuan
 */

#ifndef TRIE_H
#define TRIE_H

#include <map>
#include <stack>
#include <iterator>

#include <type_traits>

namespace huanhuan
{

/**
 * Some functions of Trie need to perform some
 * operations on the container. Since the interfaces
 * provided by each container may be different,
 * the following provides common default methods.
 * 
 * If the container is not applicable to the following
 * methods, a specialized version needs to be provided.
 */

template<typename _Container, typename _ValueType>
std::void_t<decltype(std::declval<_Container>().back())>
setBackValue(_Container& __c, const _ValueType& __value)
{
    __c.back() = __value;
}

template<typename _Container, typename _ValueType>
std::void_t<decltype(std::declval<_Container>().push_back(std::declval<_ValueType>()))>
pushBack(_Container& __c, const _ValueType& __value)
{
    __c.push_back(__value);
}

template<typename _Container>
std::void_t<decltype(std::declval<_Container>().pop_back())>
popBack(_Container& __c)
{
    __c.pop_back();
}

template<typename _KeyType, typename _ValueType,
         typename _T = std::void_t<typename _KeyType::iterator,
                                decltype(std::declval<_KeyType>().begin()),
                                decltype(std::declval<_KeyType>().end())>>
class Trie
{
public:
    
    static_assert(std::is_same_v<_T, void>, "The third template parameter should not be filled in!");
    
    using key_type = _KeyType;
    using value_type = _ValueType;
    
    using node_type = typename _KeyType::value_type;
    
    struct node
    {
        std::map<node_type, node*> son;
        
        /**
         * (value == nullptr) if this node is empty.
         */
        value_type* value = nullptr;
    };
    
    Trie()
        : _M_root(new node())
    { }
    
    template<typename _InputIterator>
    Trie(_InputIterator __first, _InputIterator __last)
        : Trie()
    {
        for (; __first != __last; ++__first)
        {
            insert(__first->first, __first->second);
        }
    }
    
    Trie(std::initializer_list<std::pair<key_type, value_type>> __init)
        : Trie(__init.begin(), __init.end())
    { }
    
    virtual ~Trie()
    {
        _M_destructor(_M_root);
    }
    
    Trie(const Trie& __x)
        : Trie()
    {
        *this = __x;
    }
    
    Trie(Trie&& __x)
        : Trie()
    {
        *this = std::forward<Trie>(__x);
    }
    
    // Copy
    Trie& operator=(const Trie& __x)
    {
        _M_copy(_M_root, __x._M_root);
        return *this;
    }
    
    // Move
    Trie& operator=(Trie&& __x)
    {
        swap(__x);
        
        /// The _M_root is allocated when the constructor
        /// is called and can only be released when the
        /// destructor is called.
        
        for (const auto& [key, value] : __x._M_root->son)
        {
            _M_destructor(value);
        }
        
        __x._M_root->son.clear();
        
        return *this;
    }
    
private:
    
    // Iterator part
    
    class TrieIterator : public std::iterator<std::bidirectional_iterator_tag, Trie>
    {
    public:
        
        TrieIterator() = default;
        
        friend Trie;
        
    protected:
        
        explicit TrieIterator(node* __nd) noexcept
            : _M_root(__nd)
        { }
        
        explicit TrieIterator(std::nullptr_t) noexcept
            : _M_root(nullptr)
        { }
        
        struct stack_item
        {
            node* _M_node;
            typename decltype(node::son)::iterator _M_iter;
        };
        
        inline value_type* get_value_pointer(const stack_item& __range)
        {
            if (__range._M_iter != __range._M_node->son.end())
                return __range._M_iter->second->value;
            else
                return nullptr;
        }
        
        TrieIterator& next()
        {
            if (_M_stack.empty())
            {
                _M_stack.push({_M_root, _M_root->son.begin()});
                pushBack(_M_key, _M_root->son.begin()->first);
                
                // Found a value
                if (value_type* value_ptr = get_value_pointer(_M_stack.top()); value_ptr != nullptr)
                {
                    return *this;
                }
            }
            
            while (not _M_stack.empty())
            {
                if (_M_stack.top()._M_iter != _M_stack.top()._M_node->son.end())
                {
                    node* nt = _M_stack.top()._M_iter->second;
                    
                    _M_stack.push({nt, nt->son->begin()});
                    pushBack(_M_key, nt->son->begin()->first);
                    
                    // Found a value
                    if (value_type* value_ptr = get_value_pointer(_M_stack.top()); value_ptr != nullptr)
                    {
                        return *this;
                    }
                }
                else
                {
                    _M_stack.pop();
                    popBack(_M_key);
                    
                    if (not _M_stack.empty())
                    {
                        ++_M_stack.top()._M_iter;
                    }
                }
            }
            
            return *this;
        }
        
        TrieIterator& prev()
        {
            if (_M_stack.empty())
            {
                // Use .end() as .rend()
                if (_M_root->son.empty())
                {
                    _M_stack.push({_M_root, _M_root->son.end()});
                }
                else
                {
                    auto it = std::prev(_M_root->son.end());
                    _M_stack.push({_M_root, it});
                    pushBack(_M_key, it->first);
                }
                
                // Found a value
                if (value_type* value_ptr = get_value_pointer(_M_stack.top()); value_ptr != nullptr)
                {
                    return *this;
                }
            }
            
            while (not _M_stack.empty())
            {
                if (_M_stack.top()._M_iter != _M_stack.top()._M_node->son.end())
                {
                    node* nt = _M_stack.top()._M_iter->second;
                    
                    if (nt->son.empty())
                    {
                        _M_stack.push({nt, nt->son->end()});
                    }
                    else
                    {
                        auto it = std::prev(nt->son->end());
                        _M_stack.push({nt, it});
                        pushBack(_M_key, it->first);
                    }
                    
                    // Found a value
                    if (value_type* value_ptr = get_value_pointer(_M_stack.top()); value_ptr != nullptr)
                    {
                        return *this;
                    }
                }
                else
                {
                    _M_stack.pop();
                    popBack(_M_key);
                    
                    if (not _M_stack.empty())
                    {
                        if (_M_stack.top()._M_iter == _M_stack.top()._M_node->son.begin())
                            _M_stack.top()._M_iter = _M_stack.top()._M_node->son.end();
                        else
                            --_M_stack.top()._M_iter;
                    }
                }
            }
            
            return *this;
        }
        
    public:
        
        bool operator==(const TrieIterator& __iter) const noexcept
        {
            if (_M_stack.empty() != __iter._M_stack.empty())
            {
                return false;
            }
            
            if (_M_stack.empty())
            {
                return true;
            }
            
            return _M_stack.top() == __iter._M_stack.top();
        }
        
        bool operator!=(const TrieIterator& __iter) const noexcept
        {
            return !(*this == __iter);
        }
        
        value_type& operator*() noexcept
        {
            if (_M_stack.empty())
                return *_M_root->value;
            else
                return *get_value_pointer(_M_stack.top());
        }
        
        const value_type& operator*() const noexcept
        {
            if (_M_stack.empty())
                return *_M_root->value;
            else
                return *get_value_pointer(_M_stack.top());
        }
        
        std::pair<key_type&, value_type&> operator->() noexcept
        {
            return { _M_key, *(*this) };
        }
        
        std::pair<const key_type&, const value_type&> operator->() const noexcept
        {
            return { _M_key, *(*this) };
        }
        
        TrieIterator& operator++()
        {
            return next();
        }
        
        TrieIterator& operator++(int)
        {
            TrieIterator it = *this;
            return it.next();
        }
        
        TrieIterator& operator--()
        {
            return prev();
        }
        
        TrieIterator& operator--(int)
        {
            TrieIterator it = *this;
            return it.prev();
        }
        
    private:
        
        key_type _M_key;
        node* _M_root;
        
        std::stack<stack_item> _M_stack;
    };
    
public:
    
    using iterator = TrieIterator;
    using const_iterator = const TrieIterator;
    
    // If the empty sequence has a value, then .begin() == .end()
    inline iterator begin()
    {
        if (_M_root->value != nullptr)
            return iterator(_M_root);
        else
            return iterator(_M_root).next();
    }
    
    inline iterator end()
    {
        return iterator(_M_root);
    }
    
    inline const_iterator cbegin()
    {
        return begin();
    }
    
    inline const_iterator cend()
    {
        return end();
    }
    
public:
    
    void swap(Trie& __x)
    {
        std::swap(_M_root, __x._M_root);
    }
    
    // If it exists, overwrite the old value
    void insert(const key_type& __key, const value_type& __value)
    {
        node* nd = _M_apply(__key);
        
        if (nd->value != nullptr)
        {
            delete nd->value;
        }
        
        nd->value = new value_type(__value);
    }
    
    void insert(const key_type& __key, value_type&& __value)
    {
        node* nd = _M_apply(__key);
        
        if (nd->value != nullptr)
        {
            delete nd->value;
        }
        
        nd->value = new value_type(std::forward<value_type>(__value));
    }
    
    void remove(const key_type& __key)
    {
        node* nd = _M_find(__key);
        
        if (nd != nullptr && nd->value)
        {
            delete nd->value;
            nd->value = nullptr;
        }
    }

    value_type* find(const key_type& __key) const
    {
        node* nd = _M_find(__key);
        
        if (nd != nullptr)
            return _M_find(__key)->value;
        else
            return nullptr;
    }
    
    inline bool contains(const key_type& __key) const
    {
        return find(__key) != nullptr;
    }
    
    template<typename _Function = void (*)(const key_type&, value_type&)>
    void forEach(_Function __func, bool __reverse = false)
    {
        key_type key;
        
        // Preorder
        
        if (__reverse == false)
        {
            _M_preorder(_M_root, key, __func);
        }
        else
        {
            _M_reversePreorder(_M_root, key, __func);
        }
    }
    
    template<typename _Function = void (*)(const key_type&, value_type&)>
    void forEach(const key_type& __prefix, _Function __func, bool __reverse = false)
    {
        key_type key(__prefix);
        node* nd = _M_find(__prefix);
        
        if (nd == nullptr)
        {
            return;
        }
        
        // Preorder
        
        if (__reverse == false)
        {
            _M_preorder(nd, key, __func);
        }
        else
        {
            _M_reversePreorder(nd, key, __func);
        }
    }
    
    /**
     * @brief operator[]
     * @return value, or create one if it does't exist
     */
    value_type& operator[](const key_type& __key)
    {
        node* nd = _M_apply(__key);
        
        if (nd->value == nullptr)
        {
            nd->value = new value_type();
        }
        
        return *(nd->value);
    }
    
private:
    
    /**
     * @brief _M_apply
     * @return node, or create one if it does't exist
     */
    node* _M_apply(const key_type& __key)
    {
        node* now = _M_root;
        
        for (const node_type& c : __key)
        {
            node*& next = now->son[c];
            
            if (next == nullptr)
            {
                next = new node();
            }
            
            now = next;
        }
        
        return now;
    }
    
    /**
     * @brief _M_find
     * @return node, or nullptr if it does't exist
     */
    node* _M_find(const key_type& __key) const
    {
        node* now = _M_root;
        
        for (const node_type& c : __key)
        {
            auto it = now->son.find(c);
            
            if (it == now->son.end())
            {
                return nullptr;
            }
            
            now = it->second;
        }
        
        return now;
    }
    
    template<typename _Function = void (*)(const key_type&, value_type&)>
    void _M_preorder(node* __cur, key_type& __key, _Function& __func)
    {
        if (__cur->value != nullptr)
        {
            __func(std::as_const(__key), *__cur->value);
        }
        
        pushBack(__key, node_type());
        for (auto it = __cur->son.begin(); it != __cur->son.end(); ++it)
        {
            setBackValue(__key, it->first);
            
            _M_preorder(it->second, __key, __func);
        }
        popBack(__key);
    }
    
    template<typename _Function = void (*)(const key_type&, value_type&)>
    void _M_reversePreorder(node* __cur, key_type& __key, _Function& __func)
    {
        if (__cur->value != nullptr)
        {
            __func(std::as_const(__key), *__cur->value);
        }
        
        pushBack(__key, node_type());
        for (auto it = __cur->son.rbegin(); it != __cur->son.rend(); ++it)
        {
            setBackValue(__key, it->first);
            
            _M_preorder(it->second, __key, __func);
        }
        popBack(__key);
    }
    
    void _M_merge(node* const __lnd, node* const __rnd, bool __overwrite = false)
    {
        if (__rnd->value != nullptr)
        {
            if (__lnd->value == nullptr)
            {
                __lnd->value = new value_type(*(__rnd->value));
            }
            else
            {
                if (__overwrite)
                {
                    delete __lnd->value;
                    __lnd->value = new value_type(*(__rnd->value));
                }
            }
        }
        
        for (const auto& [key, value] : __rnd->son)
        {
            auto it = __lnd->son.find(key);
            
            if (it == __lnd->son.end())
            {
                node* nd = new node();
                
                __lnd->son[key] = nd;
                
                _M_merge(nd, value);
            }
            else
            {
                _M_merge(it->second, value);
            }
        }
    }
    
    /**
     * @brief _M_copy copy __rnd to __lnd
     * @param __lnd
     * @param __rnd
     */
    void _M_copy(node* const __lnd, node* const __rnd)
    {
        for (const auto& [key, value] : __lnd->son)
        {
            _M_destructor(value);
        }
        
        __lnd->son.clear();
        
        _M_merge(__lnd, __rnd, true);
    }
    
    void _M_destructor(node* const __nd)
    {
        if (__nd->value != nullptr)
        {
            delete __nd->value;
        }
        
        for (auto it = __nd->son.begin(); it != __nd->son.end(); it = __nd->son.erase(it))
        {
            _M_destructor(it->second);
        }
        
        delete __nd;
    }
    
private:
    
    node* _M_root;
};

} // namespace huanhuan

#endif // TRIE_H
