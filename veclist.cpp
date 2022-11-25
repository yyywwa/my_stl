#include"./vector.cpp"
template <typename T>
class veclist{
private:
	vector<T*> index;
	void realloc(){index.realloc();}
public:
	typedef T		    value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
public:
	veclist(){}
	veclist(size_type size){index.realloc(size);}
	template <typename ...Args>
	veclist(const Args& ...args){
		size_type size = sizeof...(args);
		T temp[] = {args...};
		for(size_type i = 0;i < size;++i)
			push_back(temp[i]);
	}
	size_type size(){return index.size();}
	~veclist(){
		size_type count = size();
		for(size_type i = 0;i < count;++i){
			delete[] index[i];
		}
	}
	reference operator[](size_type i ){return *index[i];}
	reference front(){return *index.front();}
	reference back(){return *index.back();}
	size_type find(size_type first,size_type last,const T& value){
		size_type temp = first;
		for(;first != last;++first)
			if(*index[first] == value)
				break;
		return first == size()?temp:first;
	}
	void push_back(const T& value){
		iterator temp = new T;
		*temp = value;
		index.push_back(temp);
	}
	void pop_back(){
		if(size() > 0){
			delete index.back();
			index.pop_back();
		}
	}
	void erase(size_type n){
		if(0 < n&&n < size()){
			delete index[n];
			index.erase(n);
		}
	}
	void  erase(size_type first,size_type last){
		if(0 < first && last <= size()){
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
};
