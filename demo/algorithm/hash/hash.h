#ifndef _HASH_H_
#define _HASH_H_


struct hash {
	int status;
	int volume;
	int size;
	int *data;
};

struct hash *hash_create(const int vol);


#endif /* end of include guard */
