#include"./vector.cpp"
#ifndef _VECLIST_CPP
#define _VECLIST_CPP
namespace mystd{
	template <typename T>
	class veclist {
	public:
		typedef T		    value_type;
		typedef value_type* pointer;
		typedef value_type* iterator;
		typedef value_type& reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
	private:
		vector<T*> index;
		void realloc(size_type size) { index.realloc(size); }
	public:
		veclist() {}

		veclist(size_type size):index(size) {}
		
		template <typename ...Args>
		veclist(const Args& ...args) {
			size_type size = sizeof...(args);
			T temp[] = { args... };
			for (size_type i = 0; i < size; ++i)
				push_back(temp[i]);
		}
		
		veclist(veclist& V) {
			*this = V;
		}

		void operator=(veclist& V) {
			size_type size = V.size();
			if(capacity() > size) {
				clear();//使用原空间
				for(size_type i = 0;i < size;++i)
					push_back(V[i]);
			}
			else{
				realloc(V.capacity());//申请新空间
				for(size_type i = 0;i < size;++i)
					push_back(V[i]);
			}	
		}

		template <typename P>
		veclist(P& first) {*this = first;}

		template <typename P>
		void operator=(P& first) {
			if (index.size() != 0)
				clear();
			size_type size = arraySize(first);
			if (size > capacity())
				realloc(size * 2);
			for (size_type i = 0; i < size; ++i)
				push_back(*(first + i));
		}	

		~veclist() {
			size_type size = index.size();
			//std::cout<<std::endl;
			for(size_type i = 0;i<size && index[i]!=nullptr;++i) {
				//std::cout<<"delete "<<"array["<<i<<"] : "<<index[i]<<std::endl;
				delete index[i];
			}
			//std::cout<<std::endl;
		}

		void push_back(const T& value) {
			index.push_back(new T(value));
		}

		void push_back(T* pointer) {
			index.push_back(pointer);
		}

		void pop_back() {
			if (index.size() > 0) {
				index.pop_back();
			}
		}

		void erase(size_type n) {
			if (0 <= n && n < index.size()) {
				delete index[n];//不删除会造成覆盖，导致内存泄漏
				index.erase(n);
			}
		}

		void  erase(size_type first, size_type last) {
			if (0 <= first && last <= index.size()) {
				size_type count = last - first;
				for (size_type i = 0; i < count; ++i) {
				//	std::cout<<"erase array["<<first+i<<"]= "<<*index[first+i]
				//		 <<" : "<<index[first+i]<<std::endl; 
					delete index[first + i];
				}
				//std::cout<<std::endl;
				index.erase(index.begin()+first,index.begin()+last);
			}
		}//不删除会造成覆盖，导致内存泄漏

		void insert(size_type i, const T& value) {
			iterator temp = new T;
			*temp = value;
			index.insert(i, temp);
		}

		void insert(size_type first, size_type n, const T& value) {
			iterator temp[n];
			for (size_type i = 0; i < n; ++i) {
				temp[i] = new T;
				*temp[i] = value;
			}
			index.insert(first, n, temp);
		}

		void insert(size_type first,size_type n,iterator _first) {
			if(index.size()+n > capacity())
				index.keep_realloc((index.size()+n) * 2);
			iterator temp[n];
			for(size_type i = 0;i < n;++i) {
				temp[i] = new T;
				*temp[i] = _first[i];
			}
			index.insert(first,n,temp);
		}

		void clear() {
			size_type size = index.size();
			for(auto i = 0;i < size;++i) {
				delete index[i];
			}
			index.clear();
		}

		void show() {
			size_type size = index.size();
			for (size_type i = 0; i < size; ++i) {
				std::cout << "array[" << i << "]= "
						<< *index[i] <<" : "<<index[i]
						<< std::endl;
			}
			std::cout << std::endl;
		}

		reference front() { return *index.front(); }

		reference back() { return *index.back(); }

		size_type size() { return index.size(); }

		size_type capacity() { return index.capacity(); }

		size_type begin() {return 0;}

		size_type end() {return index.size();}

		reference operator[](size_type n) {
			return *index[n];
		}//*[]不该是扩容的操作

		reference at(size_type n) {
			return *(index.at(n));
		}

		template <typename P>
		size_type find(size_type first, size_type last,P& value) {
			for (; first != last; ++first)
				if (*index[first] == value)
					break;
			return first;
		}//* 可重载==运算符

	bool empty() { return index.empty(); }

	};
}
#endif