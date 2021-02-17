#include "stdlib.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "unixstat.h"
PGM(desc, Descriptive Statistics)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

/*
	This program analyses single distributions of data.
	It was written by Gary Perlman at UCSD August 1979.

		STRUCTURE OF DESC
	main
		getoptions
		input
			bindex
		printstats
			pof
			percentile
		printtable
			bindex

		ALPHABETICAL ANNOTATION
	getoptions (argc, argv)
		sets option flags according to command line arguments
	bindex (xval)
		returns the bin number xval should go into
	input ()
		reads from standard input and sets
		n, sums of powers, minimum, maximum
		might store frequencies in freq array
	percentile (perc, vector, n)
		returns the perc percentile of n long vector, vector
	pof (F, df1, df2)
		returns the probability of F ratio
	printstats ()
		prints all summary statistics
	printtable ()
		prints histogram, frequency or proportion table
*/

#define	MAXBINS		10000	/* maximum number of bins for tables	      */
#define	MAXPOINTS	10000000	/* maximum number of input points if storing  */
/*			    OPTION FLAGS				      */
typedef	int 	boolean;	/* no boolean tpe in C			      */
#define	TRUE	1
#define	FALSE	0
boolean	stats;			/* print statistics			      */
boolean	ftest;			/* print ftest				      */
boolean	table;			/* print a table of some sort		      */
boolean	histogram;		/* print histogram			      */
boolean	frequencies;		/* print frequency table		      */
boolean	proportions;		/* print proportions table		      */
boolean	cumulative;		/* make table cumulative		      */
boolean	storedata;		/* store the data			      */
boolean	onepass;		/* can run with one pass		      */
boolean	setmaximum;		/* maximum value has been set		      */
boolean	setminimum;		/* minimum value has been set		      */
boolean	setintwidth;		/* interval width has been set		      */
boolean	variable;		/* print stats in variable format	      */
/*                          	GLOBALS					      */
float	datax[MAXPOINTS];	/* data stored in here			      */
int 	freq[MAXBINS];		/* frequency counts stored in here	      */
int 	n;			/* number of points read in		      */
double	sum;			/* sum of all points read in		      */
double	s2;			/* sum of squares of all points read in	      */
double	s3;			/* sum of cubes of all points read in	      */
double	s4;			/* sum of x^4 of all points read in	      */
double	gmean;			/* geometric mean			      */
double	hmean;			/* harmonic mean			      */
int 	ngtzero;		/* number of points greater than zero	      */
double	f_null;			/* null value for t test		      */
double	minx;			/* min value of x			      */
double	maxx;			/* max value of x			      */
int 	undermin;		/* number of points less than minimum	      */
int 	overmax;		/* number of points more than maximum	      */
double	intwidth;		/* width of interval of freqency count bins   */
double	minimum;		/* minimum allowable value of x		      */
double	maximum;		/* maximum allowable value of x		      */

int number( char* );

int bindex (xval) float xval;
	{
	int 	answer;
	float	findex;
	if (xval == minimum) return (0);
	findex = (xval - minimum)/intwidth;
	if (floor (findex) == findex) answer = findex - 1.0;
	else answer = findex;
	if (answer >= MAXBINS)
		ERRMSG1 (bin[%d] is out of range, answer)
	return (answer);
	}

void getoptions (argc, argv) int argc; char **argv;
	{
	char	optionline[BUFSIZ];
	char	*options = optionline;
	int 	i;
	char	c;
	// checkstdin (argv[0]);
	for (i = 1; i < argc; i++) strcat (options, argv[i]);
	if (*options == NULL) stats = storedata = TRUE;
	else while (*options) switch (*options++)
		{
		case 'c': cumulative = TRUE; break;
		case 'f': frequencies = table = TRUE; break;
		case 'F': ftest = stats = TRUE; f_null = atof(options); break;
		case 'h': histogram = frequencies = table = TRUE; break;
		case 'i': intwidth = atof (options); setintwidth = table = TRUE;
			  if (intwidth <= 0.0) intwidth = 1.0;
				break;
		case 'm': minimum = atof (options); setminimum = TRUE; break;
		case 'M': maximum = atof (options); setmaximum = TRUE; break;
		case 'p': proportions = table =  TRUE; break;
		case 'o':
		case 'S': storedata = TRUE;
		case 's': stats = TRUE; break;
		case 't': ftest = stats = TRUE; f_null = atof(options); break;
		case 'v': variable = storedata = TRUE; break;
		default:	c = *(options-1);
				if (c != '.' && c != '-' && c < '0' || c > '9')
					ERROPT (c)
		}
	if (table)
		{
		if (setintwidth && setminimum) onepass = TRUE;
		else storedata = TRUE;
		if (!frequencies && !proportions) histogram = TRUE;
		}
	else if (setminimum || setmaximum) stats = TRUE;
	} /* ends getoptions */

