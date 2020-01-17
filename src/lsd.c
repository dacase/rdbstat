#include "unixstat.h"
PGM(lsd, Least Significant Differences and Confidence Intervals)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

double	critf ();
double
scheffe (p, df1, df2, mse, n) double p, mse;
	{
	return (sqrt (df1 * critf (p, df1, df2) * mse * 2 / n));
	}
double
lsd (p, df1, df2, mse, n) double p, mse;
	{
	return (sqrt (critf (p, df1, df2) * mse * 2 / n));
	}
main (argc, argv) char **argv;
	{
	double	prob = atof (argv[1]);
	int	df1 = atoi (argv[2]);
	int	df2 = atoi (argv[3]);
	double	mse = atof (argv[4]);
	int	n = atoi (argv[5]);
	Argv0 = argv[0];
	if (argc != 6)
		USAGE (prob df1 df2 mse n)
	puts ("Least Significant Difference and Scheffe's Method");
	printf ("LSD(%.2f) = %.3g\n", prob, lsd (prob, 1, df2, mse, n));
	printf ("S(%.2f)   = %.3g\n", prob, scheffe (prob, df1, df2, mse, n));
	}
