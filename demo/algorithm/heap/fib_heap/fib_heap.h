#ifndef _FIB_HEAP_H_
#define _FIB_HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include <cmath>
#include "../heap.h"

template<typename T>
class fib_heap: public heap<T> {
public:
	struct node {
		T key;
		bool mark;
		int degree;
		node *parent;
		node *child;
		node *left;
		node *right;
		node(const T _key): key(_key), mark(false), degree(0),
				parent(NULL), child(NULL) {}
		friend class fib_heap<T>;
	};

private:
	size_t _size;
	node *_min;
	node *_last;
	node **A; // auxilliary array used in _consolidate()
	size_t A_size;

public:
	fib_heap():_size(0), _min(NULL), _last(NULL), A(NULL), A_size(0) { }

	virtual ~fib_heap() { _free(); }

	virtual T &top() const { return _min->key; }

	virtual void pop()
	{
		if (!_min)
			return;
		node *z = _min;
		if (z->child) {
			for (node *x = z->child->left; x != z->child; x = x->left)
				_insert_left(z,x);
			_insert_left(z,z->child);
		}
		_remove_root(z);
		if (z == z->right)
			_min = NULL;
		else {
			_min = z->right;
			_consolidate();
		}
		delete z;
		_size--;
	}

	virtual void push(const T &key)
	{
		auto *tmp = new fib_heap<T>::node(key);
		if (!_min)
			_min = tmp;
		else
			_insert_left(_min,tmp);
		_size++;
		_last = tmp;
	}

	node *last() { return _last; }

	void decrease_key(node *p, const T key);

	// See Introduction to Algorithms 3rd Edition P523 for the max degree
	size_t degree() const 
	{ return (int) (::std::log(1.0*_size) / ::std::log(1.618) + 1.0E-6) + 1; }

private:
	void _free()
	{
	}

	void _alloc_A()
	{
		if (A_size >= this->degree())
			return;
		if (A)
			delete A;
		A_size = this->degree();
		A = (node**) malloc( sizeof(node*) * A_size );
		assert(A);
	}

	void _consolidate()
	{
		// at least one root node exists
		// Initialization
		_alloc_A();
		memset(A, 0, sizeof(node*) * A_size);
		// Maintenaince
		node *w = _min;
		while (1) {
			node *x = w;
			int d = x->degree;
			while (A[d]) {
				node *y = A[d];
				if (y->key < x->key)
					::std::swap(x,y);
				_attach_heap(x,y);
				A[d] = NULL;
				d++;
			}
			A[d] = x;
			w = w->right;
			if (A[w->degree] == w)
				break;
		}
		// Termination
		_min = NULL;
		for (int i = 0; i < A_size; i++) {
			if (!A[i])
				continue;
			if (!_min)
				_min = A[i];
			else {
				_insert_left(_min, A[i]);
				if (A[i]->key < _min->key)
					_min = A[i];
			}
		}
	}

	// x->key <= y->key, attach y to x
	void _attach_heap(node *x, node *y)
	{
		_remove_root(y);
		if (x->child)
			_insert_left(x->child, y);
		else {
			x->child = y;
			y->parent = x;
		}
		x->degree++;
		y->mark = false;
	}

	void _remove_root(node *p)
	{
		p->left->right = p->right;
		p->right->left = p->left;
	}

	// remove a node without children
	void _remove_leaf(node *p)
	{
		p->left->right = p->right;
		p->right->left = p->left;
		if (p->parent)
			if (p->parent->child == p)
				p->parent->child = (p == p->right)  ?  p->child  :  p->right;
	}

	// insert x to p's left
	void _insert_left(node *p, node *x)
	{
		node *q = p->left;
		q->right = x;
		p->left = x;
		x->left = q;
		x->right = p;
		x->parent = p->parent;
	}
};

#endif /* end of include guard */
