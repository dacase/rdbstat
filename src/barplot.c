/*LINTLIBRARY*/
#include <stdio.h>
#include <math.h>

#define	EPSILON (.000000001)
#define scale(x,min,max,width) ((int) (width*((x)-(min))/((max)-(min)+EPSILON)))
#define	print(x,f) printf ("%10s = %f\n", "x",  x);

meanplot (mean, sd, min, max, width)
double	mean;
double	sd;
double	min;
double	max;
	{
	char	line[BUFSIZ];
	int	ii;
	for (ii = 0; ii < width; ii++)
		line[ii] = '-';
	line[width] = NULL;
	line[scale (mean, min, max, width)] = '|';
	line[scale (mean+sd, min, max, width)] = '+';
	line[scale (mean-sd, min, max, width)] = '+';
	puts (line);
	}

barplot (vec, n, style, axes, number, width, base, interval)
float	*vec;
double	base;
double	interval;
	{
	int	elt;
	double	sum = 0.0;
	double	ss  = 0.0;
	double	min = *vec;
	double	max = *vec;
	double	mean;
	double	sd;
	int	w;
	int	midwidth;
	for (elt = 0; elt < n; elt++)
		{
		sum += vec[elt];
		ss  += vec[elt] * vec[elt];
		if (vec[elt] < min) min = vec[elt];
		if (vec[elt] > max) max = vec[elt];
		}
	mean = sum/n;
	sd = n <= 1 ? 0.0 : sqrt ((ss-sum*sum/n)/(n-1));
	midwidth = scale (mean, min, max, width);
	nlabel (-1, number, base, interval);
	if (axes) meanplot (mean, sd, min, max, width);
	for (elt = 0; elt < n; elt++)
		{
		w = scale (vec[elt], min, max, width);
		nlabel (elt, number, base, interval);
		barline (w, midwidth, style);
		}
	nlabel (-1, number, base, interval);
	if (axes) meanplot (mean, sd, min, max, width);
	}

barline (n, mid, style)
	{
	switch (style)
		{
		case 2:
			if (n < mid)
				{
				repeat ('-', n);
				}
			else if (n == mid)
				{
				repeat ('-', n);
				repeat ('+', 1);
				}
			else
				{
				repeat ('-', mid);
				repeat ('|', 1);
				repeat ('-', n-mid);
				}
			break;
		default:
			if (n < mid)
				{
				repeat (' ', n);
				repeat ('-', mid-n);
				repeat ('|', 1);
				}
			else if (n == mid)
				{
				repeat (' ', n);
				repeat ('+', 1);
				}
			else
				{
				repeat (' ', mid);
				repeat ('|', 1);
				repeat ('-', n-mid);
				}
			break;
		}
		putchar ('\n');
	}

repeat (c, n)
	{
	while (n-- > 0)
		putchar (c);
	}

nlabel (n, number, base, interval)
double	base;
double	interval;
	{
	if (!number) return;
	if ((n >= 0) && ((n%number) == 0))
		printf ("%g	", n*interval + base);
	else
		putchar ('\t');
	}