void input ()
	{
	double	x;			/* each datum read in here	      */
	double	x2;			/* square of x			      */
	char	stringx[50];		/* string version of x read in here   */
	if (scanf ("%s", stringx) != 1)
		ERRDATA
	minx = maxx = atof (stringx);;
	do	{
		if (!number (stringx))
			ERRNUM (stringx)
		x = atof (stringx);
		if (setminimum && x < minimum) {undermin++; continue;}
		if (setmaximum && x > maximum) {overmax++; continue;}
		if (storedata)
			if (n == MAXPOINTS)
			    {
			    WARNING (too much data for storing)
			    storedata = FALSE;
			    }
			else datax[n] = x;
		if (onepass) freq[bindex(x)]++;
		x2 = x*x;
		sum += x;
		s2  += x2;
		s3  += x2*x;
		s4  += x2*x2;
		if (x > 0.0)
			{
			gmean += log (x);
			hmean += 1.0 / x;
			ngtzero++;
			}
		if (x > maxx) maxx = x;
		if (x < minx) minx = x;
		n++;
	} while (scanf ("%s", stringx) > 0);
	if (n <= 1)
		ERRDATA
	}

void printstats ()
	{
	double	pof ();			/* probability of F ratio	      */
	double	percentile ();		/* percentile function		      */
	int 	fltcmp ();		/* compares two float numbers	      */
	double	M	= sum/n;	/* mean				      */
	double	M2	= M*M;		/* square of mean		      */
	double	var	= (s2 - M*sum)/(n-1); /* variance		      */
	double	sd	= sqrt (var);	/* standard deviation		      */
	double	sk;			/* skew				      */
	double	kt;			/* kurtosis			      */
	double	q1, q3;			/* first and third quartiles	      */
	double	median;			/* 50th percentile		      */
	char	*line	=
	"------------------------------------------------------------";
	if (var == 0.0)
		ERRMSG2 (All these %d numbers equal %.4g, n, M)
	sk = (s3 - 3.0*M*s2 + 3.0*M2*sum - M2*sum)/(n*var*sd);
	kt = (s4-4.*M*s3+6.*M2*s2-4.*M2*M*sum+n*M2*M2)/(n*var*var);
	if (storedata)
		{
		qsort ((char *) datax, n, sizeof (float), fltcmp);
		median = percentile (50, datax, n);
		q1 = percentile (25, datax, n),
		q3 = percentile (75, datax, n);
		}
	/*	PRINT FREQUENCY COUNTS */
	if (!variable) puts (line);
	if (variable)
		{
		printf ("under=%d\n", undermin);
		printf ("count=%d\n", n);
		printf ("over=%d\n", overmax);
		printf ("sum=%g\n", sum);
		}
	else
		{
		printf ("%12s%12s%12s%12s\n",
			"Under Range", "In Range", "Over Range", "Sum");
		printf ("%12d%12d%12d%12.3f\n", undermin, n, overmax, sum);
		puts (line);
		}
	/*	PRINT CENTRAL TENDENCY */
	if (variable)
		{
		printf ("mean=%g\n", M);
		if (storedata) printf ("median=%g\n", median);
		printf ("midpoint=%g\n", (maxx+minx)/2.0);
		if (ngtzero == n)
			{
			printf ("geomean=%g\n", exp (gmean/ngtzero));
			printf ("harmean=%g\n", ngtzero/hmean);
			}
		}
	else
		{
		printf ("%12s%12s%12s%12s%12s\n", "Mean", "Median", "Midpoint",
			"Geometric", "Harmonic");
		printf ("%12.3f", M);
		if (storedata)
			printf ("%12.3f", median);
		else	printf ("%12s", "");
		printf ("%12.3f", (maxx+minx)/2.0);
		if (ngtzero == n)
			printf("%12.3f%12.3f\n",
				exp (gmean/ngtzero), ngtzero/hmean);
		else printf ("\n");
		puts (line);
		}
	/*	PRINT VARIABILITY */
	if (variable)
		{
		printf ("sd=%g\n", sd);
		if (storedata)
			printf ("quartdev=%g\n", (q3-q1)/2.0);
		printf ("range=%g\n", maxx-minx);
		printf ("semean=%g\n", sqrt (var/n));
		}
	else
		{
		printf ("%12s%12s%12s%12s\n",
			"SD", "Quart Dev", "Range", "SE mean");
		printf("%12.3f", sd);
		if (storedata)
			printf ("%12.3f", (q3-q1)/2.0);
		else	printf ("%12s", "");
		printf ("%12.3f", maxx-minx);
		printf ("%12.3f\n", sqrt(var/n));
		puts (line);
		}
	/*	PRINT FIVENUMS */
	if (variable)
		{
		printf ("min=%g\n", minx);
		if (storedata)
			{
			printf ("q1=%g\n", q1);
			printf ("q2=%g\n", median);
			printf ("q3=%g\n", q3);
			}
		printf ("max=%g\n", maxx);
		}
	else
		{
		printf ("%12s", "Minimum");
		if (storedata)
			printf ("%12s%12s%12s",
				"Quartile 1", "Quartile 2", "Quartile 3");
		printf ("%12s\n", "Maximum");
		printf ("%12.3f", minx);
		if (storedata)
			printf ("%12.3f%12.3f%12.3f", q1, median, q3);
		printf ("%12.3f\n", maxx);
		puts (line);
		}
	/*	PRINT OTHER STUFF */
	if (variable)
		{
		printf ("skew=%g\n", sk);
		printf ("kurt=%g\n", kt);
		}
	else
		{
		printf ("%12s%12s\n", "Skew", "Kurtosis");
		printf ("%12.3f%12.3f\n", sk, kt);
		puts (line);
		}
	if (ftest)
		{
		double	tval, fval, prob;
		tval = (M - f_null)/(sqrt (var/n));
		fval = tval*tval;
		prob = pof (fval, 1, n-1);
		if (variable)
			{
			printf ("nullmean=%g\n", f_null);
			printf ("t=%g\n", tval);
			printf ("probt=%g\n", prob);
			printf ("F=%g\n", fval);
			printf ("probF=%g\n", prob);
			}
		else
			{
			printf ("%12s%12s%12s%12s%12s\n",
				"Null Mean", "t", "prob (t)", "F", "prob (F)");
			printf ("%12.3f%12.3f%12.3f%12.3f%12.3f\n", 
				f_null, tval, prob, fval, prob);
			puts (line);
			}
	    }
	}
