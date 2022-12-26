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
    static veclist<_basic<T>> _shar_ptr_map;//*为相同类型指针创建表
    T* _ptr = nullptr;
    void (*deleter)(T*) = nullptr;//*用于指定对指针的析构函数,返回值为void，参数为该类型指针

    void minus(T* outPtr){
        unsigned __int64  it = find(outPtr);
        --_shar_ptr_map[it];
        if(_shar_ptr_map[it].getCount() == 0){
            if(deleter != nullptr)
                deleter(outPtr);
            else  
                delete outPtr;
            _shar_ptr_map.erase(it);
        }
    }

    void add(T* outPtr){
        unsigned __int64 it = find(outPtr);
        if(it == _shar_ptr_map.end()){
            _shar_ptr_map.push_back(new _basic<T>(outPtr));
            ++_shar_ptr_map.back();
        }
        else{
            ++_shar_ptr_map[it];
        }
    }

    unsigned __int64 find(T* outPtr){
        return _shar_ptr_map.find(_shar_ptr_map.begin(),_shar_ptr_map.end(),outPtr);
    }

public:
    shar_ptr(T* outPtr):_ptr(outPtr){
        add(outPtr);
    }
    
    shar_ptr(T* outPtr,void(*outDeleter)(T*)):_ptr(outPtr),deleter(outDeleter){
        add(outPtr);
    }

    ~shar_ptr(){
        minus(_ptr);
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

    T* operator ->(){
        return _ptr;
    }

    void reset(T* outPtr){
        if(outPtr == _ptr)
            return;
        minus(_ptr);
        add(outPtr);
        _ptr = outPtr;
    }

    void reset(const shar_ptr<T>& x){
        if(_ptr == x._ptr)
            return;
        minus(_ptr);
        add(x._ptr);
        _ptr = x._ptr;
    }

    T* get(){
        return _ptr;
    }

    int use_count(){
        unsigned __int64 it = find(_ptr);
        return _shar_ptr_map[it].getCount();
    }

    void swap(shar_ptr<T>& A){
        T* temp = A._ptr;
        A._ptr = this->_ptr;
        this->_ptr = temp;
    }
};
template <typename T>
veclist<_basic<T>> shar_ptr<T>::_shar_ptr_map;