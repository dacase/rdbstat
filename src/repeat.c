#include "unixstat.h"
PGM(repeat, Repeat a File or String)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

char	*
saveit ()
	{
	static	char	tmpfile[BUFSIZ];
	char	c;
	FILE	*ioptr;
	sprintf (tmpfile, "/tmp/repeat%d", getpid ());
	if (ioptr = fopen (tmpfile, "w"))
		{
		checkstdin (Argv0);
		while ((c = getchar ()) != EOF)
			fputc (c, ioptr);
		fclose (ioptr);
		}
	return (tmpfile);
	}

main (argc, argv) char **argv;
	{
	int 	count;
	char	*file;
	Argv0 = argv[0];
	if (argc == 1)
		USAGE ([file] count)
	if (argc == 2)
		if (number (argv[1]) == 1)
			repeat (saveit (), count = atoi (argv[1]));
		else
			USAGE ([file] count)
	else repeat (argv[1], count = atoi (argv[2]));
	}

repeat (file, n)
char	*file;
	{
	register int C;
	FILE	*ioptr;
	if (n < 0)
		while (n++ < 0)
			puts (file);
	else if (ioptr = fopen (file, "r"))
		{
		while (n-- > 0)
			{
			while ((C = fgetc (ioptr)) != EOF)
				putchar (C);
			if (n > 0) rewind (ioptr);
			}
		fclose (ioptr);
		}
	else
		ERROPEN (file)
	}
