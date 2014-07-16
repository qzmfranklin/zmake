#ifndef _MESH_H_
#define _MESH_H_
/******************************************************************************/
#include <unistd.h>
/******************************************************************************/
struct st_mesh{
	int    status;	// internal status

	int    format;	// 1=ASCII, 2=BINARY
	int num_verts;	// #verts
	int num_trigs;	// #trigs
	int num_edges;	// #edges
	double     *v;	// verts -> coord
	int        *e;	// edges -> verts
	int        *t;	// trigs -> verts
	int       *te;	// trigs -> edges TODO
	double     *p;	// trigs -> coord
	double     *a;	// trigs -> areas
	double     *c;	// cntrs -> coord
	double     *n;	// nodes -> coord TODO
	char   fbase[FILENAME_MAX];	// file base name
};
/******************************************************************************/ 
#endif /* end of include guard */ 
