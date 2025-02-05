#include "../include/header.h"

void	setToDefault(tInfos* infos)
{
	infos->paths = NULL;
	infos->binaries = NULL;

	infos->fd = -1;
	infos->binaryLen = 0;
	infos->binary = NULL;

	infos->returnValue = 0;
	infos->errors = NULL;

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
			if (infos->binaries != NULL && infos->binaries[i] != NULL)
				freeBinary(infos->binaries[i]);
		}
		free(infos->errors);
	}

	if (infos->binaries != NULL)
		free(infos->binaries);

	if (infos->binary != NULL)
		munmap(infos->binary, infos->binaryLen);

	if (infos->fd != -1)
		close(infos->fd);

	if (infos->paths != NULL)
		freeArray(infos->paths);
}

int	main(const int argc, const char **argv)
{
	tInfos	infos;

	(void) argc;

	setToDefault(&infos);

	getOptions(&infos, argv + 1);
	getPaths(&infos, argv + 1);

	getSymbols(&infos);

	listSymbols(&infos);

	setToNull(&infos);

	return (infos.returnValue);
}
