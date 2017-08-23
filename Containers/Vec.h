/***************************************************************************
*  @file       Vec.h
*  @author     Tom E
*  @date       15.08.2017
***************************************************************************/

#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <memory>
#include <utility>
#include <initializer_list>


template<typename T>
class Vec{
public:

	// element access
	T& operator[](size_t n) {
		return elements[n];
	}
	const T& operator[](size_t n) const {
		return elements[n]; 
	}

	//===============================================================================constructor | destructor
	Vec();

	// copy constructor
	Vec(const Vec &v) { 
		auto data = alloc_n_copy(v.begin(), v.end());
		elements = data.first;
		first_free = data.second;

	}

	// move constructor
	Vec(Vec &&v) noexcept;

	// copy assignment
	Vec& operator=(const Vec &v) {     
		auto data = alloc_n_copy(v.begin(), v.end());
		free();
		elements = data.first;
		first_free = data.second;
	}

	// move assignment
	Vec& operator=(Vec&& v) noexcept { 
		if (this != &v)
			free();
		elements = v.elements;
		first_free = v.first_free;
		cap = v.cap;

		v.elements = v.first_free = v.cap = nullptr;

		return *this;
	}

	~Vec()noexcept;

	//==========================push_back | pop_back
	void push_back(T && data);
	void push_back(T const& data);
	void pop_back();

	//==================free
	void free();

	//=======================================================reserve  | resize
	void resize(std::size_t n, const T &t = T());
	void reserve(size_t sz);

	//=================================================reallocate | alloc_n_copy
	void reallocate();
	std::pair<T*, T*> alloc_n_copy(T *b, T *e) {
		auto data = alloc.allocate(e - b);
		return { data, std::uninitialized_copy(b, e, data) };
	}
	
	//=========================================begin and end
	T* begin()const { return elements; }
	T* end()const { return first_free; }

	//====================================================size and capacity
	size_t size()const { return first_free - elements; }
	size_t capacity() { return cap - elements; }
	
	//=================================emplace_back
	template<typename... Args>
	void emplace_back(Args&&... args) {
		check();
		//alloc.construct(first_elem++, std::forward <Args>(args)...);
		alloc.construct(first_free++, std::forward<Args>(args)...);
	}
	Vec &operator=(std::initializer_list<T>);

private:
	void check();

	T* elements;   // pointer to first element in the array
	T* first_free; // pointer to first free element in the array
	T* cap;        // pointer to one past the end of the array

	static std::allocator<T> alloc;

};
template <typename T> std::allocator<T> Vec<T>::alloc;

template<typename T> inline Vec<T>::Vec() :elements(nullptr), first_free(nullptr), cap(nullptr) {
}
template<typename T> inline Vec<T>::Vec(Vec &&v) noexcept:
                 elements(v.elements),first_free(v.first_free),cap(v.cap){            // move constructor
	s.elements = s.first_free = s.cap = nullptr;
}
template<typename T> inline Vec<T>::~Vec()noexcept {
	free();
}
template<typename T> inline void Vec<T>::check(){
	if (size() == capacity())
		reallocate();
}
template<typename T> inline void Vec<T>::push_back(T && data) {
	check();
	alloc.construct(first_free++, std::move(data));
}
template<typename T> inline void Vec<T>::push_back(T const& data) {
	check();
	alloc.construct(first_free++, data);
}
template<typename T> inline void Vec<T>::pop_back(){
	if (size() >= 0)
		alloc.destroy(--first_free);
}
template<typename T> inline void Vec<T>::free() {
	if (elements) {

		for (auto i = first_free; i != elements; alloc.destroy(--i)) {}

		alloc.deallocate(elements, cap - elements);
		elements = elements = cap = nullptr;

	}
}
template<typename T> inline void Vec<T>::resize(std::size_t n, const T &t = T())
{
	if (n > size()) {
		for (int i = n - size(); i != 0; --i)
			push_back(t);
	}
	else if (n < size()) {
		for (int i = size() - n; i != 0; --i)
			pop_back();
	}
}
template<typename T> inline void Vec<T>::reserve(size_t sz) {
	if (sz >= capacity()) {
		reallocate_sz(sz);
	}
}
template <typename T> inline void Vec<T>::reallocate(){
	auto newcapacity = size() ? 2 * size() : 2;

	auto first = alloc.allocate(newcapacity);
	auto dest = first;
	auto elem = elements;

	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	free();

	elements = first;
	first_free = dest;
	cap = elements + newcapacity;
}

template <typename T> inline Vec<T> &Vec<T>::operator=(std::initializer_list<T> il)
{
	// copy allocates space and copies elements from the given range
	auto data = alloc_n_copy(il.begin(), il.end());

	free();   // destroy the elements in this object and free the space

	elements = data.first; // update data members to point to the new space
	first_free = cap = data.second;

	return *this;
}

#endif
