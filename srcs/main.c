#include "../include/header.h"

void	setToDefault(tInfos* infos)
{
	infos->error = 0;

	infos->paths = NULL;

	infos->options = false;
	infos->debugOnly = false;
	infos->externOnly = false;
	infos->undefinedOnly = false;

	infos->noSort = false;
	infos->reverseSort = false;
}

int	main(const int argc, const char **argv)
{
	if (argc <= 1)
		{ printError(0); return (1); }
	else
	{
		if (isHelp(argv[1]) == true)
			printHelp();
		else
		{
			tInfos	infos;

			setToDefault(&infos);

			getPaths(&infos, argv);
			getSymbols(&infos);

			readBinary(&infos);
		}
	}

	return (0);
}
