#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct regword {
	int ch;
	int flag; // 0=normal '*'=zero or more
};

// match ch to reg_ch, 1=match 0=not
static int matchchar(const char ch, const int reg_ch)
{
	int res;
	switch (reg_ch) {
	case '.':
		res = ('a'<=ch && ch<='z') ? 1 : 0;
		break;
	default:
		res = ch==reg_ch;
		break;
	}
	return res;
}

static int match_regex(const int strlen, const char *restrict str,
		const int reglen, const char *restrict reg)
{
	assert(reglen>0);
	assert(strlen>=0);
	if (strlen==0) return 0;

	//printf("strlen = %d\n",strlen);
	//printf("reglen = %d\n",reglen);
	//printf("\n");

	int i=0, j=0;
	regword *regex=(regword*)malloc(sizeof(struct regword)*reglen);
	assert(regex);

	// first pass, construct regex
	while (i<reglen) {
		const int ch = reg[i++];
		if (ch=='*')
			regex[j-1].flag = '*';
		else {
			regex[j].ch     = ch;
			regex[j].flag   = 0;
			j++;
		}
	}

	// second pass, match string
	const int regexlen = j;
	//printf("regexlen = %d\n",regexlen);
	i=j=0;
	while (i<strlen) {
		const char    ch = str[i];
		const regword rw = regex[j];
		//printf("str[%d]=\'%c\' j=%d\n",i,ch,j);
		switch (rw.flag) {
		case 0:
			if (!matchchar(ch,rw.ch))
				return 0;
			else {
				i++;
				j++;
				//continue;
			}
			break;
		case '*':
			while (i<strlen)
				if (!matchchar(str[i++],rw.ch)) 
					break;
			i--;
			j++;
			break;
		default:
			fprintf(stderr,"invalid regex flag: %c\n",rw.flag);
			exit(1);
			break;
		}
	}

	//printf("\n");

	return j==regexlen;
}

int main(int argc, char const* argv[])
{
	const char str[]="aleeeeece";
	const char reg[] =".le*ce";
	const int strlen = sizeof(str)-1;
	const int reglen = sizeof(reg)-1;

	printf("str = \"%s\"\n",str);
	printf("reg = \"%s\"\n",reg);

	if (match_regex(strlen,str,reglen,reg))
		printf("\tmatch\n");
	else
		printf("\tno match\n");

	return 0;
}
