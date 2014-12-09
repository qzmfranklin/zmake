#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

using ::std::vector;
using ::std::priority_queue;
using ::std::pair;
using ::std::make_pair;

class permute {
public:
	class node {
	public:
		::std::vector< pair<int,int> > edge_list;
		::std::vector< pair<int,int> > group_list;
	public:
		node(): edge_list(0) {}

		node(const node &rhs):
			edge_list(rhs.edge_list),
			group_list(rhs.group_list) {}

		void sort() { ::std::sort(group_list.begin(), group_list.end(), cmp); }

		void print() {
			if (!edge_list.empty())
				print_edge_list();
			if (!group_list.empty())             
				print_group_list();
		}
	private:
		static bool cmp(const pair<int,int> &lhs, const pair<int,int> &rhs)
		{ return lhs.first < rhs.first; }
		void print_edge_list() {
			for (auto &p : edge_list)
				printf("(%d,%d) ", p.first, p.second);
			printf("\n");
		}
		void print_group_list() {
			for (auto &p : group_list)
				printf("[%d,%d] ", p.first, p.second);
			printf("\n");
		}
	};
public:
	::std::stack<node*> _stack;
	node *_last;
	int _size;
public:
	permute(const int size): _last(NULL), _size(size) {
		assert(!(size & 1));
		node *tmp = new node;
		tmp->group_list.push_back(make_pair(0, _size - 1));
		_stack.push(tmp);
	}
	~permute() {}

	// must check !_stack.empty()
	void next() {
		node *p = _stack.top();
		_stack.pop();
		_add_branch_to_stack(p);
		delete p;
	}

	node *last() { return _last; };

	void test() {
		_stack.top()->group_list.push_back(make_pair(-21,32));
		_stack.top()->group_list.push_back(make_pair(21,32));
	}

private:
	void _add_branch_to_stack(node *p) {
		p->sort();
		auto &tmp = p->group_list;
		const int start = tmp[0].first;
		//TODO
		if (tmp.empty()) {
			_last = p;
			return;
		}
		for (int i = 0; i < tmp.size(); i++) {
		}
	}
};

int main(int argc, char const* argv[])
{
	permute g(8);
	//g.test();
	//g._stack.top()->sort();
	g._stack.top()->print();
	return 0;
}
