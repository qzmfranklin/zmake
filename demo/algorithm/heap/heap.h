#ifndef _HEAP_H_
#define _HEAP_H_

template<typename T>
class heap {
public:
	virtual T top() const = 0;

	virtual void pop() = 0;

	virtual void push(const T val) = 0;
};

#endif /* end of include guard */
