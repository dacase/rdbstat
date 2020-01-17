/*LINTLIBRARY*/
double
cor (x,y,n)
float *x, *y;
	{
	double sumx = 0.0;
	double sumy = 0.0;
	double ssx = 0.0;
	double ssy = 0.0;
	double sumxy = 0.0;
	double denom, r;
	extern double sqrt ();
	int	i;
	for (i = 0; i < n; i++)
		{
		sumx += *x;
		sumy += *y;
		ssx  += *x * *x;
		ssy  += *y * *y;
		sumxy+= *x++ * *y++;
		}
	if ((denom = (n*ssx-sumx*sumx)*(n*ssy-sumy*sumy)) <= 0.0)
		return (0.0);
	r = (n*sumxy-sumx*sumy)/sqrt(denom);
	return (r);
	}
