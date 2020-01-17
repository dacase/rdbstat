/* Copyright (c) 1982 Gary Perlman (see Copyright file) */
/*LINTLIBRARY*/

#include <stdio.h>
#include <math.h>

#define	EPSILON 1.0e-8
#define scale(x,min,max,width) ((int) (width*((x)-(min))/(max==min?1:(max)-(min))))
#define	print(x,f) printf ("%10s = %f\n", "x",  x);

histogram (vec, n, hist, freq, prop, base, interval)
float	*vec;
double	base;
double	interval;
	{
	char	*malloc ();
	double	max = *vec;
	int 	bins, bin;
	int point;
	int 	*frequency;
	for (point = 0; point < n; point++)
		if (vec[point] > max) max = vec[point];
	if (interval == 0.0)
		{
		interval = (max-base)/sqrt(2.0*n);
		if (fabs (interval) > 1.0)
			interval = floor (interval);
		else if (interval <= 0.0) interval = 1.0;
		}
	if (base > max) return (0);
	if ((bins = (max-base)/interval) == 0) bins = 1;
	if (frequency = (int *) malloc ((unsigned) bins * sizeof (*frequency)))
		{
		for (bin = 0; bin < bins; bin++)
			frequency[bin] = 0;
		for (point=0; point<n; point++)
			if ((bin = scale(vec[point],base,max+EPSILON,bins)) >= 0)
				frequency[bin]++;
		barhist (frequency, bins, hist, freq, prop, base, interval);
		free ((char *) frequency);
		}
	return (bins);
	}

barhist (frequency, bins, hist, freq, prop, base, interval)
int 	*frequency;
double	base;
double	interval;
	{
	int 	maxfreq = 0;
	int 	bin, point;
	double	midpoint = base - interval/2.0;
	int 	cumf	= 0;
	double	fcumf	= 0.0;
	int 	sum = 0;
	for (bin = 0; bin < bins; bin++)
		{
		if (frequency[bin] > maxfreq) maxfreq = frequency[bin];
		sum += frequency[bin];
		}
	if (sum == 0) sum = 1;
	printf ("%8s", "Midpt");
	if (freq) printf ("%8s", "Freq");
	if (freq > 1) printf ("%8s", "Cum");
	if (prop) printf ("%8s", "Prop");
	if (prop > 1) printf ("%8s", "Cum");
	putchar ('\n');
	for (bin = 0; bin < bins; bin++)
		{
		printf ("%8.3f", midpoint += interval);
		if (freq) printf ("%8d", frequency[bin]);
		if (freq > 1) printf ("%8d", cumf += frequency[bin]);
		if (prop) printf ("%8.3f", frequency[bin]/(double)sum);
		if (prop > 1) printf ("%8.3f", (fcumf += frequency[bin])/sum);
		if (hist)
			{
			int	repeat = hist==1
						? frequency[bin]
						: scale(frequency[bin],0.0,maxfreq,hist);
			putchar (' ');
			for (point = 0; point < repeat; point++) putchar ('=');
			}
		putchar ('\n');
		}
	}
