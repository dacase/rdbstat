#include "unixstat.h"
PGM(transpose, Transpose Rows and Columns of Matrix File)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#define MAXCOLS 100
#define MAXLINES 100
#define MAXLEN  20
char	*A[MAXLINES][MAXCOLS];

main (argc, argv) char **argv;
	{
	char	array[MAXCOLS][MAXLEN];
	int	c, l;
	int	ncols = 0;
	int	lines = 1;
	int	maxcols = 0;
	char	format[10];
	char	line[BUFSIZ];
	FILE	*ioptr = stdin;
	Argv0 = argv[0];
	strcpy (format, "%s\t");
	for (c = 1; c < argc; c++)
		if (number (argv[c]) && access (argv[c], 4))
			{
			format[0] = '%';
			strcpy (format+1, argv[1]);
			strcat (format, "s ");
			}
		else if ((ioptr = fopen (argv[c], "r")) == NULL)
			ERROPEN (argv[c])
	if (ioptr == stdin) checkstdin (argv[0]);
firstline:
	if (fgets (line, BUFSIZ, ioptr))
		{
		ncols = sstrings (line, array, MAXCOLS, MAXLEN);
		if (ncols > maxcols)
			if (ncols > MAXCOLS)
				ERRMANY (columns, MAXCOLS)
			else maxcols = ncols;
		if (ncols == 0) goto firstline;
		for (c = 0; c < ncols; c++)
			A[0][c] = strdup (array[c]);
		}
	else
		ERRDATA
	while (fgets (line, BUFSIZ, ioptr))
		{
		if (lines == MAXLINES)
			ERRMANY (lines, MAXLINES)
		ncols = sstrings (line, array, MAXCOLS, MAXLEN);
		if (ncols == 0) continue;
		if (ncols > maxcols)
			if (ncols > MAXCOLS)
				ERRMANY (columns, MAXCOLS)
			else maxcols = ncols;
		for (c = 0; c < ncols; c++)
			A[lines][c] = strdup (array[c]);
		lines++;
		}
	for (c = 0; c < maxcols; c++)
		{
		for (l = 0; l < lines; l++)
			printf (format, A[l][c]);
		putchar ('\n');
		}
	}
