#include "unixstat.h"
PGM(validata, Data Validation and Consistency Checking)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

/* validata looks at a data file to see what sort of columns it has.
	It complains if there are not an equal number of columns
	for each line in the input.  At the end, it prints the
	type of each column depending on what types of data appeared
	in them.
*/

#include <sgtty.h>
#define	MAXCOLS		100
#define	MAXSTRING	50
int	nalnum[MAXCOLS];
int	nalpha[MAXCOLS];
int	nint[MAXCOLS];
int	nfloat[MAXCOLS];
int	nother[MAXCOLS];
int	ntotal[MAXCOLS];
double	minvalue[MAXCOLS];
double	maxvalue[MAXCOLS];

main (argc, argv) int argc; char **argv;
    {
    char	line[BUFSIZ];
    char	col[MAXCOLS][MAXSTRING];
    FILE	*ioptr;
    int	other;
    int linecount = 0;
    int ncols;
    int maxcols = 0;
    int	colcount = -1;
    int colno;
	double	value;
    char *s;
    Argv0 = argv[0];
    if (argc == 1)
	{
	checkstdin (Argv0);
	ioptr = stdin;
	}
    else if (argc > 2)
	ERRMSG0 (Can only check 1 file at a time)
    else if ((ioptr = fopen (argv[1], "r")) == NULL)
	ERROPEN (argv[1])
    while (fgets (line, BUFSIZ, ioptr))
	{
	linecount++;
	ncols = sstrings (line, col, MAXCOLS, MAXSTRING);
	if (ncols > MAXCOLS)
		ERRMANY (columns, MAXCOLS)
	if (ncols > maxcols) maxcols = ncols;
	if (ncols != colcount)
	    {
	    if (linecount != 1)
		fprintf (stderr, "Variable number of columns at line %d\n", linecount);
	    colcount = ncols;
	    }
	if (ncols == 0)
	    fprintf (stderr, "Line %d is empty\n", linecount);
	other = 1;
	for (colno = 0; colno < ncols; colno++)
	    {
	    s = col[colno];
	    ntotal[colno]++;
	    if (stralnum (s))
		{
		other = 0;
		nalnum[colno]++;
		if (stralpha (s))
		    nalpha[colno]++;
		}
	    if (strfloat (s))
		{
		other = 0;
		nfloat[colno]++;
		if (strint (s))
		    nint[colno]++;
		value = atof (s);
		if (nfloat[colno] == 1)
			minvalue[colno] = maxvalue[colno] = value;
		if (value > maxvalue[colno]) maxvalue[colno] = value;
		else if (value < minvalue[colno]) minvalue[colno] = value;
		}
	    if (other) nother[colno]++;
	    else other = 1;
	    }
	}

    if (ioptr != stdin) fclose (ioptr);
    if (!ispipe (stdout)) printf ("%d lines read\n", linecount);
    printf ("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
	"Column", "total", "alnum", "alpha", "int", "float", "other", "min", "max");
    for (colno = 0; colno < maxcols; colno++)
    printf ("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%g\t%g\n",
	colno+1,  ntotal[colno],
        nalnum[colno], nalpha[colno], nint[colno],
	nfloat[colno], nother[colno], minvalue[colno], maxvalue[colno]);
    }

stralnum (s) char *s;
	{
	while (isspace (*s)) s++;
	while (*s)
		if (!isalnum (*s)) return (0);
		else s++;
	return (1);
	}

stralpha (s) char *s;
	{
	while (isspace (*s)) s++;
	while (*s)
		if (!isalpha (*s)) return (0);
		else s++;
	return (1);
	}

strint (s) char *s;
	{
	while (isspace (*s)) s++;
	if (*s == '+' || *s == '-') s++;
	while (*s)
		if (!isdigit (*s)) return (0);
		else s++;
	return (1);
	}

strfloat (s) char *s;
	{
	while (isspace (*s)) s++;
	if (!*s) return (0);
	if (*s == '+' || *s == '-') s++;
	while (isdigit (*s)) s++;
	if (*s == '.') s++;
	while (isdigit (*s)) s++;
	if (*s == 'E' || *s == 'e')
		{
		s++;
		if (*s == '+' || *s == '-') s++;
		while (isdigit (*s)) s++;
		}
	while (isspace (*s)) s++;
	return (*s == NULL);
	}

ispipe (ioptr)
FILE	*ioptr;
	{
	struct	sgttyb ttybuf;
	return (gtty (fileno (ioptr), &ttybuf));
	}
