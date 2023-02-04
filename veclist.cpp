#include "./vector.cpp"
#ifndef _MY_VECLIST_CPP
#define _MY_VECLIST_CPP
namespace mystd {

  namespace _m_detail {
    template <typename _Tp>
    class _Veclist_iterator { 
      public:
        using _Ref = _Veclist_iterator<_Tp>;
        using size_type = std::size_t;

        using iterator_category = std::random_access_iterator_tag;
        using value_type  = _Tp;
        using pointer     = _Tp*;
        using difference_type   = std::ptrdiff_t;
        using reference   = _Tp&;
      private:
        _Tp** _ptr;
      public:
        _Veclist_iterator() : _ptr(nullptr)  {}

        _Veclist_iterator(_Tp** ptr) : _ptr(ptr) {}

        _Tp** get() const { return _ptr; }

        reference operator*() { return **_ptr; }

        pointer operator->() { return *_ptr; }

        _Ref& operator++() { ++_ptr; return *this; }

        _Ref operator++(int) {
          _Ref temp(_ptr);
          _ptr++;
          return temp;
        }

        _Ref& operator--() { --_ptr; return *this; }

        _Ref operator--(int) {
          _Ref temp(_ptr);
          _ptr--;
          return temp;
        } 

        bool operator==(const _Ref& i) { return _ptr == i._ptr; }

        bool operator!=(const _Ref& i) { return _ptr != i._ptr; }

        size_type subtraction(const _Ref& i) { return _ptr - i._ptr; }

        _Ref operator+(size_type n) { return _Ref(_ptr + n); }

        _Ref operator-(size_type n) { return _Ref(_ptr - n); }

        difference_type operator-(const _Ref& r) { return _ptr - r._ptr; }

        friend std::ostream& operator<<(std::ostream& out, _Ref& it) {
          out << it._ptr;
          return out; }
    };
  }

template <typename _Tp>
class veclist {
  public:
    using _Ref = veclist<_Tp>;
    using value_type = _Tp;
    using reference = _Tp&;
    using size_type = std::size_t;
    using iterator = _m_detail::_Veclist_iterator<_Tp>;
  private:
    vector<value_type*> index;
    friend class vector<value_type>;
    void __clear() {
      value_type** temp = index.begin();
      value_type** last = index.end();
      while (temp != last) {
        delete *temp;
        ++temp;
      }
    }

    template <typename ...Args>
    void __init(Args&& ...args) {
      (push_back(std::forward<value_type>(args)), ...);
    }

  public:

    veclist() {}

    veclist(size_type size) : index(size) {}

    veclist(_Ref&& v) { swap(v); }//*被编译器优化了

    veclist(value_type* first, value_type* last) : index((last - first) * 2) {
      value_type** temp = index.begin();
      index._add_finish(last - first);
      while (first != last) {
        *temp = new value_type(*first);
        ++temp;
        ++first;
      }
    }

    veclist(iterator first, iterator last) : index((last - first) * 2) {
      index._add_finish(last - first);//*改变index.end()
      value_type** temp1 = index.begin();
      value_type** temp2 = first.get();
      value_type** _last = index.end();
      while (temp1 != _last) {
        *temp1 = new value_type(**temp2);
        ++temp1;
        ++temp2;
      }
    }

    veclist(_Ref& v) : index(v.index.capacity()) {
      value_type** temp1 = index.begin();
      value_type** temp2 = v.index.begin();
      index._add_finish(v.index.size());
      value_type** last = v.index.end();
      while (temp2 != last) {
        *temp1 = new value_type(**temp2);
        ++temp1;
        ++temp2;
      }
    }
    
    template <typename ...Args>
    veclist(Args&& ...args) 
    : index(sizeof...(args) * 2) { __init(std::forward<value_type>(args)...); }

    ~veclist() { __clear(); }

    void push_back(value_type& x) { index.push_back(new value_type(x)); }

    void push_back(value_type&& x) { index.push_back(new value_type(std::move(x))); }

