#include<iostream>
#include<sstream>

#ifndef _OTHER_CPP
#define _OTHER_CPP

namespace mystd {

template <typename T, typename T2>
struct is_same {
  constexpr static bool value = false;
};

template <typename T>
struct is_same<T, T> {
  constexpr static bool value = true;
};

template <typename T, typename T2 = T>
constexpr bool is_same_v = is_same<T, T2>::value;

class is_same_f {
  public:
    bool value;
    template <typename T1, typename T2>
    is_same_f(const T1&, const T2&) {
      value = is_same<T1, T2>::value;
    }

    operator bool() {
      return value;
    }

    friend std::iostream& operator<<(std::iostream& out, is_same_f& f) {
      out << f.value;
      return out;
    }
};


template <typename T>
void print_r(const T& value, int n) {
  if (n >= 0) {
    int i = 0;
    while (i++ < n)
      std::cout << value;
  }
}

template <typename ... Args>
void print(const Args&... args){
  ((std::cout << args << ' '), ...);
  std::cout << '\n';
}

template <typename ...Args>
void print_o(const Args& ...args) {
  ((std::cout << args), ...);
  std::cout << '\n';
}


template <typename P>
int array_size(P&array) {
  return sizeof(array)/sizeof(array[0]);
}//* 必须是array本身 如 int array【10】 传入参数应为array

template <typename T>
void swap(T& a, const T&& b) {
  a = b;
}

template <typename T>
void swap(const T&& a, T& b) {
  b = a;
}

template <typename T>
void swap(T& a, T& b) {
  if (a == b)
    return;
  T temp = a;
  a = b;
  b = a;
} //*重载赋值和拷贝构造

template <typename T>
void swap(T* first, T* last) {
  static T temp;
  if (first == last) {
    return;
  }
  temp = *first;
  *first = *last;
  *last = temp;
}

std::string int_to_string(size_t i) {
  std::ostringstream temp;
  temp<<i;
  return temp.str();
}

template <typename T>
void fill_n(T* first, T* last, const T& value) {
  while (first != last) {
    *first = value;
    ++first;
  }//* 后置++会产生临时对象 影响效率 
}

template <typename Iterator>
void copy(Iterator first, Iterator last, Iterator new_first) {
  while (first != last) {
    *new_first = *first;
    ++new_first;
    ++first;
  }//* 后置++会产生临时对象 影响效率 
}

template <typename T>
void reverse(T* first, T* last) {
  --last;
  while (first < last) {
    swap(first,last);
    ++first;
    --last;
  }//*左闭右开，于是先--last
}

template <typename Iterator, typename T>
Iterator find(Iterator first, Iterator last, const T& value) {
  while (first != last && *first != value) {
    ++first;
  }
  return first;
}

template <typename Iterator>
void show_array(Iterator first, Iterator last, std::string c = ", ") {
  if(first == last) {
    std::cout << "arrays is empty" << '\n';
    return;
  }
  std::cout << "{ ";
  while (first != last) {
    std::cout << *first <<c;
    ++first;
  }
  std::cout << "}";
  std::cout<<'\n';
}//*重载<<即可

}
#endif 