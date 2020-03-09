/// @file skip_list.cpp
/// @brief wait for check
/// @author zhaolu
/// @version 1.0
/// @date 2020-03-07
/// @copyright Copyright (c) 2020 zhaolu. All Rights Reserved.

#include <string>
#include <random>
#include <iostream>

template<typename T>
class node {
	private:
		unsigned random_level() const {
			std::random_device rd; 
			std::mt19937 generator(rd());
		   	unsigned level = 1;

		   	for (int i = 1; i < 16; ++i) {
		    	if (generator() % 2 == 1) {
		    		++level;
		    	} 
			}
		    	
		    return level;
		}
	public:
		node(T value) : _value(value) {
			_level = random_level();
			_nxt = (node<T>**)malloc(sizeof(node<T>*) * _level);
			memset(_nxt, 0x00, sizeof(node<T>*) * _level);
		}
		
		virtual ~node() {
			free(_nxt);
		}

		node<T>** at(unsigned level) const {
			return (level >= _level) ? nullptr : &_nxt[level];
		}

		T _value;
		const unsigned _level;
		node<T>** _nxt;
};

template<typename T>
class skip_list {
public:
	static const unsigned MAX_LEVEL = 16;
private:
	unsigned _size;
	node<T>* _head;
	
public:
	skip_list() : _size(0), _head(new node<T>(-1)) {}

	node<T>* find_last_lower(T value) const {
		if (_head == nullptr || _head->_value >= value)
			return nullptr;

		node<T>* p = _head;
		for (int i = MAX_LEVEL - 1; i >= 0; --i) {
			while (p->at(i) != nullptr && *(p->at(i))->_value < value)
				p = *(p->at(i));
		}

		return p;
	}

	bool contains(T value) const {
		node<T>* p = find_last_lower(value);
		return p != nullptr && *(p->at(0))->_value == value;
	}
	
	node<T>* insert(T value) {
		++_size;
		node<T>* new_node = new node<T>(value);
		node<T>* p = _head;

		node<T>** update_node = new node<T>[new_node->level];
		for(int i = new_node->_level - 1; i >= 0; --i) {
			while (p->at(i) != nullptr && *(p->at(i))->_value < value)
				p = *(p->at(i));
			update_node[i] = p;
		}

		for(int i = new_node->_level - 1; i >= 0; --i) {		
			*(new_node->at(i)) = *(update_node[i].at(i));
			*(update_node[i].at(i)) = new_node;
		}

		delete[] update_node;
	}

	int erase(T value) {
		node<T>* delete_node = new node<T>[MAX_LEVEL];
		node<T>* p = _head;
		for(int i = MAX_LEVEL - 1; i >= 0; --i) {
			while (p->at(i) != nullptr && *(p->at(i))->_value < value)
				p = *(p->at(i));
			delete_node[i] = p;
		}

		if (p->at(0) == nullptr || *(p->at(0))->_value != value)
			return -1;

		for(int i = MAX_LEVEL - 1; i >= 0; --i) {
			if (delete_node[i] != nullptr && *(delete_node[i].at(i))->_value == value)
				*(delete_node[i].at(i)) = *(*(delete_node[i].at(i)).at(i));
		}

		delete[] delete_node;
	}

	virtual ~skip_list() {
		node<T>* p = _head;
		while (p != nullptr) {
			node<T>* nxt = *(p->at(0));
			delete p;
			p = nxt;
		}
	}
};

