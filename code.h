#include <string.h>

/*
 * affix codes
 */

#define	ED		(1<<0)	/* +ed, +ing */
#define	ADJ		(1<<1)	/* (nce)-t_ce, +ize,+al, +ness, -t+cy, +ity, +ly */	
#define	NOUN		(1<<2)	/* +s (+es), +make, +hood, +ship +less  */
#define PROP_COLLECT	(1<<3)	/* +'s,  +an, +ship(for -manship) +less */
#define ACTOR		(1<<4)	/* +er  */
#define	EST		(1<<5)
#define COMP		(EST|ACTOR)	/* +er,+est */
#define	DONT_TOUCH	(1<<6)
#define	ION		(1<<7)	/* +ion, +or */
#define	N_AFFIX		(1<<8) 	/* +ic, +ive, +ize, +like, +al, +ful, +ism, +ist, -t+cy, +c (maniac) */
#define	V_AFFIX		(1<<9)	/* +able, +ive, +ity((bility), +ment */
#define	V_IRREG		(1<<10)	/* +ing +es +s*/
#define	VERB		(V_IRREG|ED) 
#define MAN		(1<<11)	/* +man, +men, +women, +woman */
#define	ADV		(1<<12)	/* +hood, +ness */
#define STOP		(1<<14)	/* stop list */
#define	NOPREF		(1<<13)	/* no prefix */

#define MONO		(1<<15)	/* double final consonant as in fib->fibbing */
#define IN		(1<<16) /* in- im- ir, not un- */
#define _Y		(1<<17)	/* +y */

#define ALL		(~(NOPREF|STOP|DONT_TOUCH|MONO|IN))    /*anything goes (no stop or nopref)*/

static char *
code2str(int code)
{
	static struct codes {
		const char *str;
		int val;
	} codes[] = {
		{ "ED", ED },
		{ "ADJ", ADJ },
		{ "NOUN", NOUN },
		{ "PROP_COLLECT", PROP_COLLECT },
		{ "ACTOR", ACTOR },
		{ "EST", EST },
		{ "DONT_TOUCH", DONT_TOUCH },
		{ "ION", ION },
		{ "N_AFFIX", N_AFFIX },
		{ "V_AFFIX", V_AFFIX },
		{ "V_IRREG", V_IRREG },
		{ "MAN", MAN },
		{ "ADV", ADV },
		{ "STOP", STOP },
		{ "NOPREF", NOPREF },
		{ "MONO", MONO },
		{ "IN", IN },
		{ "_Y", _Y },
		{ NULL, 0 }
	};
	static char buffer[200];
	char *cp = buffer;

	memset(buffer, '\0', sizeof(buffer));
	for (int i = 0; codes[i].str != NULL; i++) {
		if ((code & codes[i].val) != 0) {
			if (cp != buffer)
				*cp++ = '|';
			strcpy(cp, codes[i].str);
			cp += strlen(cp);
		}
	}
	*cp = '\0';

	return buffer;
}
