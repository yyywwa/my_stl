#include"./veclist.cpp"
#include<functional>
#ifndef _SHARED_PTR_CPP
#define _SHARED_PTR_CPP
namespace mystd{
    template <typename T>
    class _basic{
    private:
        T* _ptr = nullptr;
        int count = 0;
    public:
        _basic(T* outPtr):_ptr(outPtr) {}

        void operator ++() { ++count; }

        void operator --() { --count; }

        bool operator ==(T* outPtr) { return _ptr == outPtr; }//* mystd::veclist::finde 要用 

        int getCount() { return count; }

    };//*创建表需要用到这个类

    template <typename T>
    class shared_ptr{
    private:
        static veclist<_basic<T>> _shared_ptr_map;//*为相同类型指针创建表
        T* _ptr = nullptr;
        void (*deleterFunction_ptr)(T*) = nullptr;//*用于指定对指针的析构函数, 返回值为void，参数为该类型指针
        
        class _basic_deleter {
            public:
                virtual void operator()(T*) = 0;
                
                virtual _basic_deleter* get_new() = 0;

                virtual ~_basic_deleter() {}
        };
        _basic_deleter* _deleter_ptr = nullptr;

        void minus(T* outPtr) {
            if(outPtr == nullptr) 
                return;
            unsigned __int64  it = find(outPtr);
            if(it != _shared_ptr_map.end()) {
                    --_shared_ptr_map[it];
                if(_shared_ptr_map[it].getCount() == 0) {
                    if(deleterFunction_ptr != nullptr)
                        deleterFunction_ptr(outPtr);
                    else if(_deleter_ptr != nullptr)
                        _deleter_ptr->operator()(outPtr);
                    else  
                        delete outPtr;
                    _shared_ptr_map.erase(it);
                    _ptr = nullptr;
                }
            }
        }

        void add(T* outPtr) {
            unsigned __int64 it = find(outPtr);
            if(it == _shared_ptr_map.end()) {
                _shared_ptr_map.push_back(new _basic<T>(outPtr));
                ++_shared_ptr_map.back();
            }
            else{
                ++_shared_ptr_map[it];
            }
        }

        unsigned __int64 find(T* outPtr) {
            return _shared_ptr_map.find(_shared_ptr_map.begin(), _shared_ptr_map.end(), outPtr);
        }

    public:



        shared_ptr(T* outPtr):_ptr(outPtr) {
            add(outPtr);
        }
        
        shared_ptr(T* outPtr, void(*outDeleter)(T*)):_ptr(outPtr), deleterFunction_ptr(outDeleter) {
            add(outPtr);
        }
        
        
        template <typename D>
        static auto convert(D&& outDeleter) {
            return static_cast<void(*)(T*)>(outDeleter);
        }

        template <typename D>
        shared_ptr(T* outPtr, D&& outDeleter):_ptr(outPtr) {
            add(outPtr);

            class _deleter :public D, public _basic_deleter {
                public:
                    virtual void operator() (T* outPtr) {
                        D::operator() (outPtr);
                    }

                    virtual _basic_deleter* get_new() {
                        return new _deleter;
                    }
            };
            _deleter_ptr = new _deleter;

        }       
        
        shared_ptr(T* outPtr, void* _d_ptr):_ptr(outPtr), _deleter_ptr(static_cast<_basic_deleter*>(_d_ptr)) {
            add(outPtr);
        } //*用于接get_deleter() 返回的指针，用_basic_deleter类型的指针会报错


        ~shared_ptr() { 
            minus(_ptr);
            if(_deleter_ptr != nullptr) 
                delete _deleter_ptr;
        }

        T& operator *() { return *_ptr; }

        T* operator +(int n) { return _ptr+n; }

        T* operator -(int n) { return _ptr-n; }

        T* operator ->() { return _ptr; }

        void reset(T* outPtr) {
            if(outPtr == _ptr)
                return;
            minus(_ptr);
            add(outPtr);
            _ptr = outPtr;
        }

        void reset(const shared_ptr<T>& x) {
            if(_ptr == x._ptr)
                return;
            minus(_ptr);
            add(x._ptr);
            _ptr = x._ptr;
        }

        void setDeleter(void (*outDeleter)(T*)) { deleterFunction_ptr = outDeleter; }

        template<typename D>
        void setDeleter(const D& outDeleter) {
            class _deleter_2:public D, public _basic_deleter {
                public:
                    virtual void operator() (T* outPtr) {
                        D::operator()(outPtr);
                    }

                    virtual _basic_deleter* get_new() {
                        return new _deleter_2;
                    }
            };

            if(_deleter_ptr != nullptr)
                delete _deleter_ptr;
            _deleter_ptr = new _deleter_2;
        }

        int use_count() {
            unsigned __int64 it = find(_ptr);
            return _shared_ptr_map[it].getCount();
        }

        void swap(shared_ptr<T>& A) {
            T* temp = A._ptr;
            A._ptr = this->_ptr;
            this->_ptr = temp;

            auto temp_p_f = deleterFunction_ptr;
            deleterFunction_ptr = A.deleterFunction_ptr;
            A.deleterFunction_ptr = temp_p_f;

            auto temp_p = _deleter_ptr;
            _deleter_ptr = A._deleter_ptr;
            A._deleter_ptr = temp_p;
        }
        
        void release() { minus(_ptr); }

        T* get() { return _ptr; }

        void* get_deleter() {
            if(_deleter_ptr != nullptr)
                return _deleter_ptr->get_new();
            else
                return nullptr;
        }//*用_basic_deleter类的指针使用构造函数时会报错，不知道为什么

        auto get_deleter_function() {
            return deleterFunction_ptr;
        }

    };

    template <typename T>
    veclist<_basic<T>> shared_ptr<T>::_shared_ptr_map;
}
#endif