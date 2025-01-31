#include "../../include/header.h"

int	addUnixOption(tInfos* infos, const char* argv)
{
	if (argv[1] == '-')
		return (0);

	for (int i = 1; argv[i] != '\0'; i++)
	{
		if (argv[i] != 'p' && argv[i] != 'r')
			return (1);

		if (argv[i] == 'p' && infos->noSort == false)
			infos->noSort = true;

		if (argv[i] == 'r' && infos->reverseSort == false)
			infos->reverseSort = true;
	}

	return (0);
}

int	addGnuOption(tInfos* infos, const char* argv)
{
	if (argv[1] != '-')
		return (0);

	if (isSame(argv, "--no-sort") == true)
		return (infos->noSort = true);

	if (isSame(argv, "--reverse-sort") == true)
		return (infos->reverseSort = true);

	return (1);
}

void	getOptions(tInfos* infos, const char** argv)
{
	for (int i = 0; argv[i] != NULL; i++)
	{
		if (argv[i][0] == '\0' || argv[i][0] != '-' || argv[i][1] == '\0')
			continue ;

		if (isHelp(argv[i]) == true || addUnixOption(infos, argv[i]) == 1 \
			|| addGnuOption(infos, argv[i]) == 1)
			printHelp(), exit(1);
	}
}