int fltcmp (f1, f2) float *f1, *f2;
	{ if (*f1 < *f2) return (-1); if (*f1 == *f2) return (0); return (1); }

void printtable ()
	{
	register int point;		/* looping variable		      */
	register int i;			/* looping variable		      */
	int 	maxindex;		/* maximum index for maxx	      */
	double	midpoint;		/* midpoint of each interval	      */
	int 	cumf	= 0;		/* cumulative frequency		      */
	double	fcumf	= 0.0;		/* floating cumulative frequency      */
	if (!setminimum) minimum = floor (minx);
	if (!setmaximum) maximum = maxx;
	if (!setintwidth)
		{
		intwidth = (maxx-minimum)/sqrt(2.0*n);
		if (fabs (intwidth) > 1.0)
			intwidth = floor (intwidth);
		}
	if (!onepass) for (point=0; point<n; point++)
		freq[ bindex ( datax[point] ) ]++;
	midpoint = minimum - intwidth/2.0;
	maxindex = bindex (maximum);
	printf ("%8s", "Midpt");
	if (frequencies)
		{
		printf ("%8s", "Freq");
		if (cumulative)
			printf ("%8s", "Cum");
		}
	if (proportions)
		{
		printf ("%8s", "Prop");
		if (cumulative)
			printf ("%8s", "Cum");
		}
	printf ("\n");
	for (i = 0; i <= maxindex; i++)
		{
		midpoint += intwidth;
		printf ("%8.3f", midpoint);
		if (frequencies)
			{
			printf ("%8d", freq[i]);
			if (cumulative)
				printf ("%8d", cumf += freq[i]);
			}
		if (proportions)
			{
			printf ("%8.3f", freq[i]*1.0/n);
			if (cumulative)
				{
				fcumf += freq[i];
				printf ("%8.3f", fcumf/n);
				}
			}
		if (histogram)
			{
			printf (" ");
			for (point = 1; point <= freq[i]; point++)
				printf ("*");
			}
		printf ("\n");
		}
	}

double percentile (perc, v, n) float v[]; int perc, n;
	{
	/*returns the perc percentile of v[n]*/
	/*assumes v is sorted*/
	float findex;
	int pindex;
	findex = perc / 100.0 * n  - 0.5;
	if ( findex < 0.0) findex = 0.0;
	else if (findex > (n-1.0)) findex = n - 1.0;
	pindex = findex;
	return (v[pindex+1] * (findex - pindex) +
		 v[pindex] * (1.0 - findex + pindex));
	}

int main (argc, argv) int argc; char *argv[];
	{
	Argv0 = argv[0];
	getoptions (argc, argv);
	input ();
	if (stats) printstats ();
	if (table) printtable ();
	}

