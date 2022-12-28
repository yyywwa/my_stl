#include<iostream>
#ifndef _OTHER_CPP
#define _OTHER_CPP
namespace mystd {
    template <typename T>
    void outln(const T& value) {
        std::cout<<value<<'\n';
    }

    void outln() {
        std::cout<<'\n';
    }
}
#endif