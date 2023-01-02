#include<iostream>
#include"./shared_ptr.cpp"
#ifndef _OTHER_CPP
#define _OTHER_CPP
namespace mystd {


    void outln() {std::cout<<'\n';}

    template <typename T,typename ...Args>
    void outln(T&& value,const Args&... args) {
        std::cout<<value<<' ';
        outln(args...);
    }

    void out() {std::cout<<'\n';}

    template <typename T,typename ...Args>
    void out(T&& value, const Args&... args) {
        std::cout<<value<<' ';
        out(args...);
    }

	template <typename P>
	int arraySize(P&array) {return sizeof(array)/sizeof(array[0]);}

    void filpOut(char* ptr) {
        int size = arraySize(ptr);
        for(int i = size;i != -1;--i)
            std::cout<<ptr[i];
        std::cout<<'\n';
    }

}
#endif