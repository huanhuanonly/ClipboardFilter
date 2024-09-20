#ifndef TRIE_H
#define TRIE_H

#include <map>

namespace huanhuan
{

template<typename _KeyType, typename _ValueType>
class Trie
{
public:
    
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
    
    ~Trie()
    {
        _M_destructor(_M_root);
    }
    
    explicit Trie(const Trie& __x)
        : Trie()
    {
        *this = __x;
    }
    
    explicit Trie(Trie&& __x)
        : Trie()
    {
        *this = std::forward(__x);
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
    
    void swap(Trie& __x)
    {
        std::swap(_M_root, __x._M_root);
    }
    
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
