#ifndef _FIB_HEAP_H_
#define _FIB_HEAP_H_

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <vector>
#include <unordered_set>
#include <cmath>
#include "../heap.h"

using ::std::make_pair;
using ::std::pair;

//#define fprintf(stderr,...)

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
			p->left  = left;
			p->right = this;
			p->parent= parent;
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
			parent = NULL;
		}

		void add_child(node *p)
		{
			if (child)
				child->insert(p);
			else
				child = p;
			p->parent = this;
			degree++;
			p->mark = false;
		}

		void print_debug() const
		{
			fprintf(stderr,"\t[%3d] %d", key, degree);
			if (this != right)
				fprintf(stderr,", l = %d, r = %d", left->key, right->key);
			if (parent)
				fprintf(stderr,", parent = %d", parent->key);
			if (child)
				fprintf(stderr,", child = %d", child->key);
			fprintf(stderr,"\n");
		}
	};

private:
	node *_min;
	node *_last;
	size_t A_size;
	node **A; // auxilliary array used in _consolidate()
	::std::unordered_set<node*> _node_list;

public:
	fib_heap():_min(NULL), _last(NULL), A(NULL), A_size(0) { }

	virtual ~fib_heap() { _free(); }

	virtual T &top() const { return _min->key; }

	virtual void pop()
	{
		if (!_min)
			return;
		node *z = _min;
		if (z->child) {
			node *x = z->child->right;
			while (x->right != x) {
				node *tmp = x->right;
				x->remove();
				z->insert(x);
				x = tmp;
			}
			x->remove();
			z->insert(x);
		}
		if (z == z->right)
			_min = NULL;
		else {
			_min = z->right;
			z->remove();
			_consolidate();
		}
		_node_list.erase(z);
		delete z;
	}

	virtual void push(const T &key)
	{
		auto *tmp = new fib_heap<T>::node(key);
		if (!_min)
			_min = tmp;
		else
			_min->insert(tmp);
		if (key < _min->key)
			_min = tmp;
		_last = tmp;
		_node_list.insert(tmp);
	}

	typename ::std::unordered_set<node*>::iterator begin() { return _node_list.begin(); }
	typename ::std::unordered_set<node*>::iterator end() { return _node_list.end(); }

	node *last() { return _last; }

	void decrease_key(node *x, const T &key)
	{
		if (x->key < key)
			return;
		node *z = x;
		x->key = key;
		node *y = x->parent;
		if (y)
			if (x->key < y->key)
				while (1) {
					x->remove();
					_min->insert(x);
					x = y;
					y = y->parent;
					if (!y)
						break;
					if (!x->mark) {
						x->mark = true;
						break;
					}
				}
		if (z->key < _min->key)
			_min = z;
	}

	// See Introduction to Algorithms 3rd Edition P523 for the max degree
	size_t degree() const 
	{ return (size_t) (::std::log(1.0 * _node_list.size()) / ::std::log(1.618) + 1.0E-6) + 1; }

	void print_debug() const
	{
		_min->print_debug();
		for( node *p = _min->right; p != _min; p = p->right )
			p->print_debug();
	}

private:
	void _free()
	{
		fprintf(stderr,"fib_heap::_free():  impelment me!\n");
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
		//fprintf(stderr,"maintenaince\n");
		node *w = _min;
		while (A[w->degree] != w) {
			//fprintf(stderr,"w = ");
			//w->print_debug();
			node *tmp = w->right;
			w->remove();
			node *x = w;
			int d = x->degree;
			while (A[d]) {
				node *y = A[d];
				//fprintf(stderr,"A[%d] = ", d);
				//y->print_debug();
				if (y->key < x->key)
					::std::swap(x,y);
				x->add_child(y);
				//fprintf(stderr,"x = ");
				//x->print_debug();
				A[d] = NULL;
				d++;
			}
			A[d] = x;
			w = tmp;
		}
		// Termination
		//fprintf(stderr,"termination\n");
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
