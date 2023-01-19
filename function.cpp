#include<iostream>

#ifndef _FUNCTION_CPP
#define _FUNCTION_CPP

namespace mystd {

template <typename Return_type, typename ...Parameter>
class BasicFunction {
  public:
    virtual Return_type invoke(Parameter...args) = 0;
};

template <typename User_object, typename Return_type, typename ...Parameter>
class __function : public BasicFunction<Return_type, Parameter...> {
  private:
    User_object _functor;
  public:
    __function(User_object&& functor) : _functor(std::forward<User_object>(functor)) {}

    virtual Return_type invoke(Parameter... args) {
      return _functor(std::forward<Parameter>(args)...);
    }
};

template <typename T>
class function;

template <typename Return_type, typename ...Parameter>
class function<Return_type(Parameter...)> {
  private:
    BasicFunction<Return_type, Parameter...>* handle;
  public:

    function() : handle(nullptr) {}

    template <typename User_type>
    function(User_type&& functor) {
      handle = new __function<User_type, Return_type, Parameter...>(std::forward<User_type>(functor));
    }


    template <typename User_type>
    void operator=(User_type&& functor) {
      if(handle != nullptr)
        delete handle;
      handle = new __function<User_type, Return_type, Parameter...>(std::forward<User_type>(functor));
    }

    ~function() { delete handle; }

    Return_type operator()(Parameter ...args) {
      return handle->invoke(std::forward<Parameter>(args)...);
    }
};

}

#endif