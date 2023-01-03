#include<iostream>
#include"./shared_ptr.cpp"
#ifndef _OTHER_CPP
#define _OTHER_CPP
namespace mystd {


    void print() {std::cout<<'\n';}

    template <typename T,typename ...Args>
    void print(T&& value,const Args&... args) {
        std::cout<<value<<' ';
        print(args...);
    }


	template <typename P>
	int array_size(P&array) {return sizeof(array)/sizeof(array[0]);}

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
}
#endif