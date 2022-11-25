#include<iostream>
#define defaultSize 10 
template <typename T>
class vector{
public:
	typedef T		    value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
private:
	iterator start = nullptr;//当前使用的头
	iterator finish;//当前使用的尾
	iterator end_of_storage;//可用空间的尾
	void fill_initialize(size_type n,const T& value){
		start = new T[n]();
		for(size_type i = 0;i < n;++i){
			start[i] = value;
		}
		finish = start+n;
		end_of_storage = finish;
	}
	void realloc(){//保留相对位置
		size_type _size = size();
		size_type new_capacity =  capacity() * 2;
		iterator temp = new T[new_capacity]();
		_copy(begin(),end(),temp);
		if(start != nullptr)
			delete[] start;
		start = temp;
		finish = start + _size;
		end_of_storage = start +  new_capacity;
	}
public:
	void realloc(size_type size){//重置相对位置
			if(start != nullptr)
				delete[] start;
			start = new T[size]();
			finish = start;
			end_of_storage = start + size;
	}
	iterator begin(){return start;}
	iterator end(){return finish;}
	size_type size(){return size_type(end() - begin());}
	size_type capacity(){
		return size_type(end_of_storage - begin());}
	bool empty() const{return begin()==end();}
	reference operator[](size_type n){return *(begin() + n);}	
	vector(){
		start = new T[defaultSize]();
		finish = start;
		end_of_storage = start + defaultSize;
	}
	vector(size_type n,const T& value){fill_initialize(n,value);}
	vector(int n,const T& value){fill_initialize(n,value);}
	vector(long n,const T& value){fill_initialize(n,value);}
	explicit vector(size_type n){realloc(n);}
	template<typename ...Args>
	vector(const Args& ...args){
		size_type size = sizeof...(args);
		if(size > defaultSize)
			realloc(defaultSize);
		else
			realloc(size * 2);
		T temp[] = {args...};
		for(size_type i = 0;i < size;++i)
			push_back(temp[i]);
	}
	template <typename P>
	vector(P& first){
		*this = first;
	}
	vector(vector& V){
		*this = V;
	}
	void operator=(vector& V){
		realloc(V.capacity());
		copy(V.begin(),V.end(),begin());
		finish = start+V.size();
	}
	template <typename P>
	void operator=(P& first){
		size_type size = sizeof(first)/sizeof(first[0]);
		if(size < defaultSize)
			realloc(defaultSize);
		else
			realloc(size * 2);
		_copy(first,first+size,start);
		finish = start + size;
	}
	vector(iterator first,iterator last){
		size_type size = last - first;
		if(capacity() < size)
			realloc(size);
		_copy(first,last,start);
		finish += size;
	}
	~vector(){delete[] start;}
	reference front(){return *begin();}
	reference back(){return *(end() - 1);}
	iterator find(iterator first,iterator last,const T& value){
		for(;first != last;++first)
			if(*first == value)
				break;
		return first;
	}
	void push_back(const T& x){
		if(finish != end_of_storage){
			*(finish) = x;
			++finish;
		}else{
			realloc();
			*(finish) = x;
			++finish;
		}
	}
	void pop_back(){--finish;}
	iterator copy(iterator first ,iterator last,iterator new_first){//用于同一数组间
		if(new_first == first)
			return new_first;
		if(new_first < first){
			iterator temp1 = new_first;
			iterator temp2 = first;
			for(;temp2 < last;++temp1,++temp2)
				*(temp1) = *(temp2);
		}else{
			iterator temp1 = new_first+(last-first)-1;
			iterator temp2 = last -1;
			for(;temp2>=first;--temp1,--temp2)
				*(temp1) = *(temp2);
		}
		return new_first;
	}
	void _copy(iterator first,iterator last, iterator new_first){//用于不同数组间
		for(;first!=last;++first,++new_first)
			*new_first = *first;
	}
	iterator erase(iterator position){
		if(position + 1 != end())
			copy(position + 1,end(),position);
		--finish;
		return position; 
	}
	iterator erase(iterator first,iterator last){
		copy(last,end(),first);
		finish = finish - (last - first);
		return first;
	}
	iterator erase(size_type i){return erase(start + i);}
	iterator erase(size_type i,size_type n){return erase(start + i,start + n);}
	void clear(){erase(begin(),end());}
	iterator insert(iterator position,const T& value){
		copy(position,end(),position+1);
		*position = value;
		finish += 1;
		return position;
	}
	iterator insert(iterator position,size_type n,const T& value){
		if(position == end()){
			iterator temp;
			if(end()+n <= end_of_storage){
				temp = position;
				for(int i = 0;i < n;++i)
					*(temp++) = value;
			}else{
				realloc();
				temp = finish;
				for(int i = 0;i < n;++i)
					*(temp++) = value;
			}
			finish += n;
			return temp;	
		}
		if(end() + n <= end_of_storage){
			copy(position,end(),position+n);
			iterator temp = position;
			for(;temp < position + n;++temp)
				*(temp) = value;
			finish += n;
			return temp;
		}else{
			size_type _size = end() - position;
			while(capacity() < size() + n)
				realloc();
			position = end() - _size;
			copy(position,end(),position+n);
			iterator temp = position;
			for(;temp < position + n;++temp)
				*(temp) = value;
			finish += n;
			return temp;
		}
	}
	iterator insert(iterator first,size_type n,const iterator _first){
		if(end()+n > end_of_storage){
			if(capacity()*2 < size()+n)
				return first;
			size_type _size = end() - first;
			realloc();
			first = end() - _size;
		}
		copy(first,end(),first+n);
		for(size_type i = 0;i < n;++i)
			*(first+i) = *(_first+i);
		finish += n;
		return first;
	}
	iterator insert(size_type first,const T& value){return insert(start+first,value);}
	iterator insert(size_type first,size_type n,const T& value){return insert(start+first,n,value);}
	iterator insert(size_type first,size_type n,const iterator _first){return insert(start+first,n,_first);}
	void resize(size_type new_size,const T& x){
		if(new_size < size())
			erase(begin()+new_size,end());
		else
			insert(end(),new_size - size(),x);		
	}
	void show(){
		size_type size = this->size();
		for(size_type i = 0;i < size;++i){
			std::cout<<*(start+i)<<" ";
		}
		std::cout<<std::endl;
	}
};
