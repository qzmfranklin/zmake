#ifndef _MSH_IO_H_
#define _MSH_IO_H_
/******************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mesh.h"
/*******************************************************************************/
struct st_mesh *mshio_create_mesh(const char *fbase);
void mshio_destroy_mesh(struct st_mesh *q);

#define PRINT_INFO_BRIEF 0
#define PRINT_INFO_VERBOSE 1
void mshio_print_mesh  (struct st_mesh *q, int flag=0);
/******************************************************************************/
#endif /* end of include guard _MSH_IO_H_ */
