#include "../../include/header.h"

int	addUnixOption(tInfos* infos, const char* argv)
{
	if (argv[1] == '-')
		return (0);

	int	value = -1;

	for (int i = 1; argv[i] != '\0'; i++)
	{
		if (argv[i] == 'a')
			{ value++, infos->debug = true; continue ; }

		if (argv[i] == 'g')
			{ value++, infos->externOnly = true; continue ; }

		if (argv[i] == 'p')
			{ value++, infos->noSort = true; continue ; }

		if (argv[i] == 'r')
			{ value++, infos->reverseSort = true; continue ; }

		if (argv[i] == 'u')
			{ value++, infos->undefinedOnly = true; continue ; }

		return (-1);
	}

	return (0);
}

int	addGnuOption(tInfos* infos, const char* argv)
{
	if (argv[1] != '-')
		return (0);

	if (isSame(argv, "--debug-syms") == true)
		return (infos->debug = true);

	if (isSame(argv, "--extern-only") == true)
		return (infos->externOnly = true);

	if (isSame(argv, "--no-sort") == true)
		return (infos->noSort = true);

	if (isSame(argv, "--reverse-sort") == true)
		return (infos->reverseSort = true);

	if (isSame(argv, "--undefined-only") == true)
		return (infos->undefinedOnly = true);

	return (-1);
}

void	getOptions(tInfos* infos, const char** argv)
{
	for (int i = 0; argv[i] != NULL; i++)
	{
		if (argv[i][0] == '\0' || argv[i][0] != '-' || argv[i][1] == '\0')
			continue ;

		if (isHelp(argv[i]) == true \
			|| addUnixOption(infos, argv[i]) == -1 || addGnuOption(infos, argv[i]) == -1)
			printHelp(), exit(1);
	}
}
