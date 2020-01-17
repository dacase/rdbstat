#include "unixstat.h"
PGM(dprime, Signal Detection Theory Analysis)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#define	MAXCOLS	2
#define	MAXSTRING 10
FILE	*datafile;
char	column[MAXCOLS][MAXSTRING];
int 	presented, obtained;
int 	hit, miss, false_alarm, correct_rejection;
double	hr, far;
double	dprime, beta;

#define	table(fmt,v1,v2,v3,v4) printf ("%fmt %fmt %fmt %fmt\n",v1,v2,v3,v4)

main (argc, argv) char **argv;
	{
	double	ptoz (), normaldensity ();
	int 	ncols;
	Argv0 = argv[0];
	switch (argc)
		{
		default:
			USAGE ([file] or [hit-rate false-alarm-rate])
			break;
		case 1: /* read data from standard input */
			datafile = stdin;
			checkstdin (argv[0]);
		case 2: /* open data file and read it */
			if (datafile != stdin)
				if ((datafile = fopen (argv[1], "r")) == NULL)
					ERROPEN (argv[1])
			while ((ncols = fstrings (datafile, column, MAXCOLS, MAXSTRING)) != EOF)
				{
				if (ncols != 2)
					ERRMSG0 (each line must have 2 columns)
				presented = yesno (column[0]);
				obtained  = yesno (column[1]);
				if (presented)
					{
					if (obtained) hit++;
					else miss++;
					}
				else /* not presented */
					{
					if (obtained) false_alarm++;
					else correct_rejection++;
					}
				}
			if (datafile != stdin) fclose (datafile);
			printf ("%15s%15s%15s\n", "", "signal", "noise");
			printf ("%15s%15d%15d\n", "yes", hit, false_alarm);
			printf ("%15s%15d%15d\n", "no", miss, correct_rejection);
			printf ("\n");
			hr = hit ? hit / (double) (hit + miss) : 0.0;
			far = false_alarm
				? false_alarm / (double) (false_alarm + correct_rejection)
				: 0.0;
			break;
		case 3: /* parameters supplied */
			if (!number (argv[1])) ERRNUM (argv[1])
			if (!number (argv[2])) ERRNUM (argv[2])
			hr = atof (argv[1]);
			far = atof (argv[2]);
			break;
		}
	if (hr < 0.0 || hr > 1.0)
		ERRMSG1 (The hit rate %f is not a probability, hr)
	if (far < 0.0 || far > 1.0)
		ERRMSG1 (The false-alarm rate %f is not a probability, far)
	dprime = ptoz (hr) - ptoz (far);
	beta   = normaldensity (ptoz (hr)) / normaldensity (ptoz (far));
	table (15s,"hr","far","dprime","beta");
	table (15.2f,hr,far,dprime,beta);
	}

char	*yesses[] = {"yes", "1", "1.0000", "signal", ""};
char	*noes[]   = {"no",  "0", "0.0000", "noise",  ""};
yesno (string) char *string;
	{
	int 	strno;
	for (strno = 0; *yesses[strno] && strcmp (string, yesses[strno]); strno++);
	if (*yesses[strno]) return (1);
	for (strno = 0; *noes[strno] && strcmp (string, noes[strno]); strno++);
	if (*noes[strno]) return (0);
	ERRMSG1 (illegal value (%s) in datafile, string)
	/*NOTREACHED*/
	}

double normaldensity (z) double z;
	{
	double exp (), sqrt ();
	return (exp (-z*z/2.0)/sqrt (2*3.1415926535));
	}
