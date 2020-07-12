#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"

/* read an annotated spelling list in the form
	word <tab> affixcode [ , affixcode ] ...
   print a reencoded version
	octal <tab> word
 */

typedef uint32_t Bits;
typedef	struct Dict Dict;
struct	Dict
{
	char*	word;
	Bits	encode;
};

Dict	words[200000];
char	space[500000];
Bits	encodes[4094];
long	nspace;
long	nwords;
int	ncodes;

void	readinput(FILE*);
int32_t	typecode(char*);
int	wcmp(const void*, const void*);
void	pdict(void);
void	sput(int);

int
main(int argc, char *argv[])
{
	FILE *f;

	setlocale(LC_ALL, "C");

	nwords = 0;
	nspace = 0;
	ncodes = 0;
	if (argc <= 1)
		readinput(stdin);
	while (argc > 1) {
		f = fopen(argv[1], "r");
		if (f == NULL) {
			fprintf(stderr, "Cannot open %s\n", argv[1]);
			exit(EXIT_FAILURE);
		}
		readinput(f);
		fclose(f);
		argc--;
		argv++;
	}
	fprintf(stderr, "words = %ld; space = %ld; codes = %d\n",
		nwords, nspace, ncodes);
	qsort(words, nwords, sizeof(words[0]), wcmp);
	pdict();
	return EXIT_SUCCESS;
}

int
wcmp(const void *a, const void *b)
{

	return strcmp(((Dict*)a)->word, ((Dict*)b)->word);
}

void
readinput(FILE* f)
{
	long i;
	char *code, *bword;
	char line[200];
	long lineno = 0;

	while (fgets(line, sizeof(line), f) != NULL) {
		line[strlen(line)-1] = '\0';
		lineno++;
		code = line;
		while (isspace(*code))
			code++;
		bword = code;
		while (*code && ! isspace(*code))
			code++;

		i = code - bword;
		memcpy(space + nspace, bword, i);
		words[nwords].word = space + nspace;
		nspace += i;
		space[nspace] = '\0';
		nspace++;

		if (*code) {
			*code++ = '\0';
			while (isspace(*code))
				code++;
		}
		words[nwords].encode = typecode(code);
		nwords++;
		if (nwords >= sizeof(words)/sizeof(words[0])) {
			fprintf(stderr, "words array too small\n");
			exit(EXIT_FAILURE);
		}
		if (nspace >= sizeof(space)/sizeof(space[0])) {
			fprintf(stderr, "space array too small\n");
			exit(EXIT_FAILURE);
		}
	}
}


typedef struct	Class
{
	char*	codename;
	int32_t	bits;
} Class;
Class	codea[]  =
{
	{ "a", ADJ },
	{ "adv", ADV },
	{ NULL, 0 },
};
Class	codec[] =
{
	{ "comp", COMP },
	{ NULL, 0 },
};
Class	coded[] =
{
	{ "d", DONT_TOUCH},
	{ NULL, 0 },
};

Class	codee[] =
{
	{ "ed",	ED },
	{ "er", ACTOR },
	{ NULL, 0 },
};

Class	codei[] =
{
	{ "in", IN },
	{ "ion", ION },
	{ NULL, 0 },
};

Class	codem[] =
{
	{ "man", MAN },
	{ "ms", MONO },
	{ NULL, 0 },
};

Class	coden[] =
{
	{ "n", NOUN },
	{ "na", N_AFFIX },
	{ "nopref", NOPREF },
	{ NULL, 0 },
};

Class	codep[] =
{
	{ "pc", PROP_COLLECT },
	{ NULL, 0 },
};
Class	codes[] =
{
	{ "s", STOP },
	{ NULL, 0 },
};

Class	codev[] =
{
	{ "v", VERB },
	{ "va", V_AFFIX },
	{ "vi", V_IRREG },
	{ NULL, 0 },
};

Class	codey[] =
{
	{ "y", _Y },
	{ NULL, 0 },
};

Class	codez[] =
{
	{ NULL, 0 },
};
Class*	codetab[] =
{
	codea,
	codez,
	codec,
	coded,
	codee,
	codez,
	codez,
	codez,
	codei,
	codez,
	codez,
	codez,
	codem,
	coden,
	codez,
	codep,
	codez,
	codez,
	codes,
	codez,
	codez,
	codev,
	codez,
	codez,
	codey,
	codez,
};

int32_t
typecode(char *str)
{
	Class *p;
	long code;
	int n, i;
	char *s, *sp, *st;

	code = 0;

loop:
	for (s = str; *s != '\0' && *s != ','; s++)
		;
	for (p = codetab[*str - 'a']; (sp = p->codename) != NULL; p++) {
		st = str;
		for (n = s - str; ; st++, sp++) {
			if (*st != *sp)
				goto cont;
			n--;
			if (n == 0)
				break;
		}
		code |= p->bits;
		if (*s == '\0')
			goto out;
		str = s + 1;
		goto loop;
	cont:
		;
	}
	fprintf(stderr, "Unknown affix code \"%s\"\n", str);
	return 0;
out:
	for (i = 0; i < ncodes; i++)
		if (encodes[i] == code)
			return i;
	encodes[i] = code;
	ncodes++;
	return i;
}

void
sput(int s)
{

	putchar((s>>8) & 0xff);
	putchar(s & 0xff);
}

void
lput(int32_t l)
{
	putchar((l>>24) & 0xff);
	putchar((l>>16) & 0xff);
	putchar((l>>8) & 0xff);
	putchar(l & 0xff);
}

/*
 * spit out the encoded dictionary
 * all numbers are encoded big-endian.
 *	struct
 *	{
 *		short	ncodes;
 *		int	encodes[ncodes];
 *		struct
 *		{
 *			short	encode;
 *			char	word[*];
 *		} words[*];
 *	};
 * 0x8000 flag for code word
 * 0x7800 count of number of common bytes with previous word
 * 0x07ff index into codes array for affixes
 */

void
pdict(void)
{
	long i, count;
	Bits encode;
	int j, c;
	char *lastword, *thisword, *word;

	sput(ncodes);
	for (i = 0; i < ncodes; i++)
		lput(encodes[i]);

	count = ncodes * 4 + 2;
	lastword = "";
	for (i = 0; i < nwords; i++) {
		word = words[i].word;
		thisword = word;
		for (j = 0; *thisword == *lastword; j++) {
			if (*thisword == '\0') {
				fprintf(stderr, "identical words: %s\n", word);
				break;
			}
			thisword++;
			lastword++;
		}
		if (j > 15)
			j = 15;
		encode = words[i].encode;
		c = (1 << 15) | (j << 11) | encode;
		sput(c);
		count += 2;
		for (thisword = word + j; (c = *thisword) != '\0'; thisword++) {
			putchar(c);
			count++;
		}
		lastword = word;
	}
	fprintf(stderr, "output bytes = %ld\n", count);
}
