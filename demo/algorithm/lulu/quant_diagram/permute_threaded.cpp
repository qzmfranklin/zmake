#include <cstdio>
#include <cstdlib>
#include <assert.h>
#include <algorithm>
#include <list>
#include <stack>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

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
private:
	int _size;
	size_t _count;
public:
	permute(const int size): _size(size), _count(0)
	{ assert( size % 2 == 0   &&   size < 24); }

	~permute() {}

	bool empty() const { return _queue.empty(); }

	void run(const int nthread)
	{
		::std::list<node*> task_list;
		_init_task_list(&task_list);

		::std::mutex q_mtx;
		::std::condition_variable q_cond;
		auto *worker_list = new ::std::thread[n];
		for (int i = 0; i < n; i++)
			worker_list[i] = ::std::thread(_run_worker_function,
					&q_mtx, &q_cond, &_queue);

		while (1) {
		}

		for (int i = 0; i < n; i++)
			worker_list[i].join();
		delete []worker_list;
	}

private:
	void _init_task_list(::std::list<node*> *task_list)
	{
		{
		node *tmp = new node;
		tmp->group_list.push_back(make_pair(0, _size - 1));
		task_list->push_back(tmp);
		}
		const int TASK_LIST_MIN = 100;
		while (1) {
		}
	}

	void _add_to_task_list(::std::list<node*> *task_list)
	{
		assert(q->size());
		node *p = q->front();
		q->pop();

		auto &gl = p->group_list;

		// acceptable state
		if (gl.empty()) {
			p->print();
			delete p;
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
			q->push(tmp);
		}
		// edge case of the first group
		if (end0 - start0 > 1) {
			auto *tmp = new node;
			tmp->edge_list  = p->edge_list;
			tmp->edge_list.push_back(make_pair(start0, end0));
			tmp->group_list = p->group_list;
			tmp->group_list[0] = make_pair(start0+1, end0-1);
			q->push(tmp);
		}

		// branch the remaining groups, if any
		if (gl.size() == 1) {
			delete p;
			return;
		}
		for (int i = 1; i < gl.size(); i++) {
			const int start = gl[i].first;
			const int end = gl[i].second;
			if (end - start == 0) {
				auto *tmp = new node;
				tmp->edge_list = p->edge_list;
				tmp->edge_list.push_back(make_pair(start0, start));
				tmp->group_list.reserve(gl.size() + 1);
				if (end0 - start0)
					tmp->group_list.push_back(make_pair(start0+1, end0));
				for (int k = 1; k < i; k++)
					tmp->group_list.push_back(gl[k]);
				for (int k = i+1; k < gl.size(); k++)
					tmp->group_list.push_back(gl[k]);
				q->push(tmp);
				continue;
			}
			// end > start
			for (int j = start; j <= end; j++) {
				auto *tmp = new node;
				tmp->edge_list = p->edge_list;
				tmp->edge_list.push_back(make_pair(start0, j));
				tmp->group_list.reserve(gl.size() + 1);
				if (end0 - start0)
					tmp->group_list.push_back(make_pair(start0+1, end0));
				for (int k = 1; k < i; k++)
					tmp->group_list.push_back(gl[k]);

				if (j == start) {
					tmp->group_list.push_back(make_pair(start+1, end));
				} else if (j == end) {
					tmp->group_list.push_back(make_pair(start, end-1));
				} else {
					tmp->group_list.push_back(make_pair(start, j-1));
					tmp->group_list.push_back(make_pair(j+1, end));
				}

				for (int k = i+1; k < gl.size(); k++)
					tmp->group_list.push_back(gl[k]);
				q->push(tmp);
			}
		}
		delete p;
	}

	void _next_stack(::std::stack<node*> *s)
	{
		assert(s->size());
		node *p = s->top();
		s->pop();

		auto &gl = p->group_list;

		// acceptable state
		if (gl.empty()) {
			p->print();
			delete p;
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
			s->push(tmp);
		}
		// edge case of the first group
		if (end0 - start0 > 1) {
			auto *tmp = new node;
			tmp->edge_list  = p->edge_list;
			tmp->edge_list.push_back(make_pair(start0, end0));
			tmp->group_list = p->group_list;
			tmp->group_list[0] = make_pair(start0+1, end0-1);
			s->push(tmp);
		}

		// branch the remaining groups, if any
		if (gl.size() == 1) {
			delete p;
			return;
		}
		for (int i = 1; i < gl.size(); i++) {
			const int start = gl[i].first;
			const int end = gl[i].second;
			if (end - start == 0) {
				auto *tmp = new node;
				tmp->edge_list = p->edge_list;
				tmp->edge_list.push_back(make_pair(start0, start));
				tmp->group_list.reserve(gl.size() + 1);
				if (end0 - start0)
					tmp->group_list.push_back(make_pair(start0+1, end0));
				for (int k = 1; k < i; k++)
					tmp->group_list.push_back(gl[k]);
				for (int k = i+1; k < gl.size(); k++)
					tmp->group_list.push_back(gl[k]);
				s->push(tmp);
				continue;
			}
			// end > start
			for (int j = start; j <= end; j++) {
				auto *tmp = new node;
				tmp->edge_list = p->edge_list;
				tmp->edge_list.push_back(make_pair(start0, j));
				tmp->group_list.reserve(gl.size() + 1);
				if (end0 - start0)
					tmp->group_list.push_back(make_pair(start0+1, end0));
				for (int k = 1; k < i; k++)
					tmp->group_list.push_back(gl[k]);

				if (j == start) {
					tmp->group_list.push_back(make_pair(start+1, end));
				} else if (j == end) {
					tmp->group_list.push_back(make_pair(start, end-1));
				} else {
					tmp->group_list.push_back(make_pair(start, j-1));
					tmp->group_list.push_back(make_pair(j+1, end));
				}

				for (int k = i+1; k < gl.size(); k++)
					tmp->group_list.push_back(gl[k]);
				s->push(tmp);
			}
		}
		delete p;
	}
};

int main(int argc, char const* argv[])
{
	if (argc < 3) {
		fprintf(stderr,"Usage: permute.exe [size] [nthread]\n");
		exit(1);
	}
	const int size = ::std::atoi(argv[1]);
	const int nthread = ::std::atoi(argv[2]);
	permute g(size);
	g.run(nthread);
	return 0;
}
