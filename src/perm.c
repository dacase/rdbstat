#include "unixstat.h"
PGM(perm, Randomly Permut Lines)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#include <sys/types.h>
#include <sys/stat.h>

/*
 * the divisor is used to map integers returned by rand()
 * to between zero and one.  It is one larger than the
 * largest integer on a machine
 */
#ifndef DIVISOR
#define DIVISOR (sizeof(int)==4?2147483648.0:32768.0)
#endif
double	divisor = DIVISOR;

#define MAXLINES 1000
char	*lptr[MAXLINES];
main (argc, argv) char **argv;
	{
	char	line[BUFSIZ];
	int 	nlines = 0;
	int 	lineno;
	unsigned len;
	struct	stat	statbuf;
	FILE	*infile = stdin;
	FILE	*outfile = stdout;

	Argv0 = argv[0];
	switch (argc)
		{
		case 3:
			if (strcmp (argv[1], argv[2]) == 0)
				ERRMSG0 (Cannot permut a file onto itself)
			if (stat (argv[2], &statbuf) == 0 && statbuf.st_size)
				{
				if (!confirm ("perm: destroy the old %s", argv[2]))
					ERRMSG0 (no files changed)
				}
			outfile = fopen (argv[2], "w");
			if (outfile == NULL)
				ERROPEN (argv[2])
			/* fall through to open input file */
		case 2:
			infile = fopen (argv[1], "r");
			if (infile == NULL)
				ERROPEN (argv[1])
			break;
		case 1:
			checkstdin (argv[0]);
			break;
		}
	while (fgets (line, BUFSIZ, infile))
		{
		if (++nlines == MAXLINES)
			ERRMANY (lines, MAXLINES)
		len = strlen (line);
		if ((lptr[nlines-1] = malloc (len+1)) == NULL)
			ERRSPACE (lines)
		strcpy (lptr[nlines-1], line);
		}
	strpermut (lptr, nlines);
	for (lineno = 0; lineno < nlines; lineno++)
		fputs (lptr[lineno], outfile);
	}

strpermut (array, size) char *array[];
	{
	int 	i,irand;
	char	*temp;
	long	time ();
	/* seed random number generator with time and process id */
	srand ((int) time ((long *) 0) - getpid ());
	for (i = 0; i < size; i++)
		{
		irand = (int) (rand () / divisor * (size-i)) + i;
		temp = array[i];
		array[i] = array[irand];
		array[irand] = temp;
		}
	}
