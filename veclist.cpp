#include"./vector.cpp"
template <typename T>
class veclist{
public:
	typedef T		    value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
private:
	vector<T*> index;
	void realloc(size_type size){index.realloc(size);}
public:
	veclist(){}
	veclist(size_type size){realloc(size);}
	template <typename ...Args>
	veclist(const Args& ...args){
		size_type size = sizeof...(args);
		T temp[] = {args...};
		for(size_type i = 0;i < size;++i)
			push_back(temp[i]);
	}
	veclist(veclist& V){
		*this = V;
	}
	void operator=(veclist& V){
		if(this->size() != 0)
			clear();
		realloc(V.capacity());	
		size_type size = V.size();
		for(size_type i = 0;i < size;++i){
			push_back(V[i]);
		}
	}
	template <typename P>
	veclist(P& first){
		*this = first;
	}
	template <typename P>
	void operator=(P& first){
		if(this->size() != 0)
			clear();
		size_type size = sizeof(first)/sizeof(first[0]);
		if(size > capacity())
			realloc(size  * 2);
		for(size_type i = 0;i < size;++i)
			push_back(*(first+i));
	}
	~veclist(){clear();}
	size_type size(){return index.size();}
	size_type capacity(){return index.capacity();}
	reference operator[](size_type i ){return *index[i];}
	reference front(){return *index.front();}
	reference back(){return *index.back();}
	size_type find(size_type first,size_type last,const T& value){
		size_type temp = first;
		for(;first != last;++first)
			if(*index[first] == value)
				break;
		return first == this->size()?temp:first;
	}
	void push_back(const T& value){
		iterator temp = new T;
		*temp = value;
		index.push_back(temp);
	}
	void pop_back(){
		if(this->size() > 0){
			delete index.back();
			index.pop_back();
		}
	}
	void erase(size_type n){
		if(0 < n&&n < this->size()){
			delete index[n];
			index.erase(n);
		}
	}
	void  erase(size_type first,size_type last){
		if(0 < first && last <= this->size()){
			size_type count = last - first;
			for(int i = 0;i < count;++i){
				delete index[first+i];
			}	
			index.erase(first,last);
		}
	}
	void insert(size_type i,const T& value){
		iterator temp = new T;
		*temp = value;
		insert(i,temp);
	}
	void insert(size_type first,size_type n,const T& value){
		iterator temp[n];
		for(size_type i = 0;i < n;++i){
			temp[i] = new T;
			*temp[i] = value;
		}
		index.insert(first,n,temp);
	}
	void clear(){
		size_type size = this->size();
		for(size_type i = 0;i < size;++i)
			delete index[i];
		index.clear();
	}
	void show(){
		size_type size = this->size();
		for(size_type i = 0;i < size;++i){
			std::cout<<*index[i]<<" ";	
		}
		std::cout<<std::endl;
	}
};
