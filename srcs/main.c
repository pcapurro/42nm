#include "../include/header.h"

void	setToDefault(tInfos* infos)
{
	infos->paths = NULL;
	infos->binaries = NULL;

	infos->options = false;
	infos->debugOnly = false;
	infos->externOnly = false;
	infos->undefinedOnly = false;

	infos->noSort = false;
	infos->reverseSort = false;
}

void	setToNull(tInfos* infos)
{
	if (infos->errors != NULL)
	{
		for (int i = 0; infos->paths[i] != NULL; i++)
		{
			if (infos->errors[i] != NULL)
				free(infos->errors[i]);
		}
		free(infos->errors);
	}

	if (infos->paths != NULL)
		freeArray(infos->paths);

	if (infos->binaries != NULL)
		freeBinaries(infos->binaries);
}

int	main(const int argc, const char **argv)
{
	tInfos	infos;

	setToDefault(&infos);

	getOptions(&infos, argv + 1);
	getPaths(&infos, argv + 1);

	getSymbols(&infos);

	listSymbols(&infos);

	setToNull(&infos);

	return (0);
}
