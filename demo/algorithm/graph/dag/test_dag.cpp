#include <stdlib.h>
#include <stdio.h>
#include "dag.h"

int main(int argc, char const* argv[])
{
	dag_node node;
	dag g;
	printf("sizeof(node) = %lu\n",sizeof(dag_node));
	printf("sizeof(dag)  = %lu\n",sizeof(dag));
	return 0;
}
