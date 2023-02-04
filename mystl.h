#ifndef _MY_STL_H
#define _MY_STL_H
#include "./array.cpp"
#include "./vector.cpp"
#include "./veclist.cpp"
#include "./shared_ptr.cpp"
#include "./other.cpp"
#include "./function.cpp"
#include "./list.cpp"
#include "./mystl_iterator.cpp"
//*移动构造函数被优化，直接将右值变为左值，变量名变为左值引用（应该）;
//*lambda 可以拷贝构造出另一个lumbda giao；
#endif