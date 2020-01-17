/*LINTLIBRARY*/
#include <stdio.h>
#include <math.h>

double
vsum (v, n)
float	*v;
	{
	double	sum = 0.0;
	while (n-- > 0)
		sum += *v++;
	return (sum);
	}

double
vsumsq (v, n)
float	*v;
	{
	double	sumsq = 0.0;
	while (n-- > 0)
		{
		sumsq += *v * *v;
		v++;
		}
	return (sumsq);
	}

double
standev (sum, sumsq, n)
double	sum, sumsq;
	{
	if (n <= 1) return (0.0);
	return (sqrt ((sumsq-sum*sum/n)/(n-1)));
	}

double
vsd (v, n)
float	*v;
	{
	return (standev (vsum (v, n), vsumsq (v, n), n));
	}

double
vmin (v, n)
float	*v;
	{
	double	min = *v;
	while (n-- > 0)
		{
		if (*v < min) min = *v;
		v++;
		}
	return (min);
	}

double
vmax (v, n)
float	*v;
	{
	double	max = *v;
	while (n-- > 0)
		{
		if (*v > max) max = *v;
		v++;
		}
	return (max);
	}
