#ifndef LIST_H
#define LIST_H

template<typename T>
struct node {
	T value;
	node *next;
	node *prev;

	node(T v, node *p, node *n) :value(v), next(n), prev(p) {}
	node(T v) :value(v), next(NULL), prev(NULL) {}
	node() :value(T()), next(NULL), prev(NULL) {}
};

template<typename T>
class list {
private:
	node<T> *first;
	node<T> *last;
	std::size_t sz;
public:

	class iterator;
	
	//=========================================== constructor
	list() { first = last = NULL; sz = 0; }

	//=========================================== destructor
	~list() {
		node<T> *curr, *next = first;
		while (next) {
			curr = next;
			next = next->next;
			delete curr;
		}
	}

	//=========================================== push_front
	void push_front(T &&d) {
		node<T> *temp = new node<T>(d,NULL, first);
		if (!first) {
			last = temp;
		}
		first = temp;
		sz++;
	}
	void push_front(const T &d) {
		node<T> *temp = new node<T>(d, NULL, first);
		if (!first) {
			last = temp;
		}
		first = temp;
		sz++;
	}

	//=========================================== push_back
	void push_back(T &&d) {
		node<T> *temp = new node<T>(d);
		if (!last) {
			first = temp;
		}
		else{
			last->next = temp;
		}
		last = temp;
		sz++;
	}
	void push_back(const T &d) {
		node<T> *temp = new node<T>(d);
		if (!last) {
			first = temp;
		}
		else {
			last->next = temp;
		}
		last = temp;
		sz++;
	}

	void push_back(const T *&d) { // for point 
		node<T> *temp = new node<T>(*d);
		if (!last) {
			first = temp;
		}
		else {
			last->next = temp;
		}
		last = temp;
		sz++;
	}

	//=========================================== pop_front and pop_back
	void pop_front();
	void pop_back();

	//=========================================== begin and end
	iterator begin()const { 
		return iterator(first);
	}
	iterator end()const {
		return iterator(last);
	}
	
	//=========================================== size
	std::size_t size() const {
		return sz;
	}

	//=========================================== insert and erase
	
	iterator insert(iterator pos, T && data);
	iterator erase(iterator pos);

	//=========================================== back and front
	T& front() { return first; }
	T& back() { return last; }

	//=========================================== print
	void print() {
		node<T> *next = first;
		while (next) {
			std::cout << next->value << std::endl;
			next = next->next;
		}
	}
};

template<typename T> class list<T>::iterator{
private:
	node<T> *curr;
public:
	iterator(node<T> *tmp) :curr(tmp) {}

	iterator& operator++() { 
		curr = curr->next; 
		return *this; 
	}
	iterator& operator++(int) {
		iterator temp(curr);
		curr = curr->next;
		return temp;
	}

	iterator& operator--(int) {
		iterator temp(curr);
		curr = curr->prev; 
		return temp;
	}
	iterator& operator--() {
		curr = curr->prev;
		return *this;
	}

	T& operator*() { 
		return curr->value; 
	}
	T* operator->() {
		return &curr->value;
	}

	bool operator==(const iterator &b) {
		return curr == b.curr;
	}
	bool operator!=(const iterator &b) {
		return curr != b.curr;
	}
};


#endif
