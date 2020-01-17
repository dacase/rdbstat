/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
canwrite (filename) char *filename;
	{
#if 0
	struct stat	status;
	if (stat (filename, &status)) return (1); /* no such file */
	if (status.st_size == 0) return (1); /* nothing in it */
	return (confirm ("Destroy the contents of %s?", filename));
#else
	return(1);
#endif
	}

#include <sgtty.h>
#ifndef	CBREAK
#define	CBREAK RAW
#endif

#if 0
confirm (msg, arg) char *msg; char *arg;
	{
	char ch;
	struct sgttyb normal, raw;
	if (gtty (fileno (stderr), &normal)) return (0);
	if (gtty (fileno (stderr), &raw)) return (0);
	raw.sg_flags |= CBREAK;
	raw.sg_flags &= ~ECHO;
  getconfirm:
	fprintf (stderr, msg, arg ? arg : "");
	fprintf (stderr, " (y/n) ");
	stty (fileno (stderr), &raw);
	if (read (2, &ch, 1) == 0) return (0);
	stty (fileno (stderr), &normal);
	switch (ch)
		{
		case 'Y': case 'y': fprintf (stderr, "yes\n"); return (1);
		case 'N': case 'n': fprintf (stderr, "no\n"); return (0);
		default: fprintf (stderr, "y for yes, n for no\n");
		}
	goto getconfirm;
	}

#endif

checkstdin (program)
char *program;
	{
# if 0
                        /* dac: always assume not a terminal !?!  */
	struct	sgttyb ttybuf;
	if (gtty (fileno (stdin), &ttybuf) == 0)
		{
		fprintf (stderr,"\007%s: Reading input from terminal:\n", program);
		return (1);
		}
#endif
	return (0);
	}

checkargv (target, argv0)
char	*target, *argv0;
	{
	int	offset = strlen (argv0) - strlen (target);
	if (offset < 0) return (0);
	return (!strcmp (target, argv0+offset));
	}
