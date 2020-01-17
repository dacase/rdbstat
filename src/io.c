#include "unixstat.h"
PGM(io, Control and Monitor Input and Output)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#include <sys/types.h>
#include <sys/stat.h>
#include <sgtty.h>
#define ttyin !pipein
#define ttyout !pipeout
int 	flowmeter;
int 	forceio;
int 	appendio;
main (argc, argv) char **argv;
	{
	struct	sgttyb	tty;
	int 	pipein = gtty (fileno (stdin), &tty);
	int 	pipeout = gtty (fileno (stdout), &tty);
	Argv0 = argv[0];
	argc = checkoptions (argc, argv);
	if (pipein && pipeout)		/* in the middle of a pipeline */
		{
		if (argc == 1)
			{
			flowmeter = 1;
			foutput (stdin, stdout, 0);
			exit (0);
			}
		else if (argc > 2)
			ERRMSG0 (Can only copy output to one file)
		output (0, argv);
		}
	else if (pipein && ttyout)	/* at the end of the pipeline */
		{
		if (argc == 1)
			ERRMSG0 (No file to output to)
		output (argc, argv);
		}
	else if (ttyin)			/* at beginning of pipeline */
		{
		if (argc == 1)
			ERRDATA
		if (ttyout && flowmeter)
			{
			execv ("/usr/ucb/more", argv);
			WARNING (No flowmeter for tty output)
			flowmeter = 0;
			}
		input (argc, argv);
		}
	exit (0);
	}

checkoptions (argc, argv)
char **argv;
	{
	int 	i, j;
	char	*optr;
	for (i = 1; i < argc; i++)
		if (*argv[i] == '-') /* flag argument */
			{
			optr = &argv[i][1];
			while (*optr)
				{
				switch (*optr)
					{
					case 'a': appendio = 1; break;
					case 'm': flowmeter = 1; break;
					case 'f': forceio = 1; break;
					default:
						ERROPT (*optr)
					}
				optr++;
				}
			for (j = i; j < argc-1; j++) argv[j] = argv[j+1];
			i--, argc--;
			}
	argv[argc] = 0;
	return (argc);
	}

foutput (in, out, pipeout) FILE *in, *out;
	{
	char	*flowchrs = "=*#@$&+";
	char	flowchar = flowchrs[getpid () % 7];
	char	buffer[BUFSIZ];
	int 	nbytes;
	while ((nbytes = fread (buffer, 1, BUFSIZ, in)) == BUFSIZ)
		{
		fwrite (buffer, 1, BUFSIZ, out);
		if (pipeout) fwrite (buffer, 1, BUFSIZ, stdout);
		if (flowmeter) fputc (flowchar, stderr);
		}
	fwrite (buffer, 1, nbytes, out);
	if (pipeout) fwrite (buffer, 1, nbytes, stdout);
	if (flowmeter) fputc ('\n', stderr);
	}

char *
directory (file)
char *file;
	{
	static char dir[BUFSIZ];
	strcpy (dir, file);
	for (file = dir; *file; file++);
	while (file > dir && *(file-1) != '/') file--;
	if (file == dir) strcpy (dir, ".");
	else *file = '\0';
	return (dir);
	}

output (argc, argv) char **argv;
	{
	char	tmp[50];
	char	*filename;
	FILE	*ioptr;
	int 	pipeout = (argc == 0);
	if (argc == 1)
		{
		foutput (stdin, stdout, 0);
		exit (0);
		}
	if (argc > 2)
		ERRMSG0 (Can only output to one file)
	if (access (argv[1], 4) == 0) /* must create tmp file */
		{
		if (access (argv[1], 2) || access (directory (argv[1]), 2))
			/* must be able to overwrite file and directory */
			ERRMSG1 (Cannot overwrite %s, argv[1])
		US_DEBUG (checking overwrite)
		if (forceio == 0)
			if (!confirm ("io: delayed safe overwrite of %s?", argv[1]))
				ERRMSG1 (Exiting to avoid overwriting %s, argv[1])
		US_DEBUG (saving text in /tmp file)
		filename = argv[1] + strlen (argv[1]);
		while (filename > argv[1] && *(filename-1) != '/') filename--;
		sprintf (tmp, "/tmp/%d%s", getpid (), filename);
		filename = tmp;
		if ((ioptr = fopen (filename, "w")) == NULL)
			ERROPEN (filename)
		}
	else
		{
		filename = argv[1];
		if ((ioptr = fopen (filename, "w")) == NULL)
			ERROPEN (filename)
		}
	foutput (stdin, ioptr, pipeout);
	fclose (ioptr);
	if (filename == tmp)
		{
		char	copycomm[100];
		sprintf (copycomm, "cat %s %s %s", tmp, appendio ? ">>" : ">", argv[1]);
		system (copycomm);
		unlink (tmp);
		}
	exit (0);
	}

input (argc, argv) char **argv;
	{
	int 	i;
	struct	stat statbuf;
	FILE	*ioptr;
	register int	C;
	long	size = 0, sofar = 0;
	for (i = 1; i < argc; i++)
		{
		if (stat (argv[i], &statbuf))
			ERROPEN (argv[i])
		else size += statbuf.st_size;
		}
	size /= 10;
	for (i = 1; i < argc; i++)
		{
		if ((ioptr = fopen (argv[i], "r")) == NULL)
			ERROPEN (argv[i])
		while ((C = fgetc (ioptr)) != EOF)
			{
			putchar (C);
			if (flowmeter && (++sofar%size == 0))
				fprintf (stderr, "%4D%%",10*sofar/size);
			}
		fclose (ioptr);
		}
	if (flowmeter) fputc ('\n', stderr);
	exit (0);
	}
