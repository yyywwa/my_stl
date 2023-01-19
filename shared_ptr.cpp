#include "./veclist.cpp"
#include "./function.cpp"

#ifndef _SHARED_PTR_CPP
#define _SHARED_PTR_CPP

namespace mystd {

template <typename POINTER>
class SprCounter { //* shared pointer conter
private:
  POINTER _ptr;
  int _count;
public:
  SprCounter(POINTER ptr):_ptr(ptr), _count(1) {}

  POINTER get_ptr() { return _ptr; }

  int get_count() { return _count; }

  void operator++() { ++_count; }

  void operator--() { --_count; }

  bool operator==(POINTER ptr) { return _ptr == ptr; }
}; 

template <typename POINTER>
void default_deleter(POINTER ptr) { delete ptr; }

template <typename T>
class shared_ptr {
  public:
    typedef T* POINTER;
    typedef mystd::function<void(POINTER)> POINTER_DELETER;
  private:
    POINTER _ptr;
    static veclist<SprCounter<POINTER>> _map;
    POINTER_DELETER _deleter_pointer;

    void _add_pointer(POINTER ptr) {
      auto it = _map.find(_map.begin(), _map.end(), ptr);
      if (it == _map.end()) {
        _map.push_back(new SprCounter<POINTER>(ptr)); 
      } else {
        ++(*it);
      }
    }

    void _delete(POINTER ptr) {
      if (ptr != nullptr) {
        _deleter_pointer(ptr);
      }
    } 

    void _subtract_pointer(POINTER ptr) {
      auto it = _map.find(_map.begin(), _map.end(), ptr);
      if (it == _map.end()) {
        return;
      } else {
        --(*it);
        if (it->get_count() == 0) {
          _delete(_ptr);
          _map.erase(it);
        }
      }
    }
    
  public:
    shared_ptr(POINTER ptr) : _ptr(ptr) {
      _add_pointer(ptr);
      _deleter_pointer = default_deleter<POINTER>;
    }

    template <typename DELETER>
    shared_ptr(POINTER ptr, DELETER&& d) : _ptr(ptr) {
      _add_pointer(ptr);
      _deleter_pointer = std::forward<DELETER>(d);
    }

    template <typename DELETER>
    void setDeleter(DELETER&& d) {
        _deleter_pointer = d;
    }


    ~shared_ptr() {
      _subtract_pointer(_ptr);
    }
};

template <typename T>
veclist<SprCounter<T*>> shared_ptr<T>::_map;

}

#endif 