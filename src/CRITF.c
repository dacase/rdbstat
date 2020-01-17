#include "unixstat.h"
PGM(critf, Probability to F-Ratio Conversion)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

main (argc, argv) int argc; char **argv;
	{
	double	F;
	double	p;
	double	critf ();
	double	atof ();
	int	df1;
	int	df2;
	Argv0 = argv[0];
	if (argc != 4)
	    USAGE (p df1 df2)
	if ((p	=	atof (argv[1])) <= 0.0 || p >= 1.0)
	    ERRMSG0 (probability must be between 0.0 and 1.0)
	df1	=	atoi (argv[2]);
	df2	=	atoi (argv[3]);
	if (df1 <= 0 || df2 <= 0)
	    ERRMSG0 (df must be positive)
	F	=	critf  (p, df1, df2);
	printf ("F(%d,%d) = %4.3f, p = %4.3f\n", df1, df2, F, p);
	}
