#include "unixstat.h"
PGM(series, Print a Series of Numbers)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

main (argc, argv) char **argv;
	{
	double	low, high, incr = 1.0;
	Argv0 = argv[0];
	if (argc < 3)
		USAGE (low high [incr])
	else 
		{
		if (!number (argv[1]))
			ERRNUM (argv[1])
		else low = atof (argv[1]);
		if (!number (argv[2]))
			ERRNUM (argv[2])
		else high = atof (argv[2]);
		if (low > high) incr = -incr;
		if (argc > 3)
			if (!number (argv[3]))
				ERRNUM (argv[3])
			else incr = atof (argv[3]);
		}
	if ((low < high && incr <= 0.0) || (low > high && incr >= 0.0))
		ERRMSG3 (Adding %g to %g will never reach %g, incr, low, high)
	if (low <= high)
		while (low <= high)
			{
			printf ("%g\n", low);
			low += incr;
			}
	else if (low > high)
		while (low >= high)
			{
			printf ("%g\n", low);
			low += incr;
			}
	}
