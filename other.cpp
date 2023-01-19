#include<iostream>
#include<sstream>
#ifndef _OTHER_CPP
#define _OTHER_CPP
namespace mystd {

template <typename T>
void print_r(const T& value, int n) {
  if (n >= 0) {
    int i = 0;
    while (i++ < n) {
      std::cout<<value;
    }
  }
}

void print() {
  std::cout<<'\n';
}

template <typename T,typename ...Args>
void print(T&& value,const Args&... args) {
  std::cout<<value<<' ';
  print(args...);
}

void print_o() {}

template <typename T,typename ...Args>
void print_o(T&& value,const Args&... args) {
  std::cout<<value;
  print_o(args...);
}


template <typename P>
int array_size(P&array) {
  return sizeof(array)/sizeof(array[0]);
}

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
void fill_n(T* firest, T* last, const T& value) {
  while (firest != last) {
    *firest++ = value;
  }
}

template <typename T>
void copy(T* first, T* last, T* new_first) {
  while (first != last) {
    *new_first++ = *first++;
  }
}

template <typename T>
void reverse(T* first, T* last) {
  --last;
  while (first < last) {
    swap(first++,last--);
  }
}

template <typename Iterator, typename T>
Iterator find(Iterator first, Iterator last, const T& value) {
  while (first != last && *first != value) {
    ++first;
  }
  return first;
}

template <typename Iterator> 
void show(Iterator first, Iterator last) {
  while (first != last) {
    std::cout << *first++ <<' ';
  }
  std::cout<<'\n';
}

}
#endif 