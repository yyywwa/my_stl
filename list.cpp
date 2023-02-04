#include <iostream>
#include "./other.cpp"
#include "./mystl_iterator.cpp"

#ifndef _LIST_CPP
#define _LIST_CPP

namespace mystd {

  namespace _m_detail {
    template <typename _Tp>
    struct _List_node {
      public:
        using _Self      = _List_node<_Tp>;
        using _Self_pointer = _Self*;
        using value_type = _Tp;
        using pointer    = value_type*;
      public:
        value_type _value;
        _Self_pointer _prev;
        _Self_pointer _next;

        _List_node(const value_type& v)  
        : _value(v), _prev(nullptr), _next(nullptr) 
        {}

        _List_node(value_type&& v)  
        : _value(std::move(v)), _prev(nullptr), _next(nullptr) 
        {}

        _List_node()
        : _value(), _prev(nullptr), _next(nullptr) 
        {}

    };

    template <typename _Tp>
    class _List_iterator { 
      public:
        using _Self = _List_iterator<_Tp>; 
        using _Node = _List_node<_Tp> ;

        using iterator_category = bidirectional_iterator_tag;
        using value_type   = _Tp;
        using pointer      = _Tp*;
        using reference    = _Tp&;
        using difference_type = std::ptrdiff_t;
      private:  
        _Node* _p;
      public:
        _List_iterator(_Node* p) : _p(p) {}

        _Node* get() { return _p; }

        reference operator*() { return _p->_value; }

        _Node* operator-> () { return _p; }

        _Self& operator++() {
          _p = _p->_next;
          return *this; }

        _Self operator++(int) {
          _Self temp(_p);
          _p = _p->_next;
          return temp; }

        _Self& operator--() {
          _p = _p->_prev;
          return *this; }

        _Self operator--(int) {
          _Self temp(_p);
          _p = _p->_prev;
          return temp; 
        }
        
        bool operator!=(const _Self& it) { return _p != it._p; }

        bool operator==(const _Self& it) { return _p == it._p; }
    };

    template <typename _Tp>
    class _List_reverse_iterator {
      public:
        using _Self = _List_reverse_iterator<_Tp>;
        using _Node = _List_node<_Tp> ;

        using difference_type   = ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type  = _Tp;
        using pointer     = _Tp*;
        using reference   = _Tp&;
      private:  
        _Node* _p;
      public:
        _List_reverse_iterator(_Node* p) : _p(p) {}

        _Node* get() { return _p; }

        reference operator*(){ return _p->_value; }

        _Node* operator-> () { return _p; }

        _Self& operator++() {
          _p = _p->_prev;
          return *this; }

        _Self operator++(int) {
          _Self temp(_p);
          _p = _p->_prev;
          return temp; }

        _Self& operator--() {
          _p = _p->_next;
          return *this; }

        _Self operator--(int) {
          _Self temp(_p);
          _p = _p->_next;
          return temp; }

        bool operator!=(const _Self& it) { return _p != it._p; }

        bool operator==(const _Self& it) { return _p == it._p; }
    };
  }

template <typename _Tp>
class list {
  public:
    using _Self = list<_Tp>;
    using value_type = _Tp;
    using _Node = _m_detail::_List_node<value_type>; 
    using reference = value_type&;
    using iterator = _m_detail::_List_iterator<value_type>;
    using reverse_iterator = _m_detail::_List_reverse_iterator<value_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
  private:
    _Node _head;

    void _m_clear() {
      _Node* _begin = _head._next;
      _Node* _end = &_head;
      _Node* temp = _begin->_next;
      while (_begin != _end) {
        delete _begin;
        _begin = temp;
        temp = temp->_next;
      }
    }

    void _empty_initial() {
      _head._prev = &_head;
      _head._next = &_head; }

    void _transfer(iterator position, iterator first, iterator last) {
      if (position != last) {
        last->_prev->_next = position.get();
        first->_prev->_next = last.get();
        _Node* temp = first->_prev;
        first->_prev = position->_prev;
        position->_prev->_next = first.get();
        position->_prev = last->_prev;
        last->_prev = temp;
      }
    }



  public:
    list() { _empty_initial(); }

    template <typename _Iterator>
    list(_Iterator first, _Iterator last) {
      _empty_initial();
      while (first != last) {
        push_back(*first);
        ++first;
      }
    }

    list(_Self& l) {
      _empty_initial();
      iterator _begin = l.begin();
      iterator _end = l.end();
      while (_begin != _end) {
        push_back(*_begin);
        ++_begin;
      }
    }

    list(_Self&& l) {
      if (!l.empty()) {
        _head._next = l._head._next;
        _head._prev = l._head._prev;
        _head._prev->_next = &_head;
        l._empty_initial();
      }
    }

