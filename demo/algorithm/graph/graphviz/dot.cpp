#include <gvc.h>

int main(const int argc, const char *argv[])
{
	GVC_t *gvc;
	Agraph_t *g;
	FILE *fp;
	gvc = gvContext();
	if (argc > 1)
		fp = fopen(argv[1], "r");
	else
		fp = stdin;
	g = agread(fp, 0);
	gvLayout(gvc, g, "dot");
	gvRender(gvc, g, "plain", stdout);
	gvFreeLayout(gvc, g);
	agclose(g);
	return (gvFreeContext(gvc));
}
