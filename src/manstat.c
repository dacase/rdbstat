#include "unixstat.h"
PGM(manstat, Manual for Statistical Programs)
/* Copyright (c) 1982 Gary Perlman (see Copyright file) */

#include <sys/types.h>
#include <sys/stat.h>

#ifndef DOCDIR
#define DOCDIR "/usr/rdb/stat/doc"
#endif
char	*Docdir = DOCDIR;

initial (argc, argv)
char	**argv;
	{
	int	i;
	char	*ptr;
	int	onint ();
	Argv0 = argv[0];
	if (argc == 1)
		USAGE ([-ddocdir] files)
	for (i = 1; i < argc; i++)
		{
		ptr = argv[i];
		if (*ptr == '-')
			{
			ptr++;
			while (*ptr)
				{
				switch (*ptr)
					{
					case 'd': Docdir = ptr+1; while (*ptr) ptr++; break;
					default:
						ERROPT (*ptr)
					}
				}
			}
		else process (argv[i]);
		}
	}

process (file)
char	*file;
	{
	struct	stat	man, cat;
	char	manfile[BUFSIZ];
	char	catfile[BUFSIZ];
	int	noman, nocat;
	char	command[BUFSIZ];
	sprintf (manfile, "%s/man/%s.1", Docdir, file);
	sprintf (catfile, "%s/cat/%s", Docdir, file);
	noman = stat (manfile, &man);
	nocat = stat (catfile, &cat);
	if (noman)
		if (nocat)
			sprintf (command, "man %s", file);
		else
			sprintf (command, "cat %s", catfile);
	else
		if (nocat || man.st_mtime > cat.st_mtime)
			sprintf (command, "nroff -man %s | uniq > %s; cat %s",
				manfile, catfile, catfile);
		else sprintf (command, "cat %s", catfile);
	system (command);
	}

main (argc, argv)
char	**argv;
	{
	Argv0 = argv[0];
	initial (argc, argv);
	}
