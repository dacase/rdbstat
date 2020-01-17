#include "unixstat.h"
PGM(ts, Time Series Analysis and Plots)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#define	WIDTH 70

double	Base = 1.0;
double	Interval = 1.0;
int 	Axes = 1;
int 	Echo;
int 	Width = WIDTH;
int 	Minimum;
int 	Maximum;
int 	Length;
int 	Plot;
int 	Stats;
int 	Number;
int 	Autocor;

#define	TSIZE   1000

typedef struct ts
	{
	unsigned	int	size;
	float	*data;
	} TS;

#define	check_ts(ts) if(!(ts || ts->size))return(NULL)

TS *
new_ts (size)
	{
	TS	*ts;
	if ((ts = (TS *) malloc (sizeof (TS))) == NULL) return (NULL);
	if ((ts->data = (float *) malloc ((unsigned)size * sizeof (*ts->data))) == NULL)
		return (NULL);
	ts->size = 0;
	return (ts);
	}

TS *
make_ts (ts, n)
TS	*ts;
	{
	TS	*newts = new_ts (n);
	int	i;
	int	tsn = ts->size;
	if (newts)
		{
		newts->size = n;
		for (i = 0; i < n; i++)
			newts->data[i] = 0.0;
		for (i = 0; i < n*tsn; i++)
			newts->data[i/tsn] += ts->data[i/n];
		for (i = 0; i < n; i++)
			newts->data[i] /= tsn;
		}
	return (newts);
	}

read_ts (ts)
TS	*ts;
	{
	char	s[BUFSIZ];
	check_ts (ts);
	while (scanf ("%s", s) == 1)
		{
		if (!number (s))
			ERRNUM (s)
		if (ts->size >= TSIZE)
			ERRSPACE (time series)
		ts->data[ts->size++] = atof (s);
		}
	}

print_num (n)
	{
	if (!Number) return;
	if ((n >= 0) && ((n%Number) == 0))
		printf ("%*g	", 8, n*Interval + Base);
	else
		printf ("%*s	", 8, "");
	}

print_ts (ts)
TS	*ts;
	{
	int	ii;
	check_ts (ts);
	for (ii = 0; ii < ts->size; ii++)
		{
		print_num (ii);
		printf ("%.5g\n", ts->data[ii]);
		}
	}

#define	tabprint(label,format,value) printf ("%s	= %format\n", "label", (value))

printstats (vector, n)
float	*vector;
unsigned n;
	{
	double	sum = 0.0;
	double	ss  = 0.0;
	double	min = vector[0];
	double	max = vector[0];
	int	count = n;
	while (count-- > 0)
		{
		sum += vector[0];
		ss  += vector[0] * vector[0];
		if (vector[0] > max)
			max = vector[0];
		else if (vector[0] < min)
			min = vector[0];
		vector++;
		}
	tabprint (n,d,n);
	tabprint (sum,g,sum);
	tabprint (ss,g,ss);
	tabprint (min,g,min);
	tabprint (max,g,max);
	tabprint (range,g,max-min);
	tabprint (midpt,g,(max+min)/2.0);
	tabprint (mean,g,sum/n);
	tabprint (sd,g,n <= 1 ? 0.0 : sqrt ((ss-sum*sum/n)/(n-1)));
	}

cor_ts (ts)
TS	*ts;
	{
	int	lag;
	double	cor ();
	double	r, r2;
	double	Fr;
	double	p, pof ();
	int	df;
	float	*seq = (float *) malloc (ts->size);
	for (lag = 0; lag < ts->size; lag++)
		seq[lag] = (float) lag;
	printf ("%8s %6s %6s %5s %12s %5s %6s\n",
		"Lag", "r", "r^2", "n'", "F", "df", "p");
	for (lag = 0; lag <= Autocor; lag++)
	    {
	    df = ts->size - lag - 2;
		if (lag == 0)
			r = cor (seq, ts->data, ts->size);
		else
			r = cor (ts->data, ts->data+lag, ts->size-lag);
	    r2 = r*r;
	    if (r2 == 1.0)
			{
			Fr = 0.0;
			p = 0.0;
			}
	    else
			{
			Fr = (r2 * df) / (1.0 - r2);
			p = pof (Fr, 1, df);
			}
	    printf ("%8d %6.3f %6.3f %5d %12.3f %5d %6.3f\n",
			lag, r, r2, ts->size-lag, Fr, df, p);
	    }
	}

setup (argc, argv) char **argv;
	{
	int	arg;
	char	*ptr;
	char	options[BUFSIZ];
	*options = NULL;
	for (arg = 1; arg < argc; arg++)
		strcat (options, argv[arg]);
	ptr = options;
	while (*ptr)
		{
		switch (*ptr)
			{
			case 'm':
				Minimum = atoi (++ptr);
				break;
			case 'M':
				Maximum = atoi (++ptr);
				break;
			case 'e':
				Echo = 1;
				++ptr;
				break;
			case 'i':
				Interval = atof (++ptr);
				if (Interval == 0.0) Interval = 1.0;
				break;
			case 'b':
				Base = atof (++ptr);
				if (Base == 0.0) Base = 1.0;
				break;
			case 'l':
				Length = atoi (++ptr);
				break;
			case 'p':
				Plot = atoi (++ptr);
				if (Plot == 0) Plot = 1;
				break;
			case 's':
				Stats = 1; ++ptr;
				break;
			case 'n':
				Number = atoi (++ptr);
				if (Number == 0) Number = 1;
				break;
			case 'a':
				Axes = !Axes;
				Plot = Plot ? Plot : 1;
				++ptr;
				break;
			case 'c':
				Autocor = atoi (++ptr);
				if (Autocor == 0) Autocor = 1;
				break;
			case 'w':
				Width = atoi (++ptr);
				if (Width == 0) Width = WIDTH;
				Plot = Plot ? Plot : 1;
				break;
			default:
				++ptr;
			}
		}
	if (!Plot && !Autocor && !Echo) Stats = 1;
	}

main (argc, argv) char**argv;
	{
	TS	*ts = new_ts (TSIZE);
	Argv0 = argv[0];
	checkstdin (argv[0]);
	setup (argc, argv);
	read_ts (ts);
	if (Length)
		ts = make_ts (ts, Length);
	if (Echo)
		print_ts (ts);
	if (Stats)
		printstats (ts->data, ts->size);
	if (Autocor)
		cor_ts (ts);
	if (Plot)
		barplot (ts->data, ts->size, Plot, Axes, Number, Width, Base, Interval);
	}
