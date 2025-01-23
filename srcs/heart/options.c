#include "../../include/header.h"

int	addOption(tInfos* infos, const char* argv)
{
	if (isSame(argv, "-a") == true || isSame(argv, "--debug-syms") == true)
		return (infos->debugOnly = true);

	if (isSame(argv, "-g") == true || isSame(argv, "--extern-only") == true)
		return (infos->externOnly = true);

	if (isSame(argv, "-p") == true || isSame(argv, "--no-sort") == true)
		return (infos->noSort = true);

	if (isSame(argv, "-r") == true || isSame(argv, "--reverse-sort") == true)
		return (infos->reverseSort = true);

	if (isSame(argv, "-u") == true || isSame(argv, "--undefined-only") == true)
		return (infos->undefinedOnly = true);

	return (0);
}

void	getOptions(tInfos* infos, const char** argv)
{
	for (int i = 0; argv[i] != NULL; i++)
	{
		if (argv[i][0] != '-')
			continue ;

		if (isHelp(argv[i]) == true || addOption(infos, argv[i]) == 0)
			printHelp(), exit(1);
		else if (infos->options == false)
			infos->options = true;
	}
}
