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
    typedef T& REFERENCE;
    typedef T* POINTER;
    typedef mystd::function<void(POINTER)> POINTER_DELETER;
  private:
    POINTER _ptr;
    static veclist<SprCounter<POINTER>> _map;
    POINTER_DELETER __deleter;

    void _add_pointer(POINTER ptr) {
      auto it = _map.find(_map.begin(), _map.end(), ptr);
      if (it == _map.end()) {
        _map.push_back(new SprCounter<POINTER>(ptr)); 
      } else {
        ++(*it);
      }
    }

    void _delete(POINTER ptr) { __deleter(ptr); } 

    void _subtract_pointer(POINTER ptr) {
      if (ptr == nullptr)
        return;
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
      __deleter = default_deleter<POINTER>; }

    template <typename DELETER>
    shared_ptr(POINTER ptr, DELETER&& d) : _ptr(ptr) {
      _add_pointer(ptr);
      __deleter = std::forward<DELETER>(d); }

    ~shared_ptr() { _subtract_pointer(_ptr); }

    template <typename DELETER>
    void setDeleter(DELETER&& d) { __deleter = d; }

    REFERENCE operator*() { return *_ptr; }

    POINTER operator->() { return _ptr; }

    operator bool() { return _ptr != nullptr; }

    POINTER get() { return _ptr; }

    template <typename DELETER>
    void reset(POINTER ptr, DELETER&& d) { 
      _subtract_pointer(_ptr); 
      _ptr = ptr;
      _add_pointer(ptr);
      __deleter = d;
    }

    void reset() {
      _subtract_pointer(_ptr);
      _ptr = nullptr; }

    std::size_t use_count() { 
      auto it = _map.find(_map.begin(), _map.end(), _ptr);
      return it->get_count(); }

    void swap(shared_ptr& s) {
      POINTER temp_p = s._ptr;
      s._ptr = _ptr;
      _ptr = temp_p;
      __deleter.swap(s.__deleter);
    }
};

template <typename T>
veclist<SprCounter<T*>> shared_ptr<T>::_map;

}

#endif 