#include <queue>
#include <vector>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * What is the largest number that cannot be expressed as a linear combination
 * of three different integers a, b, and c?
 * 	z = k1 * a + k2 * b + k3 * c
 *
 * 	k1, k2, k3 = 0, 1, 2, ...
 */

class Nuggets {
public:
	Nuggets(const uint8_t a, const uint8_t b, const uint8_t c)
	{
		assert(a <= b);
		assert(b <= c);
		_int3[0] = a;
		_int3[1] = b;
		_int3[2] = c;
		_queue3.resize(3);
		_queue3[0].push(_int3[0]);
		_queue3[1].push(_int3[1]);
		_queue3[2].push(_int3[2]);
	}

	uint64_t next()
	{
		uint64_t k[3];
		uint64_t k0 = -1;
		for (int i = 0; i < 3; i++) {
			k[i] = _queue3[i].front();
			if (k[i] < k0)
				k0 = k[i];
		}
		for (int i = 0; i < 3; i++) {
			_queue3[i].push(k0 + _int3[i]);
			if (k[i] == k0)
				_queue3[i].pop();
		}
		return k0;
	}

private:
	uint8_t _int3[3];
	std::vector<std::queue<uint64_t>> _queue3;
};

int main(int argc, char const* argv[])
{
	Nuggets g(6, 9, 20);
	const int N = 100000;
	std::vector<bool> mask;
	for (int i = 0; i < N; i++) {
		const uint64_t num = g.next();
		if (mask.size() <= num)
			mask.resize(num + 1);
		mask[num] = 1;
	}

	const size_t M = mask.size();
	for (int i = 0; i < M; i++)
		if (!mask[i])
			printf("%d\n", i);

	return 0;
}
