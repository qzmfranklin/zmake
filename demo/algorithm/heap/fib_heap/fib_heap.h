#ifndef _FIB_HEAP_H_
#define _FIB_HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <cmath>
#include "../heap.h"

//#define fprintf(stderr,...)

using ::std::make_pair;
using ::std::pair;

template<typename T>
class fib_heap: public heap<T> {
public:
	class node {
	public:
		T key;
		bool mark;
		int degree;
		node *parent;
		node *child;
		node *left;
		node *right;

		node(const T &_key): key(_key), mark(false), degree(0),
				parent(NULL), child(NULL), left(this), right(this) {}

		friend class fib_heap<T>;

		void insert(node *p)
		{
			p->left  = this->left;
			p->right = this;
			p->parent= this->parent;
			left->right = p;
			left = p;
		}

		// cut from parent a subtree rooted at this
		void remove()
		{
			if (parent)
				if (parent->child == this)
					parent->child = (this == right) ? NULL : right;
			right->left = left;
			left->right = right;
			left = this;
			right = this;
		}

		void add_child(node *p)
		{
			if (child)
				child->insert(p);
			else
				child = p;
			p->parent = this;
		}

		void print_debug() const
		{
			fprintf(stderr, "\t[%3d] degree = %d, left = %d, "
					"right = %d, parent = %d, child = %d\n",
					key,
					degree,
					left->key, 
					right->key,
					parent ? parent->key : 0,
					child ? child->key : 0);
		}
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
			node *x = z->child->left;
			while (x != z) {
				node *tmp = x->left;
				z->insert(x);
				x = tmp;
			}
			z->insert(z->child);
		}
		if (z == z->right)
			_min = NULL;
		else {
			_min = z->right;
			z->remove();
			_consolidate();
		}
		delete z;
		_size--;
	}

	virtual void push(const T &key)
	{
		fprintf(stderr,"\tpush(%d)\n",key);
		auto *tmp = new fib_heap<T>::node(key);
		if (!_min)
			_min = tmp;
		else
			_min->insert(tmp);
		if (key < _min->key)
			_min = tmp;
		_size++;
		_last = tmp;
	}

	node *last() { return _last; }

	void decrease_key(node *x, const T &key)
	{
		if (!(x->key < key))
			return;
		node *z = x;
		x->key = key;
		node *y = x->parent;
		if (y  &&  x->key < y->key) {
			while (1) {
				_cut_subtree(x);
				x = y;
				y = y->parent;
				if (!y)
					break;
				if (!x->mark) {
					x->mark = true;
					break;
				}
			}
		}
		if (z->key < _min->key)
			_min = z;
	}

	// See Introduction to Algorithms 3rd Edition P523 for the max degree
	size_t degree() const 
	{ return (size_t) (::std::log(1.0*_size) / ::std::log(1.618) + 1.0E-6) + 1; }

	void print_debug() const
	{
		_min->print_debug();
		for( node *p = _min->right; p != _min; p = p->right )
			p->print_debug();
	}

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
			//fprintf(stderr,"\nw = ");
			//w->print_debug();
			node *x = w;
			int d = x->degree;
			while (A[d]) {
				node *y = A[d];
				//fprintf(stderr,"y = ");
				//y->print_debug();
				if (y->key < x->key)
					::std::swap(x,y);
				x->add_child(y);
				// TODO: mark, degree
				x->degree++;
				//fprintf(stderr,"x = ");
				//x->print_debug();
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
			//A[i]->print_debug();
			if (!_min)
				_min = A[i];
			else {
				_min->insert(A[i]);
				if (A[i]->key < _min->key)
					_min = A[i];
			}
		}
	}
};

#endif /* end of include guard */
