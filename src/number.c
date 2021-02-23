/* Copyright (c) 1982 Gary Perlman (see Copyright file) */
/*LINTLIBRARY*/

/* returns 1 for an int, 2 for a real, 0 for non-numbers */
#include <stdio.h>
#include <ctype.h>
int number (char* string) 
	{
	char	answer = 1;
	while (isspace (*string)) string++;
	if (!*string) return (0);
	if (*string == '-') /* optional plus not allowed by atof */
		{
		string++;
		if (!isdigit (*string) && *string != '.') return (0);
		}
	while (isdigit (*string)) string++;
	if (*string == '.')
		{
		answer = 2;
		string++;
		}
	while (isdigit (*string)) string++;
	if (*string == 'E' || *string == 'e')
		{
		answer = 2;
		string++;
		if (*string == '+' || *string == '-') string++;
		while (isdigit (*string)) string++;
		}
	while (isspace (*string)) string++;
	return (*string == '\0' ? answer : 0);
	}
