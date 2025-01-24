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

	if (getArrLen(infos.paths) > 1)
		readBinaries(&infos);
	else
		readBinary(&infos);

	freeArray(infos.paths);
	freeArray(infos.binaries);

	return (0);
}

	// printf("paths: \n");
	// for (int i = 0; infos.paths[i] != NULL; i++)
	// 	printf("- %s\n", infos.paths[i]);
	// printf("\n");
	// printf("options: \n");
	// if (infos.debugOnly == true)
	// 	printf("-a\n");
	// if (infos.externOnly == true)
	// 	printf("-g\n");
	// if (infos.noSort == true)
	// 	printf("-p\n");
	// if (infos.reverseSort == true)
	// 	printf("-r\n");
	// if (infos.undefinedOnly == true)
	// 	printf("-u\n");
	// if (infos.options == false)
	// 	printf("none\n");

	// exit(0);