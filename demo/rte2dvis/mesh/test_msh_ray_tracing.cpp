#include "msh_ray_tracing.h"
#include "mshio.h"
#include "utils.h"
#include <vector>
/******************************************************************************/
int test01(void);
/******************************************************************************/
int main(int argc, char const* argv[])
{ 
	if (argc>1) link_stdout(argv[1]); 
	if (argc>1) unlink_stdout(); 
	test01();

	return 0;
}

int test01(void)
{
	int err=0; 
        printf("TEST01\n");
        printf("	|Test meshtree_new\n");

	char *fbase="msh/square162"; 
	struct st_mesh *q = load_mesh(fbase);
			//	xmin, ymin, xmax, ymax
	const double limits[]={	0.0,  0.0,  1.0,  1.0}; 
	const int npl=3; // number of packed levels
	struct st_meshtree *t;
	t=create_meshtree(q->num_trigs,q->trigs,limits,npl);

	destroy_meshtree(t);
	destroy_mesh(q);

        printf("END OF TEST01\n");
        printf("\n");
	return err;
}
