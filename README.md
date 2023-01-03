## veclist 使用vector<T*>类型的index管理空间

## shared_ptr使用一个表来进行计数管理，表类型为veclist,每个元素都包含指针和计数器

## shared_ptr可接收函数型和仿函数型删除器

### shared_ptr用法  
#### 1.使用仿函数
~~~c++
  class deleter {  
      public:  
          void operator()(int* ptr) {  
              delete[] ptr;  
              mystd::print("use deleter");  
          }  
  };  
~~~
##### 1、传临时对象  
    mystd::shared_ptr<int> a(new  int[10], deleter());  
#####  2、传另一个shared_ptr的删除器  
    mystd::shared_ptr<int> b(new int[10], a.get_deleter());  
##### 3、先构造，再指定删除器  
    mystd::shared_ptr<int> c(new int[10]);  
    c.setDeleter(deleter());  
##### 或者是  
    c.setDeleter(a.get_deleter());  
    
#### 2.使用函数  
~~~c++
  void deleter(int* ptr) {  
    delete[] ptr;  
  }  
~~~
##### 1.传函数地址  
    mystd::shared_ptr<int> a(new int[10], deleter);  
##### 2.传另一个shared_ptr的删除器  
    mystd::shared_ptr<int> b(new int[10], a.get_deleter_function());  


#### 3.使用lambda表达式  
##### 1.使用static_cast转换lambda表达式  
~~~c++
    mystd::shared_ptr<int> a(new int[10], static_cast<void(*)(int*)>(([](int*ptr) {  
        delete[] ptr;  
    })));  
~~~ 
#####  2.使用convert_lambda函数  
~~~c++
    mystd::shared_ptr<int> a(new int[10], a.convert_lambda([](int*ptr) {  
        delete[] ptr;  
    })); //本质上还是使用static_cast<void*(int*)>来转换  
~~~ 