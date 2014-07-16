#include <stdio.h>
#include "utils.h"
#include "mshio.h"
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{
	if (argc>1) link_stdout(argv[1]);
	//test01(); 
	if (argc>1) unlink_stdout(); 

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test load_mesh()\n");

	char *fbase="msh/square162"; 
	struct st_mesh *q;
	q=mshio_create_mesh(fbase);
	mshio_print_mesh(q,PRINT_INFO_VERBOSE);
	
	mshio_destroy_mesh(q);
        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
