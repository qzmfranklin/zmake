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
		node(): edge_list(0), group_list(0) {}

		void sort() { ::std::sort(group_list.begin(), group_list.end(), cmp); }

		void print()
		{
			if (!edge_list.empty()) {
				for (auto &p : edge_list)
					printf("(%d,%d) ", p.first, p.second);
				printf("\n");
			}
			if (!group_list.empty()) {
				for (auto &p : group_list)
					printf("[%d,%d] ", p.first, p.second);
				printf("\n");
			}
		}

		void print_debug()
		{
			if (!edge_list.empty()) {
				fprintf(stderr,"\t");
				for (auto &p : edge_list)
					fprintf(stderr,"(%d,%d) ", p.first, p.second);
				fprintf(stderr,"\t");
				fprintf(stderr,"\n");
			}
			if (!group_list.empty()) {
				fprintf(stderr,"\t");
				for (auto &p : group_list)
					fprintf(stderr,"[%d,%d] ", p.first, p.second);
				fprintf(stderr,"\t");
				fprintf(stderr,"\n");
			}
		}
	private:
		static bool cmp(const pair<int,int> &lhs, const pair<int,int> &rhs)
		{ return lhs.first < rhs.first; }
	};
public:
	::std::stack<node*> _stack;
	int _size;
public:
	permute(const int size): _size(size)
	{
		assert( size % 2 == 0 );
		node *tmp = new node;
		tmp->group_list.push_back(make_pair(0, _size - 1));
		_stack.push(tmp);
	}
	~permute() {}

	bool empty() const { return _stack.empty(); }

	void next()
	{
		//fprintf(stderr,"next()\n");
		assert(_stack.size());
		node *p = _stack.top();
		//p->print_debug();
		_stack.pop();

		auto &gl = p->group_list;

		// acceptable state
		if (gl.empty()) {
			p->print();
			return;
		}

		// branch the first group
		const int start0 = gl[0].first;
		const int end0 = gl[0].second;
		for (int i = start0 + 2; i < end0; i++) {
			auto *tmp = new node;
			tmp->edge_list = p->edge_list;
			tmp->edge_list.push_back(make_pair(start0, i));
			tmp->group_list.reserve(gl.size() + 1);
			tmp->group_list.push_back(make_pair(start0+1, i-1));
			tmp->group_list.push_back(make_pair(i+1, end0));
			for (int j = 1; j < gl.size(); j++)
				tmp->group_list.push_back(gl[j]);
			_stack.push(tmp);
		}
		// edge case of the first group
		if (end0 - start0 > 1) {
			auto *tmp = new node;
			tmp->edge_list  = p->edge_list;
			tmp->edge_list.push_back(make_pair(start0, end0));
			tmp->group_list = p->group_list;
			tmp->group_list[0] = make_pair(start0+1, end0-1);
			_stack.push(tmp);
		} else if (end0 - start0 == 1) {
			auto *tmp = new node;
			tmp->edge_list  = p->edge_list;
			tmp->edge_list.push_back(make_pair(start0, end0));
			for (int i = 1; i < gl.size(); i++)
				tmp->group_list.push_back(gl[i]);
			_stack.push(tmp);
		}

		// branch the remaining groups, if any
		if (gl.size() == 1)
			return;
		for (int i = 1; i < gl.size(); i++) {
			const int start = gl[i].first;
			const int end = gl[i].second;
			for (int j = start; j <= end; j++) {
				auto *tmp = new node;
				if (end0 - start0)
					tmp->group_list.push_back(make_pair(start0+1, end0));
				for (int k = 1; k < i; k++)
					tmp->group_list.push_back(gl[k]);

				/* 
				 * If performance becomes the issue, manually
				 * unroll the following if statements
				 */
				if (j == start)
					tmp->group_list.push_back(make_pair(start+1, end));
				else if (j == end)
					tmp->group_list.push_back(make_pair(start, end-1));
				else {
					tmp->group_list.push_back(make_pair(start, j-1));
					tmp->group_list.push_back(make_pair(j+1, end));
				}

				for (int k = i+1; k < gl.size(); k++)
					tmp->group_list.push_back(gl[k]);
			}
		}
	}
};

int main(int argc, char const* argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: permute.exe [size]\n");
		exit(1);
	}
	permute g(::std::atoi(argv[1]));
	while (!g.empty())
		g.next();
	return 0;
}
