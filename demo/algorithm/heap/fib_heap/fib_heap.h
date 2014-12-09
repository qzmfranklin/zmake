#ifndef _FIB_HEAP_H_
#define _FIB_HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include "../heap.h"

template<typename T>
class fib_heap: public heap<T> {
public:
	class node {
		T _key;
		int _status;
		int _degree;
		node *_parent;
		node *_child;
		node *_left;
		node *_right;
	public:
		node(const T key): _key(key), _status(WHITE), _degree(0),
				_parent(NULL), _child(NULL) {}
		enum { WHITE = 0, BLACK = 1 };
		friend class fib_heap<T>;
	};

private:
	node *_min;
	node *_last;

public:
	fib_heap(const int n) { }

	virtual ~fib_heap() { }

	virtual T top() const { return _min->_key; }

	/*
	 * Extract_min in O(lgN) time
	 */
	virtual void pop();

	/*
	 * Insert a new node with key in O(1) time
	 * Retrieve the pointer to the newly inserted node by calling last()
	 */
	virtual void push(const T key);

	/*
	 * Retrieve the last inserted node
	 */
	node *last() { return _last; }

	/*
	 * Decrease the key of a node in O(1) amortized time
	 */
	void decrease_key(node *p, const T key);

private:
	void free();
};

#endif /* end of include guard */
