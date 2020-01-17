#include "unixstat.h"
PGM(abut, Join Files)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

/*	abut reads from its argument files, one line at a time
	per cycle, and prints all those lines on one line to
	the standard output.
*/
#define MAXFILES 20
#define FORMAT	"%s\t"  /* width of printed field */
char	format[100];
int	filenum;
int	nfiles = 0;
FILE	*ioptr[MAXFILES], *fopen ();
char	inline[BUFSIZ];
char	outline[BUFSIZ];
char	tmpline[BUFSIZ];
int	linelen;
int	done = 0;		/* true if output is to stop */
int	doneonce[MAXFILES];	/* true if [file] has been exhausted >= once */
int	numberlines = 0;	/* true if lines are to be numbered */
int	cycle;			/* true if abut should cycle through files */
				/* until all have been doneonce */
int	linenumber = 0;
main (argc, argv) char **argv;
	{
	int	arg;
	char	*ptr;
	Argv0 = argv[0];
	if (argc == 1)
		USAGE ([-nc] file1 file2 ...)

	strcpy (format, FORMAT);
	for (arg = 1; arg < argc; arg++)
		{
		ptr = argv[arg];
		if (number (ptr) && access (ptr, 4))
			{
			format[0] = '%';
			strcpy (format+1, ptr);
			strcat (format, "s ");
			}
		else if ((*ptr == '-') && ptr[1])
		    {
		    ptr++;
		    while (*ptr) switch (*ptr)
			{
			case 'n': numberlines = 1; ptr++; break;
			case 'c': cycle = 1; ptr++; break;
			default:
				ERROPT (*ptr)
			}
		    }
		else
		    {
		    if (nfiles == MAXFILES)
			ERRMANY (files, MAXFILES)
			if (*ptr == '-')
				ioptr[nfiles] = stdin;
		    else ioptr[nfiles] = fopen (ptr, "r");
		    if (ioptr[nfiles] == NULL)
			    ERROPEN (ptr)
		    nfiles++;
		    }
		}
	while (!done)
		{
		*outline = NULL;
		if (numberlines) sprintf (outline, "%-4d ", ++linenumber);
		for (filenum = 0; filenum < nfiles; filenum++)
			{
			if (fgets (inline, BUFSIZ, ioptr[filenum]) == NULL)
				{
				doneonce[filenum] = 1;
				if (cycle)
					{
					rewind (ioptr[filenum]);
					if (fgets (inline, BUFSIZ, ioptr[filenum]) == NULL)
						inline[0] = NULL;
					}
				else inline[0] = NULL;
				}
			linelen = strlen (inline) - 1;
			while (linelen > 0 && isspace (inline[linelen]))
				inline[linelen--] = NULL;
			sprintf (tmpline, format, inline);
			strcat (outline, tmpline);
			}
		done = 1;
		for (filenum = 0; filenum < nfiles; filenum++)
			if (!doneonce[filenum]) done = 0;
		if (!done) printf ("%s\n", outline);
		}
	}
