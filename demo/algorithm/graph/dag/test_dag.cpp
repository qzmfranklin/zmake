#include <stdlib.h>
#include <stdio.h>
#include "dag.h"

int main(int argc, char const* argv[])
{
	printf("sizeof(struct _st_dag_node) = %lu\n",sizeof(struct _st_dag_node));
	printf("sizeof(struct _st_dag) = %lu\n",sizeof(struct _st_dag));
	return 0;
}
