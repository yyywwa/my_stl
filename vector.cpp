#include<iostream>
#include"./other.cpp"
#ifndef _MY_VECTOR_CPP
#define _MY_VECTOR_CPP
                //*一切有为法
                //*如梦幻泡影
                //*如露亦如电
                //*应做如是观
namespace mystd {
template <typename _Tp>
class vector {
  #define _DEFUALT_SIZE 10
  public:
    using _Ref = vector<_Tp>;
    using size_type = std::size_t;
    using value_type = _Tp;
    using reference = _Tp&;
    using iterator = _Tp*;
  private:
    iterator start;
    iterator finish;
    iterator end_of_storage;

    void _create(size_type size = _DEFUALT_SIZE) {
      if (size <= _DEFUALT_SIZE)
        size = _DEFUALT_SIZE;
      start = new value_type[size]();
      finish = start;
      end_of_storage = start + size;
    } 

    void _create(size_type size, const value_type& x) {
      start = new value_type[size]();
      end_of_storage = start + size;
      fill_n(start, end_of_storage, x);
      finish = end_of_storage;
    }

    void _inside_copy(iterator first, iterator last, iterator new_first) {
      if (new_first == first)
        return;
      if (new_first < first || new_first >= end_of_storage) {
        for (;first != last;++new_first, ++first)
          *new_first = *first;
      } else { //* first < new_first < end_of_storage
        iterator temp1 = new_first - first + last - 1;
        iterator temp2 = last - 1;
        for (;temp2 >= first;--temp1, --temp2)
          *temp1 = *temp2;
      }
    }

    void _exterior_copy
    (iterator first, iterator last, iterator new_first) { copy(first, last, new_first); }


    void _realloc(size_type size) {
      if (size < finish - start) {
        finish = start;
        return;
      } else {
        delete[] start;
        start = new value_type[size]();
        finish = start;
        end_of_storage = start + size;
      }
    }

    void _keep_realloc(size_type size) {
      iterator temp = new value_type[size]();
      _exterior_copy(start, finish, temp);//*值传递
      finish = temp + (finish - start);
      delete[] start;
      start =  temp;
      end_of_storage = start + size;
    }

  public:

    void _add_finish(size_type i) { finish += i; }

    vector() { _create(); }

    vector(size_type size) { _create(size); }

    vector(size_type size, const value_type& x) { _create(size, x); }

    vector(_Ref& v) {
      _create(v.capacity());
      _exterior_copy(v.begin(), v.end(), start);
      finish = start + v.size();
    }

    vector(iterator first, iterator last) {
      if (first != nullptr && last != nullptr) {
        size_type size = last - first;
        _create(size * 2);
        _exterior_copy(first, last, start);
        finish = start + size;
      } else {
        _create();
      }
    }

    template <typename ...Args>
    vector(Args&& ...args) {
      _create(sizeof...(args) * 2);
      ((push_back(std::forward<value_type>(args))), ...);
      finish = start + sizeof...(args);
    }

    ~vector() { delete[] start; }

    size_type size() { return finish - start; }

    size_type capacity() { return end_of_storage - start; }

    iterator begin() { return start; }

    iterator end()   { return finish; }

    reference front() { return *start; }

    reference back() { return *(finish - 1); } 
    
    _Ref& operator=(_Ref&& v) {
      this->swap(v);
      return *this; } 

    _Ref& operator=(_Ref& v) {
      if (this == &v)
        return *this;

      if (capacity() < v.capacity()) {
        _realloc(v.capacity());
      } else {
        clear();
      }
      _exterior_copy(v.begin(), v.end(), start);
    }

    bool empty() { return start == finish; }

    bool operator==(_Ref& v) { return start == v.start; }

    template <typename _T>
    void push_back(_T&& x) {
      if (finish != end_of_storage) {
        *finish = std::forward<_T>(x);
        ++finish;
      } else {
        _keep_realloc(size() * 2);
        *finish = x;
        ++finish;
      }
    }

    void pop_back() {
      if (finish != start)
        --finish; }

    reference operator[] (size_type i) { return start[i]; }

    iterator erase(iterator first, iterator last) {
      if (first >= start && last <= end_of_storage) {
        _inside_copy(last, finish, first);
        finish -= last - first;
      } else {
        throw "no find the number\n";
      }
      return first;
    }

    iterator erase(iterator first) { return erase(first, first + 1); }

    iterator clear() { return start = finish; } //*erase(start, finish) 层层调用太麻烦了

    iterator insert(iterator first, size_type size, const value_type& x) {
      if (first >= start && first < end_of_storage) {
        if (finish + size >= end_of_storage) {
            size_type temp = finish - first;
            _keep_realloc((this->size() + size) * 2);
            first = finish - temp;//* realloc之后就换地址了
        }
        _inside_copy(first, finish, first + size);
        fill_n(first, first + size, x);
        finish += size;
      }
      return first;
    } 

    iterator insert(iterator first, const value_type& x) { return insert(first, 1, x); }

    iterator insert(iterator position, iterator array_first, iterator array_last) {
      if (position >= start && position < end_of_storage) {
        size_type size = array_last - array_first;
        if (finish + size >= end_of_storage) {
            size_type temp = finish - position;
            _keep_realloc((this->size() + size) * 2);
            position = finish - temp;//* realloc之后就换地址了
        }
        _inside_copy(position, finish, position + size);
        _exterior_copy(array_first, array_last, position);
        finish += size;
      }
      return position;
    }

    void resize(size_type size) {
      if (size <= this->size()) {
        finish = start + size;
      } else {
        _keep_realloc(size * 2);
        finish = start + size;          
      }
    }

    void resize(size_type size, const value_type& x) {
      if (size <= this->size()) {
        finish = start + size;
      } else {
        insert(finish, size - this->size(), x);
        finish = start + size;
      }
    }

    void reserver(size_type size) {
      if (size > this->size())
        _keep_realloc(size); }

    void swap(_Ref& v) {
      iterator temp = v.start;
      v.start = start;
      start = temp;
      
      temp = v.finish;
      v.finish = finish;
      finish = temp;

      temp = v.end_of_storage;
      v.end_of_storage = end_of_storage;
      end_of_storage = temp;
    }

    void swap(_Ref&& v) {
      iterator temp = v.start;
      v.start = start;
      start = temp;
      
      temp = v.finish;
      v.finish = finish;
      finish = temp;

      temp = v.end_of_storage;
      v.end_of_storage = end_of_storage;
      end_of_storage = temp;
    }//* 自己的start交给右值对象删除

    iterator find(const value_type& x) {
      iterator temp;
      for (temp = start;temp != finish;++temp)
        if (*temp == x)
          break;
      return temp; 
    }

    reference at(size_type i) {
      if (i >= size())
        throw "Array out of bounds";
      return start[i];
    }

    void realloc(size_type size) { _realloc(size); }

    size_type get_number(iterator i) {
      if (i >= start && i < finish)
        return i - start;
      else
        return 0;
    }
};
}
#endif 