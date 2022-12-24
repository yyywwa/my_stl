#include"./veclist.cpp"

template <typename T>
class _basic{
private:
    T* _ptr = nullptr;
    int count = 0;
public:
    _basic(T* outPtr):_ptr(outPtr){}

    void operator ++(){
        ++count;
    }

    void operator --(){
        --count;
    }

    bool operator ==(T* outPtr){
        return _ptr == outPtr;
    }

    int getCount(){
        return count;
    }
};

template <typename T>
class shar_ptr{
private:
    static veclist<_basic<T>> _shar_ptr_map;
    T* _ptr = nullptr;
public:
    shar_ptr(T* outPtr):_ptr(outPtr){
        auto it = _shar_ptr_map.find(_shar_ptr_map.begin(),_shar_ptr_map.end(),outPtr);
        if(it == -1){
            _shar_ptr_map.push_back(new _basic<T>(outPtr));
            ++_shar_ptr_map.back();
        }
        else{
            ++_shar_ptr_map[it];
        }
    }
    
    ~shar_ptr(){
        auto it = _shar_ptr_map.find(_shar_ptr_map.begin(),_shar_ptr_map.end(),_ptr);
        --_shar_ptr_map[it];
        if(_shar_ptr_map[it].getCount() == 0)
            delete[] _ptr;
    }

    T& operator *(){
        return *_ptr;
    }

    T* operator +(int n){
        return _ptr+n;
    }

    T* operator -(int n){
        return _ptr-n;
    }

    T* getPtr(){
        return _ptr;
    }
};
template <typename T>
veclist<_basic<T>> shar_ptr<T>::_shar_ptr_map;