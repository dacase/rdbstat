#include "unixstat.h"
PGM(maketrix, Make A Matrix)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

main (argc, argv) char **argv;
	{
	int	ncols;
	int	nstrings = 0;
	char	string[BUFSIZ];
	Argv0 = argv[0];
	switch (argc)
	    {
	    case 1:
		ncols = 2;
		break;
	    case 2:
		if (!number (argv[1]) || ((ncols = atoi (argv[1])) < 1))
		    ERRMSG1 (%s is not a valid number of columns, argv[1])
		break;
	    default:
			USAGE ([ncols])
	    }
	checkstdin (argv[0]);
	while (scanf ("%s", string) == 1)
		{
		fputs (string, stdout);
		if (++nstrings == ncols)
			{
			putchar ('\n');
			nstrings = 0;
			}
		else putchar ('\t');
		}
	if (nstrings != 0)
	    WARNING (last line does not have expected number of columns)
	}
