#include "unixstat.h"
PGM(reverse, Reverse Lines/Fields/Characters)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#define MAXLINES  8192
#define	MAXFIELDS  100
#define	MAXLEN     100
char	*lptr[MAXLINES];
int	Revlines, Revchars, Revfields;

char *
revchar (s)
char	*s;
	{
	char	buf[BUFSIZ];
	char	*ptr = buf;
	char	*sptr = s;
	while (*sptr) sptr++;
	while (sptr >= s)
		*ptr++ = *--sptr;
	*ptr = NULL;
	return (strcpy (s, buf));
	}

char *
revfield (line)
char	*line;
	{
	char	array[MAXFIELDS][MAXLEN];
	int	ncols, ii;
	char	*ptr = line;
	if ((ncols = sstrings (line, array, MAXFIELDS, MAXLEN)) > MAXFIELDS)
		ERRMANY (fields, MAXFIELDS)
	for (ii = ncols-1; ii >= 0; ii--)
		{
		strcpy (ptr, array[ii]);
		while (*ptr) ptr++;
		if (ii != 0) *ptr++ = '\t';
		}
	*ptr = NULL;
	return (line);
	}

initial (argc, argv)
char	**argv;
	{
	int	i;
	char	*ptr;
	for (i = 1; i < argc; i++)
		{
		ptr = argv[i];
		if (*ptr == '-')
			{
			ptr++;
			while (*ptr)
				{
				switch (*ptr)
					{
					case 'f': Revfields = 1; ptr++; break;
					case 'l': Revlines = 1; ptr++; break;
					case 'c': Revchars = 1; ptr++; break;
					default:
						ERROPT (*ptr)
					}
				}
			}
		}
	checkstdin (argv[0]);
	}

main (argc, argv) char **argv;
	{
	char	line[BUFSIZ];
	int	nlines = 0;
	char	*ptr;
	Argv0 = argv[0];
	if (argc == 1) Revlines = 1;
	initial (argc, argv);
	while (gets (line))
		{
		ptr = line;
		if (Revchars) ptr = revchar (ptr);
		if (Revfields) ptr = revfield (ptr);
		if (Revlines)
			if (++nlines == MAXLINES)
				ERRMANY (lines, MAXLINES)
			else lptr[nlines-1] = strdup (ptr);
		else puts (ptr);
		}
	if (Revlines)
		while (nlines > 0)
			puts (lptr[--nlines]);
	exit (0);
	}
