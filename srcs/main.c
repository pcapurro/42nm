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

int	main(const int argc, const char **argv)
{
	tInfos	infos;

	setToDefault(&infos);

	getOptions(&infos, argv + 1);
	getPaths(&infos, argv + 1);

	getSymbols(&infos);

	listSymbols(&infos);

	freeArray(infos.paths);
	freeBinaries(infos.binaries);

	return (0);
}
