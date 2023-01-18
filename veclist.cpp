#include "./vector.cpp"
#ifndef _MY_VECLIST_CPP
#define _MY_VECLIST_CPP
namespace mystd {

    template <typename T>
    class veclist {
        public:
            typedef T value_type;
            typedef T& reference;
            typedef std::size_t size_type;
        private:
            vector<T*> index;

            void del() {
                T** temp = index.begin();
                T** last = index.end();
                while(temp != last) {
                    delete *temp++;
                }
                index.clear();
            }

            void unfold() {}
            template <typename ...Args>
            void unfold(const T& x,const Args& ...args) {
                push_back(new T(x));
                unfold(args...);
            }

        public:

            class iterator {
                typedef iterator&   iterator_reference;
                private:
                    T** _ptr;
                public:
                    iterator():_ptr(nullptr) {}

                    iterator(T** ptr):_ptr(ptr) {}

                    T** get() { return _ptr; }

                    T& operator*() { return **_ptr; }

                    T* operator->() { return *_ptr; }

                    iterator_reference operator++() { ++_ptr; return *this; }

                    iterator operator++(int) {
                        iterator temp(_ptr);
                        _ptr++;
                        return temp;
                    }

                    iterator_reference operator--() { --_ptr; return *this; }

                    iterator operator--(int) {
                        iterator temp(_ptr);
                        _ptr--;
                        return temp;
                    } 

                    bool operator==(const iterator& i) { return _ptr == i._ptr; }

                    bool operator!=(const iterator& i) { return _ptr != i._ptr; }

                    size_type subtraction(const iterator& i) { return _ptr - i._ptr; }

                    iterator operator+(size_type n) { return iterator(_ptr + n); }

                    iterator operator-(size_type n) { return iterator(_ptr - n); }

                    friend std::ostream& operator<<(std::ostream& out, iterator& it) {
                        out << it._ptr;
                        return out;
                    }
            };

            veclist() {}

            veclist(size_type size):index(size) {}

            veclist(veclist<T>&& v) {
                swap(v);
            }//*被编译器优化了

            veclist(T* first, T* last):index((size_type)(last - first) * 2) {
                T** temp = index.begin();
                index._add_finish(last - first);
                while(first != last) {
                    *temp++ = new T(*first++);
                }
            }

            veclist(iterator first, iterator last):index(last.subtraction(first) * 2) {
                index._add_finish(last.subtraction(first));//*改变index.end()
                T** temp1 = index.begin();
                T** temp2 = first.get();
                T** _last = index.end();
                while(temp1 != _last) {
                    *temp1++ = new T(**temp2++);
                }
            }

            veclist(veclist<T>& v):index(v.index.capacity()) {
                T** temp1 = index.begin();
                T** temp2 = v.index.begin();
                index._add_finish(v.index.size());
                T** last = v.index.end();
                while(temp2 != last) {
                    *temp1++ = new T(**temp2++);
                }
            }
            

            template <typename ...Args>
            veclist(const Args& ...args) {
                unfold(args...);
            }

            ~veclist() {
                del();
            }

            void push_back(const T& x) {
                index.push_back(new T(x));
            }

            void push_back(T* ptr) {
                if(ptr != nullptr) {
                    index.push_back(ptr);
                }
            }

            void pop_back() {
                if(index.size() != 0) {
                    delete index.back();
                    index.pop_back();
                }
            }

            size_type size() { return index.size(); }

            T& front() { return *index.front(); }

            T& back() { return *index.back(); }

            iterator begin() { return iterator(index.begin()); }

            iterator end() { return iterator(index.end()); }

            void operator=(veclist<T>& v) {
                del();    
                if(index.capacity() < v.index.capacity()) {
                    index.realloc(v.index.capacity());
                }
                else {
                    index.clear();
                }
                auto size = v.size();
                T** temp1 = index.begin();
                T** temp2 = v.index.begin();
                for(auto i = 0;i < size;++i) {
                    temp1[i] = new T(*temp2[i]);
                }
                index._add_finish(size);
            }

            void operator=(veclist<T>&& v) {
                del();
                index.swap(v.index);
            }

            T& operator[] (size_type i) { return *index[i]; }


            iterator erase(size_type first, size_type last) {
                if(first >= 0 && last <= index.size()) {
                    T** temp = index.begin();
                    for(auto i = first;i != last;++i) {
                        delete temp[i];
                    }
                    index.erase(temp + first,temp + last);
                    return iterator(index.begin() + first);
                }
                return this->last();
            }//* 推荐使用数字而不是迭代器

            iterator erase(iterator first, iterator last) {
                size_type size = last.subtraction(first);
                T** temp1 = first.get();
                T** _last = last.get();
                while(temp1 != _last) {
                    delete *temp1++;
                }
                index.erase(first.get(), last.get());
                return first;
            }

            iterator erase(size_type first) {
                return erase(first, first + 1);
            }//* 推荐使用数字而不是迭代器

            iterator erase(iterator first) {
               return erase(first, first + 1); 
            }

            void insert(size_type first, size_type size, const T& x) {
                T* temp[size];
                for(size_type i = 0;i < size;++i) {
                    temp[i] = new T(x);
                }
                index.insert(index.begin() + first, temp, temp + size);
            }//* 推荐使用数字而不是迭代器

            iterator insert(iterator first, size_type size, const T& x) {
                T* temp[size];
                for(size_type i = 0;i < size;++i) {
                    temp[i] = new T(x);
                }
                index.insert(first.get(), temp, temp + size);
            }
            
            void insert(size_type first, const T& x) {
                insert(first, 1, x);
            }

            iterator insert(iterator first, const T& x) {
                insert(first, 1, x);
            }

            void insert(size_type first, T* array_first, T* array_last) {
                size_type size = array_last - array_first;
                T* temp[size];
                for(size_type i = 0;i < size;++i) {
                    temp[i] = new T(array_first[i]);
                }
                index.insert(index.begin() + first, temp, temp + size);
            }//* 推荐使用数字而不是迭代器

            iterator insert(iterator first, T* array_first, T* array_last) {
                size_type size = array_last - array_first;
                T* temp[size];
                for(size_type i = 0;i < size;++i) {
                    temp[i] = new T(array_first[i]);
                }
                index.insert(first.get(), temp, temp + size);
                return first;
            }

            void swap(veclist<T>& v) { index.swap(v.index); }
            
            void swap(veclist<T>&& v) { index.swap(v.index); }

            template <typename X>
            iterator find(iterator first, iterator last, const X& x) {
                while(first != last) {
                    if(*first == x) {
                        break;
                    }
                    ++first;
                }
                return first;
            }

            T& at(size_type i) { return *index.at(i); }

            bool empty() { return index.empty(); }

            std::size_t get_number(iterator it) {
                return  index.get_number(it.get());                
            }
    };
}
#endif