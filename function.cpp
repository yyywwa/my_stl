#include <iostream>

#ifndef _FUNCFunctor_typeION_CPP
#define _FUNCFunctor_typeION_CPP

namespace mystd {

/*
template <typename T>
class function;

template <typename Return_type, typename ...Parameter>
class function<Return_type(Parameter...)> {
  public:
    Return_type (*invoke_function)(function*, Parameter ...) ;
    void* (*copy_function )(const function&) ;
    void (*destruct_function )(const function&) ;
  private:
    void* any_call = nullptr;
  private:
    template <typename Functor_type>
    static Return_type invoke(function* f, Parameter ...args) {
      Functor_type* invoke_obj = static_cast<Functor_type*>(f->any_call);
      return ( (*invoke_obj)(std::forward<Parameter>(args)...) );
    }

    template <typename Functor_type>
    static void* copy(const function& f) {
      return new Functor_type(*static_cast<Functor_type*>(f.any_call));
    } 

    template <typename Functor_type>
    static void destruct(const function& f) {
      delete static_cast<Functor_type*>(f.any_call);
    }
  public:
    template <typename Functor_type>
    function(Functor_type functor) : invoke_function(invoke<Functor_type>), copy_function(copy<Functor_type>), 
                          destruct_function(destruct<Functor_type>), any_call(new Functor_type(functor)) {}
    
    function() : any_call(nullptr) {}

    function(const function& f) : invoke_function(f.invoke_function), copy_function(f.copy_function),
                                  destruct_function(f.destruct_function), any_call(copy_function(f)) {}

    ~function() { destruct_function(*this); }

    Return_type operator()(Parameter ...args) {
      return  invoke_function(this, std::forward<Parameter>(args)...);
    }

    function& operator=(const function& f) {
      if (any_call != nullptr)
        destruct_function(this);
      any_call = copy_function(f);
      return *this;
    }

    template <typename Functor_type>
    function& operator=(Functor_type f) {
      if (any_call != nullptr)
        destruct_function(this);
      invoke_function = invoke<Functor_type>;
      copy_function = copy<Functor_type>;
      destruct_function = destruct<Functor_type>;
      any_call = new Functor_type(f); 
      return *this;
    }

};
*/

template <typename T>
class function;

template <typename _Return_type, typename ..._Parameter>
class function<_Return_type(_Parameter...)> {
  private:
    class _basic_function {
      public:
        virtual _Return_type operator()(_Parameter...) = 0;
        virtual ~_basic_function() {}
        virtual _basic_function* copy() = 0;
    };
    _basic_function* _handle;

    template <typename _Functor_type>
    class _function_driver : public _basic_function {
      private:
        _Functor_type base;
      public:
        _function_driver(_Functor_type& functor) : base(functor) {} //* 传进来的一定是左值

        virtual _Return_type operator()(_Parameter ...args) {
          return base(std::forward<_Parameter>(args)...);
        }

        virtual _basic_function* copy() {
          return new _function_driver<_Functor_type>(base);
        }
    };

  public:
    function() : _handle(nullptr) {}

    function(const function& f) : _handle(f._handle->copy()) {}

    template <typename _Functor_type>
    function(_Functor_type functor) : _handle(new _function_driver(functor)) {}

    ~function() {
      if (_handle != nullptr) 
        delete _handle;
    }

    bool operator==(const function& f) {
      return _handle == f._handle;
    }

    bool operator!=(const function& f) {
      return _handle != f._handle;
    }

    function& operator=(const function& f) {
      if (*this == f) {
        return *this;
      } else { 
        if (_handle != nullptr)
          delete _handle;
        _handle = f._handle->copy(); 
      }
    }

    template <typename _Functor_type>
    function& operator=(_Functor_type functor) {
      if (_handle != nullptr)
        delete _handle;
      _handle = new _function_driver(functor);
      return *this;
    }

    _Return_type operator()(_Parameter ...args) {
      return _handle->operator()(std::forward<_Parameter>(args)...);
    }

    void swap(function& f) {
      auto temp_p = f._handle;
      f._handle = _handle;
      _handle = temp_p;
      temp_p = nullptr;
    }
};

}

#endif