    template <typename ...Args>
    list(Args&& ...args) {
      _empty_initial();
      ((push_back(std::forward<value_type>(args))), ...);
    }

    ~list() { _m_clear(); } //*erase(begin(), end());操作太多

    bool empty() { return _head._next == &_head; }

    void splice(iterator position, _Self&& l) { 
      if (!l.empty())
        _transfer(position, l.begin(), l.end()); }

    void splice(iterator position, _Self& l) { 
      if (!l.empty())
        _transfer(position, l.begin(), l.end()); }

    void splice
    (iterator positon, _Self& l, iterator first, iterator last) {
      if(first != last)
        _transfer(positon, first, last); }

    void splice
    (iterator positon, _Self&& l, iterator first, iterator last){ 
      if (first != last)
        _transfer(positon, first, last); }

    void splice(iterator position, _Self& l, iterator i) {
      iterator j = i;
      ++j;
      if(position != i && position != j)
        _transfer(position, i, j);
    }

    void splice(iterator position, _Self&& l, iterator i) {
      iterator j = i;
      ++j;
      if(position != i && position != j)
        _transfer(position, i, j);
    }

    size_type size() {
      size_type size = 0;
      _Node* temp = &_head;
      while(temp != _head._prev) {
        ++size;
        temp = temp->_next;
      }
      return size;
    }

    reverse_iterator rbegin() { return reverse_iterator(_head._prev); }
    
    reverse_iterator rend() { return reverse_iterator(&_head); }

    iterator begin() { return iterator(_head._next); }

    iterator end() { return iterator(&_head); }

    iterator insert(iterator position, const value_type& v) {
      position->_prev->_next = new _Node((v));
      position->_prev->_next->_next = position.get();
      position->_prev->_next->_prev = position->_prev;
      position->_prev = position->_prev->_next;
      return position;
    }

    iterator insert(iterator position, value_type&& v) {
      position->_prev->_next = new _Node(std::move(v));
      position->_prev->_next->_next = position.get();
      position->_prev->_next->_prev = position->_prev;
      position->_prev = position->_prev->_next;
      return position;
    }

    void insert(iterator position, size_type n, value_type v) {
      size_type i = -1;
      while(++i < n)
        position = insert(position, v);
    }


    void assign(size_type n, value_type v) {
      clear();
      insert(begin(), n, v); }

    void push_back(const value_type& v) { insert(end(), v); }

    void push_back(value_type&& v) { insert(end(), std::move(v)); }

    void push_front(const value_type& v) { insert(begin(), v); } 

    void push_front(value_type&& v) { insert(begin(), std::move(v)); } 

    void pop_back() { erase(--end()); }

    void pop_front() { erase(begin()); }

    reference back() { return *(--end()); }

    reference front() { return *(begin()); }

    iterator erase(iterator first, iterator last) {
      if (first.get() == &_head)
        return begin();
      _Node* temp = first->_next;
      while(first != last) {
        first->_prev->_next = first->_next;
        first->_next->_prev = first->_prev;
        delete first.get();
        first = temp;
        temp = first->_next;
      }
      return last;
    }

    iterator erase(iterator position) { return erase(position++, position); }
    
    void clear() { 
      _m_clear();
      _empty_initial(); }

    void remove(const value_type& v) {
      auto temp = _head._next;
      while(temp != &_head) {
        if(temp->_value == v)
          temp = erase(iterator(temp)).get();
        else 
          temp = temp->_next;
      }
    }

    void swap(_Self& l) {
      if (!empty() && !l.empty()) {
        _Node* temp = _head._next;
        _head._next = l._head._next;
        l._head._next = temp;
        temp = _head._prev;
        _head._prev = l._head._prev;
        l._head._prev = temp;

        _head._prev->_next = &_head;
        l._head._prev->_next = &l._head;
      } else if (!empty() && l.empty()) {
        l._head._prev = _head._prev;
        l._head._next = _head._next;
        l._head._prev->_next = &l._head;
        _empty_initial();
      } else if (empty() && !l.empty()) {
        _head._prev = l._head._prev;
        _head._next = l._head._next;
        _head._prev->_next = &_head;
        l._empty_initial();
      } else if (empty() && l.empty()) {
        return;
      }
    }

    void reverse() {
      if (!empty()) {
        _Node* temp;
        _Node* _begin = begin().get();
        _Node* first = begin().get();
        _Node* last = end().get();
        while(first != last) {
          temp = first->_next;
          first->_next = first->_prev;
          first->_prev = temp;
          first = first->_prev;
        }
        _head._next = first->_prev;
        _head._prev = _begin;
      }
    }

};

}
 
#endif 