    template<typename _T>
    void push_dif(_T&& x) {
      size_type size = index.size();
      for (size_type i = 0;i != size;++i) {
        if (*index[i] == x)
          return;//* 如果有重复的就不添加
      }
      push_back(std::forward<_T>(x));
    }

    void push_back(value_type* ptr) {
      if (ptr != nullptr)
        index.push_back(ptr); }

    void pop_back() {
      if (index.size() != 0) {
        delete index.back();
        index.pop_back();
      }
    }

    size_type size() { return index.size(); }

    value_type& front() { return *index.front(); }

    value_type& back() { return *index.back(); }

    iterator begin() { return iterator(index.begin()); }

    iterator end() { return iterator(index.end()); }

    veclist& operator=(_Ref& v) {
      if (this == &v)
        return *this;

      size_type old_size = this->size();
      size_type new_size = v.size();
      
      if (index.capacity() < v.index.capacity())
        index._keep_realloc(v.index.capacity());
      index.clear();

      value_type** _begin = index.begin(); 
      value_type** _v_begin = v.index.begin();
      for (size_type i = 0;i < old_size;++i)
        *(_begin + i) = *(_v_begin + i);

      if (old_size < new_size) {
        while (old_size < new_size) {
          this->push_back(v[old_size]);
          ++old_size;
        }
      } 
      return *this;
    }

    veclist& operator=(_Ref&& v) {
      index.swap(v.index);
      return *this; }

    value_type& operator[] (size_type i) { return *index[i]; }

    iterator erase(size_type first, size_type last) {
      if (first >= 0 && last <= index.size()) {
        value_type** temp = index.begin();
        for (auto i = first;i != last;++i)
          delete temp[i];
        index.erase(temp + first,temp + last);
      }
      return iterator(index.begin() + first);
    }//* 推荐使用数字而不是迭代器

    iterator erase(iterator first, iterator last) {
      size_type size = last.subtraction(first);
      value_type** temp = first.get();
      value_type** _last = last.get();
      while (temp != _last) {
        delete *temp;
        ++temp;
      }
      index.erase(first.get(), last.get());
      return first;
    }

    iterator erase(size_type first) { return erase(first, first + 1); }//* 推荐使用数字而不是迭代器

    iterator erase(iterator first) { return erase(first, first + 1); }

    void insert(size_type first, size_type size, const value_type& x) {
      value_type* temp[size];
      for (size_type i = 0;i < size;++i)
        temp[i] = new value_type(x);
      index.insert(index.begin() + first, temp, temp + size);
    }//* 推荐使用数字而不是迭代器

    iterator insert(iterator first, size_type size, const value_type& x) {
      value_type* temp[size];
      for (size_type i = 0;i < size;++i)
        temp[i] = new value_type(x);
      index.insert(first.get(), temp, temp + size);
    }
    
    void insert(size_type first, const value_type& x) { insert(first, 1, x); }

    iterator insert(iterator first, const value_type& x) { insert(first, 1, x); }

    void insert(size_type first, value_type* array_first, value_type* array_last) {
      size_type size = array_last - array_first;
      value_type* temp[size];
      for (size_type i = 0;i < size;++i)
        temp[i] = new value_type(array_first[i]);
      index.insert(index.begin() + first, temp, temp + size);
    }//* 推荐使用数字而不是迭代器

    iterator insert(iterator first, value_type* array_first, value_type* array_last) {
      size_type size = array_last - array_first;
      value_type* temp[size];
      for (size_type i = 0;i < size;++i)
        temp[i] = new value_type(array_first[i]);
      index.insert(first.get(), temp, temp + size);
      return first;
    }

    void swap(_Ref& v) { index.swap(v.index); }
    
    void swap(_Ref&& v) { index.swap(v.index); }

    template <typename X>
    iterator find(iterator first, iterator last, const X& x) {
      while (first != last) {
        if (*first == x)
          break;
        ++first;
      }
      return first;
    }

    reference at(size_type i) { return *index.at(i); }

    bool empty() { return index.empty(); }

    void clear() {
      __clear();
      index.clear(); }

    long get_number(const iterator& it) { return (it - begin() < 0) ? -1 : it - begin(); }
};

}
#endif 