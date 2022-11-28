#include"./vector.cpp"
template <typename T>
class veclist {
public:
	typedef T		    value_type;
	typedef value_type* pointer;
	typedef value_type* __iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
private:
	vector<T*> index;
	void realloc(size_type size) { index.realloc(size); }
public:
	veclist() {}
	veclist(size_type size) { realloc(size); }
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
		if(capacity() > size){
			clear();
			for(size_type i = 0;i < size;++i)
				push_back(V[i]);
		}
		else{
			realloc(V.capacity());
			for(size_type i = 0;i < size;++i)
				push_back(V[i]);
		}	
	}
	template <typename P>
	veclist(P& first) {
		*this = first;
	}
	template <typename P>
	void operator=(P& first) {
		if (this->size() != 0)
			clear();
		size_type size = arraySize(first);
		if (size > capacity())
			realloc(size * 2);
		for (size_type i = 0; i < size; ++i)
			push_back(*(first + i));
	}
	~veclist() {
		size_type size = index.capacity();
		std::cout<<std::endl;
		for(size_type i = 0;i<size && index[i]!=nullptr;++i){
			std::cout<<"del "<<"array["<<i<<"]= "
					 <<*index[i]<<" : "<<index[i]<<std::endl;
			delete index[i];
		}
		std::cout<<std::endl;
	}
	size_type size() { return index.size(); }
	size_type capacity() { return index.capacity(); }
	reference operator[](size_type i) { return *index[i]; }
	reference at(size_type n){
		if (n < size()){
			return *(index[n]);}
		else{
			std::cout<<"-------------------error------------------------"<<std::endl;
			std::cout<<"Crossing the line in ____"<<n<<"____"<<std::endl;
			std::cout<<"-------------------error------------------------"<<std::endl;
			exit(-1);
		}
	}
	reference front() { return *index.front(); }
	reference back() { return *index.back(); }
	size_type find(size_type first, size_type last, const T& value) {
		size_type temp = first;
		for (; first != last; ++first)
			if (*index[first] == value)
				break;
		return first == this->size() ? temp : first;
	}
	void push_back(const T& value) {
		if (size() == capacity())
			index.redouble();
		if(index[index.size()] == nullptr){
			__iterator temp = new T;
			*temp = value;
			index.push_back(temp);
		}else{
			*index[index.size()] = value;
			index.add_finish();	
		}
	}
	void pop_back() {
		if (this->size() > 0) {
			index.pop_back();
		}
	}
	void erase(size_type n) {
		if (0 < n && n < this->size()) {
			delete index[n];//不删除会造成覆盖，导致内存泄漏
			index.erase(n);
		}
	}
	void  erase(size_type first, size_type last) {
		if (0 < first && last <= this->size()) {
			size_type count = last - first;
			for (size_type i = 0; i < count; ++i) {
				std::cout<<"del array["<<first+i<<"]= "<<*index[first+i]
				<<" :"<<index[first+i]<<std::endl; 
				delete index[first + i];
			}
			std::cout<<std::endl;
			index.erase(first, last);
		}
	}//不删除会造成覆盖，导致内存泄漏
	void insert(size_type i, const T& value) {
		__iterator temp = new T;
		*temp = value;
		index.insert(i, temp);
	}
	void insert(size_type first, size_type n, const T& value) {
		__iterator temp[n];
		for (size_type i = 0; i < n; ++i) {
			temp[i] = new T;
			*temp[i] = value;
		}
		index.insert(first, n, temp);
	}
	void insert(size_type first,size_type n,__iterator _first){
		if(size()+n > capacity())
			index.keep_realloc((size()+n) * 2);
		__iterator temp[n];
		for(size_type i = 0;i < n;++i){
			temp[i] = new T;
			*temp[i] = _first[i];
		}
		index.insert(first,n,temp);
	}
	void clear() { index.clear(); }
	void resize(size_type new_size) {
		size_type _size = this->size();
		if (new_size > _size) {
			index.keep_realloc(new_size);
		}
		if (new_size < _size) {
			for (; new_size < _size; ++new_size)
				delete index[new_size];
			index.setLocationSize(new_size);
		}
	}
	void show() {
		size_type size = this->size();
		for (size_type i = 0; i < size; ++i) {
			std::cout << "array[" << i << "]= "
			 		  << *index[i] <<" : "<<index[i]
					  << std::endl;
		}
		std::cout << std::endl;
	}
};
